/*********
  Adapted from https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

#define DebugMode

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino_JSON.h>
#include <LoRa.h>
#include <SPI.h>
#include <SPIFFS.h>
#include <Utils.h>
#include <Wire.h>

// Define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

// Define the used band
#define BAND 866E6

// Define the spread factor
// SF =  7  ->  greater data rate and lower symbols airtime
// SF = 12  ->  highest sensitivity and transmission range with the lowest data rate and higher energy consumption
#define SF 12

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SleepTimeInSeconds 10
#define DisplayTimeInSeconds 2

JSONVar settings;

// Packet counter
//RTC_DATA_ATTR int counter = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

long secondsToMicros(int seconds) {
  return (long)1000000 * (long)seconds; 
}

long secondsToMillis(int seconds) {
  return (long)1000 * (long)seconds;
}

void listFilesInDir(File dir, int numTabs = 1) {
  while (true) {
    File entry =  dir.openNextFile();
    if ( !entry ) {
      // no more files in the folder
      break;
    }
    for ( uint8_t i=0; i<numTabs; i++ ) {
      Serial.print('\t');
    }
    Serial.print( entry.name() );
    
    if ( entry.isDirectory() ) {
      Serial.println("/");
      listFilesInDir(entry, numTabs + 1);
    }
    else {
      // display size for file, nothing for directory
      Serial.print( "\t\t" );
      Serial.println( entry.size(), DEC);
    }
    entry.close();
  }
}

// Get all information of SPIFFS
void showFileSystemInfo() {
  unsigned int totalBytes = SPIFFS.totalBytes();
  unsigned int usedBytes = SPIFFS.usedBytes();
 
  Serial.println("===== File system info =====");
 
  Serial.print( "Total space:      ");
  Serial.print( totalBytes);
  Serial.println( "byte");
 
  Serial.print("Total space used: ");
  Serial.print(usedBytes);
  Serial.println("byte");
 
  Serial.println();
 
  // Open dir folder
  File dir = SPIFFS.open("/");
  
  // List file at root
  listFilesInDir( dir );

  dir.close();
}

void setup() {
  DebugSerialBegin( 115200 );

  // Reset OLED display via software
  pinMode( OLED_RST, OUTPUT);
  digitalWrite( OLED_RST, LOW);
  delay( 20 );
  digitalWrite( OLED_RST, HIGH);

  // Initialize Wire
  Wire.begin(OLED_SDA, OLED_SCL);

  // Initialize OLED
  if( !display.begin( SSD1306_SWITCHCAPVCC, 0x3c, false, false) ) {
    DebugMessagePrintf( "SSD1306 allocation failed.\n" );
    for(;;)
      ;
  }
  display.ssd1306_command( SSD1306_DISPLAYON );
  display.clearDisplay();
  display.display();

  if( !SPIFFS.begin() ) {
    DebugMessagePrintf( "An Error has occurred while mounting SPIFFS\n" );
    for(;;)
      ;
  }

  DebugMessagePrintf( "SPIFFS is mounted\n" );

  File settingsFile = SPIFFS.open( "/settings.json", "r" );
  String settingsAsString = settingsFile.readString();
  settingsFile.close();

  settings = JSON.parse( settingsAsString.c_str() );
  const char* tag = (const char*)settings[ "Tag" ];
  int counter = (int)settings[ "Counter" ];
  
  DebugMessagePrintf( "settings[ Tag ] = %s\n", tag );
  DebugMessagePrintf( "settings[ Counter ] = %d\n", counter  );
  
  DebugMessagePrintf( "LoRa Sender Test\n" );

  // SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);

  // Setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if ( !LoRa.begin( BAND ) ) {
    DebugMessagePrintf( "Starting LoRa failed!\n" );
    for(;;)
      ;
  }

  LoRa.setSpreadingFactor( SF );

  LoRa.beginPacket();
    myPrintf( LoRa, "%s %d", tag, counter );
    //LoRa.print( tag );
    //LoRa.print( " " );
    //LoRa.print( counter );
  LoRa.endPacket();
  
  LoRa.sleep();

  display.clearDisplay();
  display.setTextColor( WHITE );
  display.setTextSize( 1 );
  display.setCursor( 0, 0);

  myPrintf( display, "LoRa Sender\n\n" );
  myPrintf( display, "LoRa Initializing OK\n" );
  myPrintf( display, "LoRa SF=%d\n\n", SF );
  myPrintf( display, "LoRa packet sent\n" );
  myPrintf( display, "Counter: %d\n", counter );

  display.display();

  DebugMessagePrintf( "LoRa Sender\n" );
  DebugMessagePrintf( "LoRa Initializing OK\n" );
  DebugMessagePrintf( "LoRa SF=%d\n", SF );
  DebugMessagePrintf( "LoRa packet sent\n" );
  DebugMessagePrintf( "Counter: %d\n\n", counter );

  settings[ "Counter" ] = ++counter;
  settingsFile = SPIFFS.open( "/settings.json", "w" );
  settingsFile.print( JSON.stringify( settings ) );
  settingsFile.close();

  delay( secondsToMillis( DisplayTimeInSeconds ) );
  
  display.ssd1306_command( SSD1306_DISPLAYOFF );

  Serial.flush();
 
  esp_sleep_enable_timer_wakeup( secondsToMicros( SleepTimeInSeconds - DisplayTimeInSeconds ) );
  esp_deep_sleep_start();
}

void loop() {
}
