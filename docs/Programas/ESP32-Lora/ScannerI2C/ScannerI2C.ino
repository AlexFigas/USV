
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Wire.h>

#include <Utils.h>
#include <UtilsI2C.h>

const int dataPinSDA = 21;
const int dataPinSCL = 22;

const int dataPinOledSDA = 4;
const int dataPinOledSCL = 15;
const int dataPinOledRST = 16;

const uint8_t screenWidth = 128;
const uint8_t screenHeight = 64;

Adafruit_SSD1306* display;

const int StateScanWire = 0;
const int StateScanWireDisplay = StateScanWire + 1;
const int StatePause = StateScanWireDisplay + 1;

const unsigned long StateScanInterval = 2000;
const unsigned long StatePauseInterval = 4000;

unsigned long lastTime;
int currentState;
int counter;

void initDisplay() {
  pinMode( dataPinOledRST, OUTPUT );

  digitalWrite( dataPinOledRST, LOW);
  delay( 20 );
  digitalWrite( dataPinOledRST, HIGH);

  Wire1.begin( dataPinOledSDA, dataPinOledSCL );
  Serial.println( "Wire1 (for display) is ready." );

  display = new Adafruit_SSD1306( screenWidth, screenHeight, &Wire1, dataPinOledRST );

  if( !display->begin( SSD1306_SWITCHCAPVCC, 0x3c, false, false) ) {
    DebugMessagePrintf( "SSD1306 allocation failed.\n" );
    display = NULL;
    return;
  }

  DebugMessagePrintf( "SSD1306 is ready.\n" );

  display->ssd1306_command( SSD1306_DISPLAYON );
  display->clearDisplay();
  display->display();
}

void clearDisplay() {
  display->clearDisplay();
  display->setTextColor( WHITE );
  display->setTextSize( 1 );
  display->setCursor( 0, 0);
  display->display();
}

Print* getDisplay() {
  return (Print*)( display );
}

void setup() {
  DebugDelay( 2000 );

  serialBeginNoBlock( 115200 );

  Serial.println( "Serial ready." );

  initDisplay();
  Serial.println( "Display ready." );

  Serial.println( "I2C Scanner." );
  display->println( "I2C Scanner." );
  
  Wire.begin( dataPinSDA, dataPinSCL );

  Serial.println( "Wire is ready." );
  display->println( "Wire is ready.");

  scanBus( Wire, getDisplay(), "Wire", true );
  display->display();
  DebugDelay( 1000 );
  clearDisplay();

  scanBus( Wire1, getDisplay(), "Wire1", true );
  display->display();
  DebugDelay( 1000 );
  clearDisplay();

  lastTime = millis();

  Serial.println( "Setup is ready." );
  DebugDelay( 1000 );
  clearDisplay();

  currentState = StateScanWire;
  counter = 0;
}

void loop() {
  unsigned long currentTime = millis();

  switch ( currentState ) {
    case StateScanWire:
      if ( ( currentTime-lastTime)>StateScanInterval ) {
        lastTime = currentTime;

        clearDisplay();
        Print* pr = getDisplay();
        Serial.printf( "Counter = %d\n", counter  );
        if ( pr!=NULL ) pr->printf( "Counter = %d\n", counter );
        ++counter;

        scanBus( Wire, pr, "Wire", true );
        display->display();

        currentState = StateScanWireDisplay;
      }
      break;

    case StateScanWireDisplay:
      if ( ( currentTime-lastTime)>StateScanInterval ) {
        lastTime = currentTime;

        clearDisplay();
        Print* pr = getDisplay();
        Serial.printf( "Counter = %d\n", counter  );
        if ( pr!=NULL ) pr->printf( "Counter = %d\n", counter );
        ++counter;

        scanBus( Wire1, pr, "Wire1", true );
        display->display();

        currentState = StatePause;
      }
      break;
  
    case StatePause:
      if ( ( currentTime-lastTime)>StatePauseInterval ) {
        lastTime = currentTime;

        currentState = StateScanWire;

        clearDisplay();
      }
      break;
  }
}
