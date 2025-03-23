#ifndef _UtilsBoards_h_
#define _UtilsBoards_h_

#ifdef ARDUINO_ARCH_ESP32
// Boards based on ESP32

// Custom defines
#define BOARD_FAMILY "ESP32"
#define BOARD_NAME ARDUINO_BOARD

// Includes for WiFi and asyncronous Web Server
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>

// #include <esp_wpa2.h>
// #include <esp_eap_client.h>

#include <AsyncTCP.h>

// Includes for file system
#include <SPIFFS.h>

// I2C pins
#define SDA 21
#define SCL 22

#ifndef LED_BUILTIN
#define LED_BUILTIN -1
#endif
#else
#ifdef ARDUINO_ARCH_ESP8266
// Boards based on ESP8266 or ESP8285

// Custom defines
#define BOARD_FAMILY "ESP8266"
#define BOARD_NAME ARDUINO_BOARD

// Includes for WiFi and asyncronous Web Server
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>

#include <ESPAsyncTCP.h>

typedef ESP8266WiFiMulti WiFiMulti;

// Includes for file system
#include <FS.h>

#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
// I2C pins
#define SDA 19
#define SCL 20
#endif

#if defined(ESP8266_GENERIC) || defined(ESP8266_ESP01)
// I2C pins
#define SDA 0
#define SCL 2
#endif
#else
#ifdef ARDUINO_ARCH_AVR
// Boards based on Arduino Uno
#define BOARD_FAMILY "Arduino Uno"
#define BOARD_NAME BOARD_FAMILY

// I2C pins
#define SDA -1
#define SCL -1
#else
#ifdef ARDUINO_ARCH_MBED_NANO
// Boards based Arduino Nano 33 BLE
#define BOARD_FAMILY "Arduino Nano 33"
#define BOARD_NAME BOARD_FAMILY

// I2C pins
#define SDA -1
#define SCL -1

#else
// Boards not used so far
#define BOARD_FAMILY "???"
#define BOARD_NAME BOARD_FAMILY

// I2C pins
#define SDA -1
#define SCL -1

#define LED_BUILTIN -1
#endif
#endif

typedef void WiFiMulti;
#endif
#endif

#include <Print.h>

/**
        \brief
        Struct to hold the WiFi credentials.

        A WiFi credential is made by the name of the <b>SSID</b> and the corresponding <b>password</b>.

        \param ssid the <b>SSID</b> name of the network
        \param password the <b>password</b> name of the network
*/
typedef struct
{
    const char* ssid;
    const char* password;
} WiFiMultiEntry, *PtrWiFiMultiEntry;

/**
        \brief
        Turn inboard LED <b>ON</b>.

        Utility macro to turn <b>ON</b> the internal LED of the board.
*/
#define internalLedOn() digitalWrite(LED_BUILTIN, LOW)

/**
        \brief
        Turn inboard LED <b>OFF</b>.

        Utility macro to turn <b>OFF</b> the internal LED of the board.
*/
#define internalLedOff() digitalWrite(LED_BUILTIN, HIGH)

/**
        \brief
        Initialize the internal LED.

        This function configures the pin associatied with the internal LED to a digital output and turns off the
   internal LED.
*/
void initInternalLed();

/**
        \brief
        Blink the internal LED of the board.

        Function to blink the internal LED of the board. Internally uses the <code>delay(...)</code> function.

        \param onTime the amount of time to maintain the internal LED <b>ON</b>
        \param offTime the amount of time to maintain the internal LED <b>OFF</b>
*/
void blinkInternalLed(int onTime, int offTime);

/**
        \brief
        Show the board name.

        Utility function to show the name of the current board. By default the board name is printed to the default
   <code>Serial</code> port.

        \param pr Arduino <code>Print</code> object that suports the <b><code>print</code></b> and
   <b><code>println</code></b> methods
*/
void showBoardType(Print& pr = Serial);

