#include "BusI2C.h"
#include "Buttons.h"
#include "Crossing.h"
#include "Display.h"
#include "ExpanderIO.h"
#include "FileSystem.h"
#include "Settings.h"
#include "ServoModelRailroad.h"
#include "WebServerUtils.h"
#include "WiFiUtils.h"

#include <AsyncElegantOTA.h>
#include <Button2.h>
#include <ESPAsyncWebServer.h>
#include <Utils.h>
#include <UtilsI2C.h>

unsigned long lastDisplayUpdateTime;

Button2 buttonShowStatus;

AsyncWebServer serverWeb( 80 );

void setup() {
  DebugDelay( 1000 );
  
  DebugSerialBeginNoBlock( 115200 );

  initializeSPIFFS();

  initializeDisplay();
  delay( getMessageTimeout() );
  clearDisplay();

  initializeWire();
  scanBus( Wire, getDisplay()  );
  delay( getMessageTimeout() );
  clearDisplay();

  readSettings();

  initializeWiFi();

  setupWebRoutes( serverWeb );

  AsyncElegantOTA.begin( &serverWeb, "admin", "@dmin" );

  serverWeb.begin();

  ExpanderPWM::initExpanders();

  ExpanderIO::initExpanders();

  ServoModelRailroad::initServosModelRailroad();

  Crossing::initCrossings();

  initializeButtons( buttonShowStatus );

  lastDisplayUpdateTime = millis();

  myPrintf( getDisplay(), "Setup is ready.\n" );
}

void loop() {
  if ( lastDisplayUpdateTime!=-1 && ( (millis()-lastDisplayUpdateTime)>getMessageTimeout() ) ) {
    clearDisplay();

    lastDisplayUpdateTime = -1;
  }

  buttonShowStatus.loop();

  ServoModelRailroad::loops();

  ExpanderIO::loops();

  Crossing::loops();
}
