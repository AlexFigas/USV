#include "WiFiUtils.h"
#include "Display.h"
#include "Settings.h"
#include <Utils.h>
#include <WiFi.h>

static const unsigned long WifiTimeout = 100;

static void waitForWiFi(char* ssidName)
{
    DebugMessagePrintf("WiFiUtils::waitForWiFi(%s)\n", ssidName);

    myPrintf(getDisplay(), "Connecting to %s...\n", ssidName);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(WifiTimeout);
        DebugMessagePrintf(".");
    }
}

void initializeWiFi()
{
    DebugMessagePrintf("WiFiUtils::initializeWiFi()\n");

    PtrSettingsWiFi settingsWiFi = getSettingsWiFi();

    DebugMessagePrintf("Disconnecting from previous mode...\n");
    WiFi.disconnect();

    if (settingsWiFi->apMode)
    {
        DebugMessagePrintf("Configuring WiFi in AP mode...\n");

        WiFi.mode(WIFI_AP);

        IPAddress apAddress, apGateway, apNetworkMask;

        apAddress.fromString(settingsWiFi->address);
        apGateway.fromString(settingsWiFi->gateway);
        apNetworkMask.fromString(settingsWiFi->network);

        DebugMessagePrintf("softAPConfig(%s, %s, %s)...\n",
                           apAddress.toString().c_str(),
                           apGateway.toString().c_str(),
                           apNetworkMask.toString().c_str());
        WiFi.softAPConfig(apAddress, apGateway, apNetworkMask);

        DebugMessagePrintf("softAP(%s)...\n", settingsWiFi->ssid);
        WiFi.softAP(settingsWiFi->ssid /*, settingsWiFi->password*/);

        DebugMessagePrintf("Wait %d ms for AP_START...\n", WifiTimeout);
        delay(WifiTimeout);

        DebugMessagePrintf("Acess Point IP: %s\n", WiFi.softAPIP().toString().c_str());

        myPrintf(getDisplay(), "AP IP: %s.\n", WiFi.softAPIP().toString().c_str());
    }
    else
    {
        DebugMessagePrintf("Configuring WiFi in STA mode...\n");

        WiFi.mode(WIFI_STA);

        WiFi.begin(settingsWiFi->ssid, settingsWiFi->password);

        waitForWiFi(settingsWiFi->ssid);
        // We are connected

        DebugMessagePrintf("\nSSID: %s\n", WiFi.SSID().c_str());
        DebugMessagePrintf("IP: %s\n", WiFi.localIP().toString().c_str());
        DebugMessagePrintf("RSSI: %d dBm\n", WiFi.RSSI());

        myPrintf(getDisplay(), "IP: %s.\n", WiFi.localIP().toString().c_str());
    }
}
