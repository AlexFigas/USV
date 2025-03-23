#include "WebServerUtils.h"
#include "Crossing.h"
#include "ServoModelRailroad.h"
#include "Settings.h"
#include <SPIFFS.h>
#include <Utils.h>

static const int WebMessagesTimeout = 2;

static String processorSettings(const String& var)
{
    DebugMessagePrintf("WebServerUtils::processorSettings(%s)\n", var.c_str());

    PtrSettingsWiFi settingsWiFi = getSettingsWiFi();

    if (var == "WIFI_SSID")
    {
        return String(settingsWiFi->ssid);
    }

    if (var == "WIFI_PASSWORD")
    {
        return String(settingsWiFi->password);
    }

    if (var == "WIFI_APMODE")
    {
        return settingsWiFi->apMode ? String("checked") : String("");
    }

    if (var == "WIFI_IPADDRESS")
    {
        return String(settingsWiFi->address);
    }

    if (var == "WIFI_GATEWAY")
    {
        return String(settingsWiFi->gateway);
    }

    if (var == "WIFI_NETMASK")
    {
        return String(settingsWiFi->network);
    }

    PtrSettingsRange settingsGloMot = getSettingsGlobalMotion();

    if (var == "GLO_MOT_MIN")
    {
        return String(settingsGloMot->min);
    }

    if (var == "GLO_MOT_MAX")
    {
        return String(settingsGloMot->max);
    }

    if (var == "GLO_MOT_VALUE")
    {
        return String(settingsGloMot->value);
    }

    return String();
}

static boolean parseField(AsyncWebServerRequest* request, const char* fieldName, char*& fieldValue)
{
    DebugMessagePrintf("WebServerUtils::parseField(AsyncWebServerRequest*, %s, String&)\n", fieldName);

    if (request->hasParam(fieldName, true))
    {
        String aux = request->getParam(fieldName, true)->value();
        aux.trim();

        DebugMessagePrintf("<%s> is present with value: <%s>\n", fieldName, aux.c_str());

        if (aux.length() > 0)
        {
            DebugMessagePrintf("<%s> is a valid field.\n", fieldName);

            fieldValue = new char[aux.length() + 1];
            strcpy(fieldValue, aux.c_str());
            return true;
        }
        else
        {
            DebugMessagePrintf("<%s> is an invalid field.\n", fieldName);
            return false;
        }
    }

    DebugMessagePrintf("<%s> is missing: \n", fieldName);

    return false;
}

static boolean parseAddress(AsyncWebServerRequest* request, const char* fieldName, char*& addressValue)
{
    DebugMessagePrintf("WebServerUtils::parseAddress(AsyncWebServerRequest*, %s, String&)\n", fieldName);

    if (request->hasParam(fieldName, true))
    {
        String aux = request->getParam(fieldName, true)->value();
        aux.trim();

        DebugMessagePrintf("<%s> is present with value: <%s>\n", fieldName, aux.c_str());

        IPAddress auxIP;
        if (auxIP.fromString(aux))
        {
            DebugMessagePrintf("<%s> is a valid IP address.\n", fieldName);

            addressValue = new char[aux.length() + 1];
            strcpy(addressValue, aux.c_str());

            return true;
        }

        DebugMessagePrintf("<%s> is a invalid IP address!\n", fieldName);
        return false;
    }

    DebugMessagePrintf("<%s> is missing: \n", fieldName);

    return false;
}

