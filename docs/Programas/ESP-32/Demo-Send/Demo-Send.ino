/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DebugMode
#include <Utils.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America

#define BAND 866E6

//#define SF 7    // greater data rate and lower symbols airtime
#define SF 12   // highest sensitivity and transmission range with the lowest data rate and higher energy consumption

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SenderTag "Cajo "
//packet counter
int counter = 0;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
  delay( 2000 );

  serialBegin( 115200 );

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    DebugMessagePrintf( "SSD1306 allocation failed.\n" );
    for(;;)
      ;
  }
  
  display.clearDisplay();
  display.setTextColor( WHITE );
  display.setTextSize( 1 );
  display.setCursor( 0, 0);
  
  myPrintf( display, "LoRa SENDER\n" );
  display.display();

  DebugMessagePrintf( "LoRa Sender Test.\n" );

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);

  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    DebugMessagePrintf( "Starting LoRa failed!\n" );
    for(;;)
      ;
  }

  LoRa.setSpreadingFactor( SF );
      
  myPrintf( display, "LoRa Initializing OK\n" );
  myPrintf( display, "LoRa SF=%d\n", SF );
  display.display();
  
  delay( 2000 );
}

void loop() {
   
  DebugMessagePrintf( "Sending packet: %d\n", counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print( SenderTag );
  LoRa.print( counter );
  LoRa.endPacket();
  LoRa.sleep();
  
  display.clearDisplay();
  display.setCursor(0,0);
  
  myPrintf( display, "LoRa SENDER\n" );
  myPrintf( display, "LoRa SF=%d\n\n", SF );

  myPrintf( display, "LoRa packet sent\n" );
  myPrintf( display, "Counter: %d\n", counter );
        
  display.display();

  counter++;
  
  delay( 10000 );
}
