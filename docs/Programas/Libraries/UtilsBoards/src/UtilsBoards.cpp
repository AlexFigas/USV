#include <Arduino.h>
#include <Utils.h>
#include <UtilsBoards.h>

// Value in milliseconds
static const int ConnSleepTimeOn = 250;

// Value in milliseconds
static const int ConnSleepTimeOff = 250;

// Value in seconds
static const int ConnectionTimeOut = 30;
static const int ConnSleepTime = (ConnSleepTimeOn + ConnSleepTimeOff);

static const int ConnectionSteps = (1000 / ConnSleepTime) * ConnectionTimeOut;

void showBoardType(Print& pr)
{
    pr.printf("Board family: %s\n", BOARD_FAMILY);
    pr.printf("Board name: %s\n", BOARD_NAME);
}

void blinkInternalLed(int onTime, int offTime)
{
    internalLedOn();
    delay(onTime);
    internalLedOff();
    delay(offTime);
}

void initInternalLed()
{
    pinMode(LED_BUILTIN, OUTPUT);
    internalLedOff();
}

static bool waitAndReset(int& counter, const bool blinkInternal, Print& pr, const bool resetIfNoWiFi)
{

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)

#ifdef DebugModeWiFi
#ifdef DebugMode
    pr.printf("(Counter=%3d, ConnectionSteps=%3d, ConnSleepTime=%5d)\n", counter, ConnectionSteps, ConnSleepTime);
#endif
#else
    pr.printf(".");
#endif

    if (blinkInternal == true)
    {
        blinkInternalLed(ConnSleepTimeOn, ConnSleepTimeOff);
    }
    else
    {
        delay(ConnSleepTime);
    }

    if (++counter > ConnectionSteps)
    {
// After "ConnectionTimeOut" there was no connection to WiFi
#ifdef DebugMode
        pr.printf("\nAfter %d seconds there was no connection to WiFi.\n", ConnectionTimeOut);
#endif

        if (resetIfNoWiFi == true)
        {
#ifdef DebugMode
            pr.printf("Resetting device...");
#endif
            ESP.restart();
        }
        else
        {
            return false;
        }
    }

#endif

    return true;
}

bool initWiFi(const char* ssid,
              const char* password,
              const bool resetIfNoWiFi,
              const bool blinkInternal,
              const char* hostName,
              Print& pr)
{

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
#ifdef DebugMode
    unsigned long timeOnInitWiFiBegin = millis();
#endif

#ifdef DebugMode
    pr.printf("Initializing WiFi(%s, ******, %s, board host name=%s, Print&)\n\n",
              ssid,
              blinkInternal == true ? "blinkOn" : "blinkOff",
              hostName != NULL ? hostName : "Not set");
#endif

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);

    if (hostName != NULL)
    {
#ifdef DebugMode
        pr.printf("Setting hostName to \"%s\".\n", hostName);
#endif

#ifdef ARDUINO_ARCH_ESP8266
        WiFi.hostname(hostName);
#endif

#ifdef ARDUINO_ARCH_ESP32
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        WiFi.setHostname(hostName);
#endif
    }

    WiFi.begin(ssid, password);

    int counter = 0;
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        if (waitAndReset(counter, blinkInternal, pr, resetIfNoWiFi) == false)
        {
            return false;
        }
    }

    if (blinkInternal == true)
    {
        internalLedOff();
    }

// We are connected
#ifdef DebugMode
    pr.printf("\nSSID: %s\n", WiFi.SSID().c_str());
    pr.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    pr.printf("RSSI: %d dBm\n", WiFi.RSSI());
#endif

#ifdef DebugMode
    DebugMessagePrintf("WiFi initialized in %ld miliseconds.\n", ((unsigned long)(millis() - timeOnInitWiFiBegin)));
#endif
#endif

    return true;
}

