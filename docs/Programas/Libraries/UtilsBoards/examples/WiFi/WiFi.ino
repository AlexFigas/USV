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

WiFiMultiEntry creds[] = {
  { "SSID1", "pass1" },
  { "SSID2", "pass2" }
};

void setup() {
	// The next delay is executed only  if the compliler flag -DDebugMode is set
	DebugDelay( 2000 );
	
	serialBegin( 115200 );

	// The next message is printed only if the compliler flag -DDebugMode is set
	DebugMessagePrintf( "Board Testing.\n" );

	showBoardType();

	initInternalLed();

	// The next message is allways printed
	myPrintf( "Starting WiFi...\n" );

	// The debug messages inside the function "initWiFi" are only printed if the compliler flag -DDebugMode is set
		// Init WiFi with with minimal settings
		initWiFi( creds[0].ssid, creds[0].password );
		
		// Init WiFi with a reset if connection fails 
		if ( initWiFi( creds[0].ssid, creds[0].password, false )==false ) {
			DebugMessagePrintf( "WiFi connection failed!\n!" );
		}
		
		// Init WiFi and blink internal led during establishment of the connection 
		initWiFi( creds[0].ssid, creds[0].password, true, true );

		// Init WiFi and set ESP DNS hostname
		initWiFi( creds[1].ssid, creds[1].password, true, true, "myESP32");
		
		// Init WiFi and specify the ESP DNS hostname
		initWiFi( creds[1].ssid, creds[1].password, true, true, "myESP32");
		
		// Init WiFi, do not specify the ESP DNS hostname, and specify a Print object where the debug messages are printed
		initWiFi( creds[1].ssid, creds[1].password, true, true, NULL, Serial);

		// Init WiFi using multiple credentials
		initWiFi( creds, 2 );
		
		// Init WiFi with a reset if connection fails
		if ( initWiFi( creds, 2, false )==false ) {
			DebugMessagePrintf( "WiFi (multi) connection failed!\n" );
		}
		
		initWiFi( creds, 2, false, true );
		
		initWiFi( creds, 2, false, true, "ESP8266" );
		
		initWiFi( creds, 2, false, true, "ESP8266", Serial );

	// The next message is allways printed
	myPrintf( "WiFi is ready.\n" );

	delay( 2000 );
}

void loop() {
	blinkInternalLed( 300, 700 );
}
