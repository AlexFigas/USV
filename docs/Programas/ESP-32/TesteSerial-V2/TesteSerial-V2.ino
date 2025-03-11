#include <SoftwareSerial.h>
#include <Utils.h>

const unsigned long DelaySerial = 10;

unsigned long lastTime;

// Pins used by the Software Serial port
const int8_t pinSoftRX = 32;
const int8_t pinSoftTX = 33;

// Pins used by the Hardware Serial 2 port
const int8_t pinHardRX = 16;
const int8_t pinHardTX = 17;

// The baud rate used both in the Software Serial and Hardware Serial 2
//const int myBaud = 9600;
//const int myBaud = 14000;
//const int myBaud = 19200;
//const int myBaud = 38400;
const int myBaud = 57600;
//const int myBaud = 115200;

// Define the sofware serial
SoftwareSerial serialSoft( pinSoftRX, pinSoftTX, false);

void setup() {
  delay( 2000 );

  Serial.begin( 115200 );
  while ( !Serial ) {
    myPrintf( "Waiting for Hard Serial 0...\n" );
  }
  myPrintf( "Hard Serial 0 is ready.\n" );

  Serial2.begin( myBaud, SERIAL_8N1, pinHardRX, pinHardTX );
  while ( !Serial2 ) {
    myPrintf( "Waiting for Hard Serial 2...\n" );
  }
  myPrintf( "Hard Serial 2 is ready.\n" );

  serialSoft.begin( myBaud, SWSERIAL_8N1 );

  while ( !serialSoft ) {
    myPrintf( "Waiting for Software Serial...\n" );
  }
  myPrintf( "Software serial is ready.\n" );
  
  myPrintf( "Serial test is ready.\n" );

  float delta = ( (float)myBaud - (float)serialSoft.baudRate() ) * 100.0f / (float)myBaud;
  myPrintf( "my br=%d, sw br=%d, delta=%f %%\n", myBaud, serialSoft.baudRate(), delta );

  uint8_t rxChar;
  Serial2.read( (uint8_t*)(&rxChar), 1 );

  lastTime = millis();
}

void loop() {

  // Check for incoming data on default serial (Hardware serial 0) every 10 (DelaySerial) ms
  if ( (millis()-lastTime) > DelaySerial ) {
    lastTime = millis();

    // while data is available...
    while ( Serial.available()>0 ) {
      uint8_t rxChar;

      // read it from Hardware serial 0
      Serial.read( (uint8_t*)(&rxChar), 1 );

      // and send it to Software serial
      serialSoft.write( (uint8_t*)(&rxChar), 1 );
    }  
  }

  // Check for incoming data on Software serial and show it on the console 
  while ( serialSoft.available()>0 ) {
    uint8_t rxChar;
    serialSoft.read( (uint8_t*)(&rxChar), 1 );

    Serial.write( (uint8_t*)(&rxChar), 1 );
  }

  // Check for incoming data on Hardware serial 2 
  while ( Serial2.available()>0 ) {
    uint8_t rxChar;
    Serial2.read( (uint8_t*)(&rxChar), 1 );

    Serial2.write( (uint8_t*)(&rxChar), 1 );
  }

  // Run the internal processing and event engine
  serialSoft.perform_work();
}