void setupWebRoutes(AsyncWebServer& server)
{
    DebugMessagePrintf("WebServerUtils::setupWebRoutes(AsyncWebServer)\n");

    // Resource not found
    server.onNotFound([](AsyncWebServerRequest* request) {
        String message;
        message = "Resource not found!";
        message += "<br>\nURL: ";
        message += request->url();
        message += "<br>\nMethod: ";
        message += request->methodToString();

        AsyncWebServerResponse* response = request->beginResponse(404, "text/html", message);

        response->addHeader("Server", "ESP Async Web Server");
        request->send(response);
    });

    // Route for root web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) { request->send(SPIFFS, "/index.html", "text/html"); });

    // Route to load bootstrap.min.css file
    server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/bootstrap.min.css", "text/css");
    });

    // Route to control.html file
    server.on("/control.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/control.html", String(), false, processorSettings);
    });

    // Route for site icon
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/favicon.ico", "image/ico");
    });

    // Route to load font-awesome.css file
    server.on("/font-awesome.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/font-awesome.css", "text/css");
    });

    // Route to load fontawesome-webfont.eot file
    server.on("/fontawesome-webfont.eot", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/fontawesome-webfont.eot", "application/octet-stream");
    });

    // Route to load fontawesome-webfont.woff file
    server.on("/fontawesome-webfont.woff", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/fontawesome-webfont.woff", "application/octet-stream");
    });

    // Route to load fontawesome-webfont.woff2 file
    server.on("/fontawesome-webfont.woff2", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/fontawesome-webfont.woff2", "application/octet-stream");
    });

    // Route for index.css page
    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/index.css", "text/css");
    });

    // Route for index page
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Route to load railroad.css file
    server.on("/railroad.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/railroad.css", "text/css");
    });

    // Route to load railroad.js file
    server.on("/railroad.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/railroad.js", "text/javascript");
    });

    // Route to handle the update of WiFi settings (using form)
    server.on("/updateWiFi", HTTP_POST, [](AsyncWebServerRequest* request) {
        DebugMessagePrintf("/updateWiFi called using HTTP POST\n");

        SettingsWiFi settings;
        char* msg;

        settings.apMode = request->hasParam("settingsWF_AccessPoint", true);

        if (settings.apMode == true)
        {
            // AP mode. Going to collect: SSID, IP address, Gateway address and Network mask

            if (parseField(request, "settingsWF_SSID", settings.ssid) == true &&
                parseAddress(request, "settingsWF_Address", settings.address) == true &&
                parseAddress(request, "settingsWF_Gateway", settings.gateway) == true &&
                parseAddress(request, "settingsWF_NetworkMask", settings.network) == true)
            {
                msg = "WiFi Access Point settings are valid.";
            }
            else
            {
                request->send(200, "text/html", "Invalid AP parameters!");
                return;
            }
        }
        else
        {
            if (parseField(request, "settingsWF_SSID", settings.ssid) == true &&
                parseField(request, "settingsWF_Password", settings.password) == true)
            {
                msg = "WiFi Station settings are valid.";
            }
            else
            {
                request->send(200, "text/html", "Invalid Station parameters!");
                return;
            }
        }

        int rebootTimeout = 1000;

        char* userMessage = format("%s\nRebooting in %d ms.", msg, rebootTimeout);

        writeSettings(&settings);

        request->send(200, "text/plain", userMessage);

        delete userMessage;

        delay(rebootTimeout);

        ESP.restart();
    });

    // Route to handle the update of Global Motion settings (using form)
    server.on("/updateGlobalMotion", HTTP_POST, [](AsyncWebServerRequest* request) {
        DebugMessagePrintf("/updateGlobalMotion called using HTTP POST\n");

        char* aux;
        int motionValue;

        if (parseField(request, "motionTime", aux) == true)
        {
            motionValue = (String(aux)).toInt();
        }
        delete aux;

        PtrSettingsRange settings = getSettingsGlobalMotion();
        if ((motionValue > settings->min) && (motionValue < settings->max))
        {
            settings->value = motionValue;

            writeSettings(settings);

            AsyncResponseStream* response = request->beginResponseStream("text/html");
            response->addHeader("Server", "ESP Async Web Server");
            char* msg1 =
                format("\n\t\t<meta http-equiv=\"refresh\" content=\"%d; URL=/control.html\" />\n", WebMessagesTimeout);
            char* msg2 =
                format("\n\t</head>\n\t<body>Configurations updated. Redirecting in %d seconds.\n\t</body>\n</html>",
                       WebMessagesTimeout);
            response->print("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Configurations update</title>");
            response->print(msg1);
            response->print(msg2);
            request->send(response);

            delete msg1;
            delete msg2;
        }
        else
        {
            request->send(200, "text/plain", "Invalid value.");
        }
    });

    // Route to handle the update of a single Servos settings (using form)
    server.on("/updateServo", HTTP_POST, [](AsyncWebServerRequest* request) {
        DebugMessagePrintf("/updateServo called using HTTP POST\n");

        char* aux;
        int servoID;

        if (parseField(request, "servoID", aux) == false)
        {
            request->send(200, "text/plain", "Invalid value for servoID.");
        }
        servoID = (String(aux)).toInt();
        delete aux;

        PtrSettingsServos settings = getSettingsServos();
        if (servoID < 0 || (servoID + 1) > settings->servos.size())
        {
            request->send(200, "text/plain", "Out of range servoID.");
        }

        String nameValue1 = String("servo_min#");
        nameValue1.concat(servoID);

        String nameValue2 = String("servo_max#");
        nameValue2.concat(servoID);

        char* value1Aux;
        char* value2Aux;

        if (parseField(request, nameValue1.c_str(), value1Aux) == false ||
            parseField(request, nameValue2.c_str(), value2Aux) == false)
        {
            request->send(200, "text/plain", "Invalid value (min and/or max).");
        }

        int value1 = (String(value1Aux)).toInt();
        int value2 = (String(value2Aux)).toInt();
        delete value1Aux;
        delete value2Aux;

        DebugMessagePrintf("Value1=%d, Value2=%d\n", value1, value2);

        if ((value1 >= settings->servos.get(servoID)->minPosition.min) &&
            (value1 <= settings->servos.get(servoID)->minPosition.max) &&
            (value2 >= settings->servos.get(servoID)->maxPosition.min) &&
            (value2 <= settings->servos.get(servoID)->maxPosition.max))
        {

            settings->servos.get(servoID)->minPosition.value = value1;
            settings->servos.get(servoID)->maxPosition.value = value2;

            writeSettings(settings);

            ServoModelRailroad::getServo(servoID)->updateFromSettings();

            AsyncResponseStream* response = request->beginResponseStream("text/html");
            response->addHeader("Server", "ESP Async Web Server");
            response->print("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Configurations update</title>");

            char* msg1 =
                format("\n\t\t<meta http-equiv=\"refresh\" content=\"%d; URL=/control.html\" />\n", WebMessagesTimeout);
            char* msg2 =
                format("\n\t</head>\n\t<body>Configurations updated. Redirecting in %d seconds.\n\t</body>\n</html>",
                       WebMessagesTimeout);
            response->print("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Configurations update</title>");
            response->print(msg1);
            response->print(msg2);
            request->send(response);

            delete msg1;
            delete msg2;
        }
        else
        {
            request->send(200, "text/plain", "Invalid value.");
        }
    });

    // Route to handle the movement of a single Servo (using AJAX)
    server.on("/moveServo", HTTP_POST, [](AsyncWebServerRequest* request) {
        DebugMessagePrintf("/moveServo called using HTTP POST\n");

        char* aux;
        int servoID, servoCommand, servoMovementType;

        if (parseField(request, "servoID", aux) == false)
        {
            request->send(200, "text/plain", "Invalid value for servoID.");
        }
        servoID = (String(aux)).toInt();
        delete aux;

        PtrSettingsServos settings = getSettingsServos();
        if (servoID < 0 || (servoID + 1) > settings->servos.size())
        {
            request->send(200, "text/plain", "Out of range servoID.");
        }

        if (parseField(request, "command", aux) == false)
        {
            request->send(200, "text/plain", "Invalid value for servoCommand.");
        }
        servoCommand = (String(aux)).toInt();
        delete aux;

        if (servoCommand < ServoModelRailroad::CommandMin || servoCommand > ServoModelRailroad::CommandMax)
        {
            request->send(200, "text/plain", "Out of range servoCommand.");
        }

        if (parseField(request, "movType", aux) == false)
        {
            request->send(200, "text/plain", "Invalid value for movType.");
        }
        servoMovementType = (String(aux)).toInt();
        delete aux;

        PtrServoModelRailroad servo = ServoModelRailroad::getServo(servoID);

        switch (servoCommand)
        {
            case ServoModelRailroad::CommandMin:
                servoMovementType == 0 ? servo->gotoToMinimum() : servo->moveToMinimum();
                break;

            case ServoModelRailroad::CommandCenter:
                servo->gotoToCenter();
                break;

            case ServoModelRailroad::CommandMax:
                servoMovementType == 0 ? servo->gotoToMaximum() : servo->moveToMaximum();
                break;
        }

        char* userMessage = format("Command on servo %d started.", servoID);
        request->send(200, "text/plain", userMessage);

        delete userMessage;
    });

    // Route to handle getting all the Servos settings (using AJAX)
    server.on("/getServos", HTTP_GET, [](AsyncWebServerRequest* request) {
        DebugMessagePrintf("/getServos called using HTTP GET\n");

        request->send(200, "application/json", getSettingsServosAsJSON());
    });

    // Route to handle the update of a single Crossing settings (using form)
    server.on("/updateCrossing", HTTP_POST, [](AsyncWebServerRequest* request) {
        DebugMessagePrintf("/updateCrossing called using HTTP POST\n");

        char* aux;
        int crossingID;

        if (parseField(request, "crossingID", aux) == false)
        {
            request->send(200, "text/plain", "Invalid value for crossingID.");
        }
        crossingID = (String(aux)).toInt();
        delete aux;

        PtrSettingsCrossings settings = getSettingsCrossings();
        if (crossingID < 0 || (crossingID + 1) > settings->crossings.size())
        {
            request->send(200, "text/plain", "Out of range crossingID.");
        }

        String nameValue1 = String("crossing_min#");
        nameValue1.concat(crossingID);

        String nameValue2 = String("crossing_max#");
        nameValue2.concat(crossingID);

        char* value1Aux;
        char* value2Aux;

        if (parseField(request, nameValue1.c_str(), value1Aux) == false ||
            parseField(request, nameValue2.c_str(), value2Aux) == false)
        {
            request->send(200, "text/plain", "Invalid value (min and/or max).");
        }

        int value1 = (String(value1Aux)).toInt();
        int value2 = (String(value2Aux)).toInt();
        delete value1Aux;
        delete value2Aux;

        DebugMessagePrintf("Value1=%d, Value2=%d\n", value1, value2);

        if ((value1 >= settings->crossings.get(crossingID)->minVolume.min) &&
            (value1 <= settings->crossings.get(crossingID)->minVolume.max) &&
            (value2 >= settings->crossings.get(crossingID)->maxVolume.min) &&
            (value2 <= settings->crossings.get(crossingID)->maxVolume.max))
        {

            settings->crossings.get(crossingID)->minVolume.value = value1;
            settings->crossings.get(crossingID)->maxVolume.value = value2;

            writeSettings(settings);

            AsyncResponseStream* response = request->beginResponseStream("text/html");
            response->addHeader("Server", "ESP Async Web Server");
            response->print("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Configurations update</title>");

            char* msg1 =
                format("\n\t\t<meta http-equiv=\"refresh\" content=\"%d; URL=/control.html\" />\n", WebMessagesTimeout);
            char* msg2 =
                format("\n\t</head>\n\t<body>Configurations updated. Redirecting in %d seconds.\n\t</body>\n</html>",
                       WebMessagesTimeout);
            response->print("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Configurations update</title>");
            response->print(msg1);
            response->print(msg2);
            request->send(response);

            delete msg1;
            delete msg2;
        }
        else
        {
            request->send(200, "text/plain", "Invalid value.");
        }
    });

    // Route to handle the movement of a single Crossing (using AJAX)
    server.on("/moveCrossing", HTTP_POST, [](AsyncWebServerRequest* request) {
        DebugMessagePrintf("/moveCrossing called using HTTP POST\n");

        char* aux;
        int crossingID, crossingCommand;

        if (parseField(request, "crossingID", aux) == false)
        {
            request->send(200, "text/plain", "Invalid value for crossingID.");
        }
        crossingID = (String(aux)).toInt();
        delete aux;

        PtrSettingsCrossings settings = getSettingsCrossings();
        if (crossingID < 0 || (crossingID + 1) > settings->crossings.size())
        {
            request->send(200, "text/plain", "Out of range crossingID.");
        }

        if (parseField(request, "command", aux) == false)
        {
            request->send(200, "text/plain", "Invalid value for crossingCommand.");
        }
        crossingCommand = (String(aux)).toInt();
        delete aux;

        if (crossingCommand < Crossing::CommandOpen || crossingCommand > Crossing::CommandClose)
        {
            request->send(200, "text/plain", "Out of range crossingCommand.");
        }

        PtrCrossing crossing = Crossing::getCrossing(crossingID);

        switch (crossingCommand)
        {
            case Crossing::CommandOpen:
                crossing->open();
                break;

            case Crossing::CommandClose:
                crossing->close();
                break;
        }

        char* userMessage = format("Command on crossing %d started.", crossingID);
        request->send(200, "text/plain", userMessage);

        delete userMessage;
    });

    // Route to handle getting all the Crossings settings (using AJAX)
    server.on("/getCrossings", HTTP_GET, [](AsyncWebServerRequest* request) {
        DebugMessagePrintf("/getCrossings called using HTTP GET\n");

        request->send(200, "application/json", getSettingsCrossingsAsJSON());
    });
}
