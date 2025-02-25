/*
 * Setting the macro "DebugMode" here only works for files that exists in the projet directory.
 * 
 * In order for the macro work in all files (including the libraries) we need to define the macro in the compiler command line
 * For each platfor edit the file "platform.local.txt" and add the option:
 * 
 * -DDebugMode
 * 
 * to the properties "compiler.cpp.extra_flags"
 * 
 * The file "platform.local.txt" must be defined for each hardware platform.
 * 
 * For example, for the ESP32 the file is:
 * 
 * $HOMEPATH$\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.6
 * 
 * where $HOMEPATH$ is the home directory of the user (in my case is C:\Users\cgonc )
 */

// Print utilities
#include <Utils.h>

// Board compatibility
#include <UtilsBoards.h>

// I2C related
#include <UtilsI2C.h>
#include <Wire.h>

// Interval time between two scans - Value in milliseconds
const unsigned long scanTime = 5000;

// Time where last I2C scan was done
unsigned long lastScanTime;

void setup() {
	// Wait 2 seconds...
	DebugDelay( 2000 );

	// Initialize default serial port and wait until is is ready
	serialBegin( 115200 );

	myPrintf( "I2C Scanner.\n" );

	showBoardType();
		
	Wire.begin( SDA, SCL );
	myPrintf( "Wire is ready.\n" );

	myPrintf( "I2C scanner is ready.\n" );

	lastScanTime = millis();
}

void loop() {
	if ( (millis()-lastScanTime)>scanTime ) {
		lastScanTime = millis();
		
		scanBus( Wire );

		// Or
		scanBus( Wire, Serial );
	}
}