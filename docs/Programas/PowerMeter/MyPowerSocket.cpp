#include "MyPowerSocket.h"

#include <Utils.h>

// Control the relay to activate the power on the socket
#ifdef ARDUINO_TTGO_T1
    #define PowerSocketPin  17
#else
  #ifdef ARDUINO_ESP32_DEV
    #define PowerSocketPin  14
  #else
    #define PowerSocketPin  5
  #endif
#endif

// Stores power socket state
String statePowerSocket;

void initPowerSocket(void) {
    DebugMessagePrintf( "initPowerSocket()\n" );
  
	pinMode( PowerSocketPin, OUTPUT );
	
	statePowerSocket = "Off";
	
	offPowerSocket();
}

void onPowerSocket(void) {
	digitalWrite( PowerSocketPin, HIGH );
}

void offPowerSocket(void) {
	digitalWrite( PowerSocketPin, LOW );
}

String statusPowerSocket(void) {
  if ( digitalRead( PowerSocketPin ) ) {
    statePowerSocket = "On";
  }
  else {
    statePowerSocket = "Off";
  }
  
  return statePowerSocket;
}
