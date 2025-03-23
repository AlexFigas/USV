#include "WirelessCommunication.h"
#include <Utils.h>
#include <UtilsBoards.h>

WiFiMultiEntry creds[] = {{"Vodafone-A49491-24G", "CG847937n9"}, {"CarEst2", "48038942"}};

const char* ssid = "eduroam";
const char* identity = "cgoncalves@deetc.isel.ipl.pt";
const char* password = "8762f4304f24db5d";

boolean eduroamPresent()
{

    // Disconnect and set mode
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    int n = WiFi.scanNetworks();

    if (n == 0)
    {
        DebugMessagePrintf("No networks found.\n");
        // Don't proceed, loop forever
        for (;;)
        {
            ;
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (WiFi.SSID(i).equals("eduroam"))
        {
            DebugMessagePrintf("eduroam is present.\n");
            return true;
        }
    }

    DebugMessagePrintf("eduroam not present.\n");

    return false;
}

void initializeWiFi()
{

    if (eduroamPresent() == true)
    {
        initWiFi(ssid, identity, password, true);
    }
    else
    {
        initWiFi(creds, 2, true);
    }
}
