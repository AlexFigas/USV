#define PROJECT     "Cajo PZEM-004T-100A(v3.0) with class in external library and state machine on loop"
#define FILE_NAME   "PowerMeter-V4.ino"

/*
 * To use the ESP32 TTGO display do not forget to uppdate the file:
 * 
 * Arduino\libraries\TFT_eSPI_Setups\TFT_eSPI_Setups.h to include the file "Setup25_TTGO_T_Display.h"
 * 
 * Also after a library update ensure that the file:
 * 
 * Arduino\libraries\TFT_eSPI_Setups\User_Setup_Select.h includes the file "../TFT_eSPI_Setups/TFT_eSPI_Setups.h"
 */

#include "MyDisplay.h"
#include "MyPowerSocket.h"
#include "MyPZEM.h"
#include "MyTime.h"
#include "MyWebroutes.h"
#include "WirelessCommunication.h"

#include <Utils.h>
#include <UtilsBoards.h>

// If the next define is set and the board is an ESP32 the serial port used to communicate with the PZEM is hardware based
//#define UseHardSerial

// On the TTGO
#ifdef ARDUINO_TTGO_T1
    // Button pin
    #define buttonShowDataPin 0
    
    // Serial Port pins used to communicate with the PZEM
    #define SerialPinRX 32
    #define SerialPinTX 33
#endif

// On the ESP32
#ifdef ARDUINO_ESP32_DEV
    // Button pin
    #define buttonShowDataPin 27

    // Serial Port pins used to communicate with the PZEM
    #ifdef UseHardSerial
        #define SerialPinRX 16
        #define SerialPinTX 17
    #else
        #define SerialPinRX 32
        #define SerialPinTX 33
    #endif
#endif

// On the ESP8266-12
#ifdef ESP8266_WEMOS_D1MINI
    // Button pin
    #define buttonShowDataPin 16
    
    // Serial Port pins used to communicate with the PZEM
    #define SerialPinRX 13
    #define SerialPinTX 15
#endif

// To use a button to show PZEM values on a TFT Display
#include <Button2.h>

// Web server, including to be used in Over The Air Update (OTA Update)
#include <ESPAsyncWebServer.h>

// To use Over The Air Update
#include <AsyncElegantOTA.h>    

// Web server
AsyncWebServer server(80);

// OTA Update credentials
#define UserNameOTA "admin"
#define PasswordOTA "@dmin"

// Time interval to update the PZEM values
const unsigned long UpdateTimeInterval = 1000;
unsigned long lastUpdateTime;

// Time for the display being on after press the button
const unsigned long ShowTime = 5000;
unsigned long lastShowTime;

// The display
TFT_eSPI display = TFT_eSPI();

// Button used to trigger the display of data
Button2 buttonShowData = Button2( buttonShowDataPin );

// The PZEM
PtrPZEM pzem;

// Values for the state machine
const int StateNop = 0;
const int StartShowingValues = 1;
const int ShowingValues = 2;

int currentState;

void showStatus(Print& out=Serial) {
    PtrValuesPZM values = pzem->getValues();
    
    myPrintf( out, "V: %4.2f\n",   !isnan(values->voltage) ? values->voltage : -220.0 );
    myPrintf( out, "C: %4.2f\n",   !isnan(values->current) ? values->current: -1.0 );
    myPrintf( out, "P: %4.2f\n",   !isnan(values->power) ? values->power : -1.0 );
    myPrintf( out, "E: %4.2f\n", !isnan(values->energy) ? values->energy : -1.0 );
    myPrintf( out, "F: %4.2f\n",  !isnan(values->frequency) ? values->frequency : -50.0 );
    myPrintf( out, "PF: %4.2f\n\n",  !isnan(values->powerFactor) ? values->powerFactor : -1.0 );

    myPrintf( out, "%s",  WiFi.localIP().toString().c_str() );
}

void clickButtonShowData(Button2& btn) {
    currentState = StartShowingValues;
}

void setup() {
    DebugDelay( 2000 );
    
    DebugSerialBeginNoBlock( 115200 );

    DebugMessagePrintf( "Setup:\n" );
    DebugMessagePrintf( "Project: %s\n", PROJECT );
    DebugMessagePrintf( "File name: %s\n\n", FILE_NAME );

    showBoardType();

    if ( !SPIFFS.begin() ) {
        DebugMessagePrintf( "SPIFFS failed!\n" );
    }
    else {
        DebugMessagePrintf( "SPIFFS is ready.\n" );
    }

    initDisplay( display );
    DebugMessagePrintf( "Display is ready.\n" );

    buttonShowData.setClickHandler( clickButtonShowData );

    initPowerSocket();

#ifdef ARDUINO_TTGO_T1
    // Board is a TTGO ESP32
    pzem = new PZEM( SerialPinRX, SerialPinTX, PZEM::DefaultAddressPZEM );
#endif

#ifdef ARDUINO_ESP32_DEV
    // Board is a standard ESP32
    #ifdef UseHardSerial
        pzem = new PZEM( &Serial2, SerialPinRX, SerialPinTX, PZEM::DefaultAddressPZEM);
    #else
        pzem = new PZEM( SerialPinRX, SerialPinTX, PZEM::DefaultAddressPZEM );
    #endif
#endif

#ifdef ESP8266_WEMOS_D1MINI
    // Board is a TTGO ESP32
    pzem = new PZEM( SerialPinRX, SerialPinTX, PZEM::DefaultAddressPZEM );
#endif

    initializeWiFi();

    clearDisplay( display );
    myPrintf( display, "%s\n", WiFi.localIP().toString().c_str() );

    initTime();

    // Begin Over the Air Update (OTA)
    // Start AsyncElegantOTA
    AsyncElegantOTA.begin( &server, UserNameOTA, PasswordOTA );   

    // Start AsyncWebServer
    server.begin();                     
    // End OTA
    DebugMessagePrintf( "HTTP server started.\n" );

    webRoutesSetup( server, pzem );

    myPrintf( display, "Ready at:\n%s", WiFi.localIP().toString().c_str() );
    delay( 2000 );

    currentState = StartShowingValues;

    lastUpdateTime = lastShowTime = millis();
}

void loop() {
    unsigned long currentTime = millis();

    switch ( currentState ) {
        case StateNop:
            break;

        case StartShowingValues:
            lastShowTime = currentTime;

            onDisplay();
            clearDisplay( display );
            showStatus( display );

            currentState = ShowingValues;
            break;

        case ShowingValues:
            if ( ((unsigned long)(currentTime-ShowTime))>lastShowTime ) {
                offDisplay();
                currentState = StateNop;
            }
            break;
    }
  
    if ( ( (unsigned long)( currentTime-lastUpdateTime ) ) > UpdateTimeInterval ) {
        lastUpdateTime = currentTime;
        pzem->updateValues();
    }
  
    //AsyncElegantOTA.loop();

    buttonShowData.loop();

    yield();
}