/*
bool initWiFi(const char* ssid, const char* identity, const char* password, const bool resetIfNoWiFi, const bool
blinkInternal, const char* hostName, Print& pr) {

        #ifdef ARDUINO_ARCH_ESP32
                #ifdef DebugMode
                        unsigned long timeOnInitWiFiBegin = millis();
                #endif

                DebugMessagePrintf(
                        pr,
                        "Initializing WiFi(%s, %s, ******, %s, board host name=%s, Print&)\n\n",
                        ssid,
                        identity,
                        blinkInternal==true ? "blinkOn" : "blinkOff",
                        hostName!=NULL ? hostName : "Not set" );

                WiFi.disconnect();
                WiFi.mode( WIFI_STA );

                // Provide identity
                esp_wifi_sta_wpa2_ent_set_identity( (uint8_t *)identity, strlen(identity) );

                // Provide username (identity and username are the same)
                esp_wifi_sta_wpa2_ent_set_username( (uint8_t *)identity, strlen(identity) );

                // Provide password
                esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password) );

                // Set config settings to default
                //esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();

                // Set config settings to enable function
                //esp_wifi_sta_wpa2_ent_enable( &config );

                esp_wifi_sta_wpa2_ent_enable();

                WiFi.begin( ssid );

                if ( hostName!=NULL ) {
                        DebugMessagePrintf( "Setting hostName to \"%s\".\n", hostName );

                        WiFi.config( INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE );
                        WiFi.setHostname( hostName );
                }

                int counter = 0;
                // Wait for connection
                while ( WiFi.status()!=WL_CONNECTED ) {
                        if ( waitAndReset( counter, blinkInternal, pr, resetIfNoWiFi)==false ) {
                                return false;
                        }
                }

                if ( blinkInternal==true ) {
                        internalLedOff();
                }

                // We are connected
                DebugMessagePrintf( pr, "\nSSID: %s\n", WiFi.SSID().c_str() );
                DebugMessagePrintf( pr, "IP: %s\n", WiFi.localIP().toString().c_str() );
                DebugMessagePrintf( pr, "RSSI: %d dBm\n", WiFi.RSSI() );

                #ifdef DebugMode
                        DebugMessagePrintf( "WiFi initialized in %ld miliseconds.\n" , ( (unsigned long) (millis() -
timeOnInitWiFiBegin) ) ); #endif #endif

        return true;
}
*/

bool initWiFi(PtrWiFiMultiEntry creds,
              int sizeOfCreds,
              const bool resetIfNoWiFi,
              const bool blinkInternal,
              const char* hostName,
              Print& pr)
{

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
#ifdef DebugMode
    unsigned long timeOnInitWiFiBegin = millis();
#endif

#ifdef DebugMode
    pr.printf("Initializing WiFi(PtrWiFiMultiEntry, %d, %s, board host name=%s, Print&)\n\n",
              sizeOfCreds,
              blinkInternal == true ? "blinkOn" : "blinkOff",
              hostName != NULL ? hostName : "Not set");
#endif

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);

    WiFiMulti wifiMulti;

    for (int idxCreds = 0; idxCreds < sizeOfCreds; ++idxCreds)
    {
#ifdef DebugMode
        pr.printf("Adding %s SSID\n", creds[idxCreds].ssid);
#endif
        wifiMulti.addAP(creds[idxCreds].ssid, creds[idxCreds].password);
    }

    DebugMessagePrintf("Done adding credentials.\n");

    if (hostName != NULL)
    {
#ifdef DebugMode
        DebugMessagePrintf("Setting hostName to \"%s\".\n", hostName);
#endif

#ifdef ARDUINO_ARCH_ESP8266
        WiFi.hostname(hostName);
#endif

#ifdef ARDUINO_ARCH_ESP32
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        WiFi.setHostname(hostName);
#endif
    }

    int counter = 0;
    // Wait for connection
    while (wifiMulti.run() != WL_CONNECTED)
    {
        if (waitAndReset(counter, blinkInternal, pr, resetIfNoWiFi) == false)
        {
            return false;
        }
    }

    if (blinkInternal == true)
    {
        internalLedOff();
    }

    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

// We are connected
#ifdef DebugMode
    pr.printf("\nSSID: %s\n", WiFi.SSID().c_str());
    pr.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    pr.printf("RSSI: %d dBm\n", WiFi.RSSI());
#endif

#ifdef DebugMode
    DebugMessagePrintf("WiFi initialized in %ld miliseconds.\n", ((unsigned long)(millis() - timeOnInitWiFiBegin)));
#endif

#endif

    return true;
}
