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

void setup() {
	DebugDelay( 2000 );

	DebugSerialBegin( 115200 );

	showBoardType();

	void initInternalLed();
}

void loop() {
	blinkInternalLed( 300, 700 );
}