/**
        \brief
        Initialize the WiFi.

        Function to initialize the WiFi component of the board in station mode (<code>WIFI_STA</code>).

        Beasides the authentication credentials (name of the <b>SSID</b> and corresponding <b>password</b>) this
   function accepts a set of aditional arguments that can be used to:

        <ul>
                <li>Reset the board if no connection could be established.</li>
                <li>Blink the internal LED while wainting for the establishment of the WiFi connection.</li>
                <li>Set the DNS host name of the board.</li>
                <li>Specify a <code>Print</code> object to show debug messages.</li>
        </ul>

        \param ssid name of the network
        \param password the matching password for the specified <code>SSID</code>
        \param resetIfNoWiFi boolean flag that can be used to reset the board if no connection could be done within 30
   seconds. This is the default behavior \param blinkInternal boolean flag that can be used to controll the blinking of
   the interal LED while waiting for the establishment of the WiFi connection. The default behavior is to not blink the
   internal LED \param hostName DNS name of the board. By default the DNS name is not set. \param pr Arduino
   <code>Print</code> object that suports the <b><code>print</code></b> and <b><code>println</code></b> methods. By
   default these messages are shown in the default <code>Serial</code> port

        \return <b><code>true</code></b> if it was possible to connect the the specified network. In case of failure, if
   the flag <b><code>resetIfNoWiFi</code></b> is set to <b><code>true</code></b> the board is reset, else the value
   <b><code>false</code></b> is returned.

*/
bool initWiFi(const char* ssid,
              const char* password,
              const bool resetIfNoWiFi = true,
              const bool blinkInternal = false,
              const char* hostName = NULL,
              Print& pr = Serial);

/*
        \brief
        Initialize the WiFi.

        Function to initialize the WiFi component of the board in station mode (<code>WIFI_STA</code>).

        This function is the version to be used in networks with WPA2 authentication.

        Beasides the authentication credentials (name of the <b>SSID</b>, <b>identity/username</b> and <b>password</b>)
   this function accepts a set of aditional arguments that can be used to:

        <ul>
                <li>Reset the board if no connection could be established.</li>
                <li>Blink the internal LED while wainting for the establishment of the WiFi connection.</li>
                <li>Set the DNS host name of the board.</li>
                <li>Specify a <code>Print</code> object to show debug messages.</li>
        </ul>

        \param ssid name of the network
        \param identity user name
        \param password the matching password for the specified user
        \param resetIfNoWiFi boolean flag that can be used to reset the board if no connection could be done within 30
   seconds. This is the default behavior \param blinkInternal boolean flag that can be used to controll the blinking of
   the interal LED while waiting for the establishment of the WiFi connection. The default behavior is to not blink the
   internal LED \param hostName DNS name of the board. By default the DNS name is not set. \param pr Arduino
   <code>Print</code> object that suports the <b><code>print</code></b> and <b><code>println</code></b> methods. By
   default these messages are shown in the default <code>Serial</code> port

        \return <b><code>true</code></b> if it was possible to connect the the specified network. In case of failure, if
   the flag <b><code>resetIfNoWiFi</code></b> is set to <b><code>true</code></b> the board is reset, else the value
   <b><code>false</code></b> is returned.

*/
/*
bool initWiFi(
        const char* ssid,
        const char* identity,
        const char* password,
        const bool resetIfNoWiFi=true,
        const bool blinkInternal=false,
        const char* hostName=NULL,
        Print& pr=Serial);
*/

/**
        \brief
        Initialize the WiFi.

        Function to initialize the WiFi component of the board in station mode (<code>WIFI_STA</code>).

        This function receives a set of authentication credentials (name of the <b>SSID</b> and corresponding
   <b>password</b>). Thus board tries to establish a connection with every networs in the list until establish a
   connection.

        This function also accepts a set of aditional arguments that can be used to:

        <ul>
                <li>Reset the board if no connection could be established.</li>
                <li>Blink the internal LED while wainting for the establishment of the WiFi connection.</li>
                <li>Set the DNS host name of the board.</li>
                <li>Specify a <code>Print</code> object to show debug messages.</li>
        </ul>

        \param creds set of authentication credentials
        \param sizeOfCreds number of authentication credentials
        \param resetIfNoWiFi boolean flag that can be used to reset the board if no connection could be done within 30
   seconds. This is the default behavior \param blinkInternal boolean flag that can be used to controll the blinking of
   the interal LED while waiting for the establishment of the WiFi connection. The default behavior is to not blink the
   internal LED \param hostName DNS name of the board. By default the DNS name is not set. \param pr Arduino
   <code>Print</code> object that suports the <b><code>print</code></b> and <b><code>println</code></b> methods. By
   default these messages are shown in the default <code>Serial</code> port

*/
bool initWiFi(PtrWiFiMultiEntry creds,
              int sizeOfCreds,
              const bool resetIfNoWiFi = true,
              const bool blinkInternal = false,
              const char* hostName = NULL,
              Print& pr = Serial);

#endif  // _UtilsBoards_h_
