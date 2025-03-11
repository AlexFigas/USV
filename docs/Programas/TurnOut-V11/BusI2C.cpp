#include "BusI2C.h"

#include <Arduino.h>
#include <Utils.h>
#include <Wire.h>

void initializeWire(int dataPin, int clockPin) {
	DebugMessagePrintf( "BusI2C::initializeWire(DataPin=%d,ClockPin=%d)...\n", dataPin, clockPin );
  
	Wire.begin( dataPin, clockPin );
}
