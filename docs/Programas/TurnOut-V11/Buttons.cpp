#include "Buttons.h"
#include "Display.h"
#include "Settings.h"
#include "ServoModelRailroad.h"

#include <Utils.h>
#include <WiFi.h>

extern unsigned long lastDisplayUpdateTime;

static const uint8_t pinButtonbShowStatus = 0; // GPIO 0

void initializeButtons(Button2& bShowStatus) {
  DebugMessagePrintf( "Buttons::initializeButtons(Button2)\n" );
  
  bShowStatus.begin( pinButtonbShowStatus );
  bShowStatus.setClickHandler( handleClick );
}

void handleClick(Button2& bShowStatus) {
  DebugMessagePrintf( "Buttons::handleClick(Button2)\n" );

  clearDisplay();
  lastDisplayUpdateTime = millis();

  PtrSettingsWiFi settingsWiFi = getSettingsWiFi();

  if ( settingsWiFi->apMode ) {
    myPrintf( 
      getDisplay(), 
      "SSID: %s.\nIP: %s.\n", 
      settingsWiFi->ssid, WiFi.softAPIP().toString().c_str() );
  }
  else {
    myPrintf( 
      getDisplay(), 
      "SSID: %s.\nIP: %s.\nRSSI: %d dBm\n", 
      WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), WiFi.RSSI() );
  }
}

