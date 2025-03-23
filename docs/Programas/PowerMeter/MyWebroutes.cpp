#include "MyWebroutes.h"
#include "MyPowerSocket.h"
#include "MyTime.h"
#include <Arduino_JSON.h>
#include <UtilsBoards.h>

PtrPZEM _pzem;

String processorSocket(const String& var)
{
    if (var == "PowerSocketState")
    {
        return statusPowerSocket();
    }

    return String();
}

String processorSensor(const String& var)
{
    if (var == "VOLTAGE")
    {
        return String(_pzem->voltage());
    }

    if (var == "CURRENT")
    {
        return String(_pzem->current());
    }

    if (var == "POWER")
    {
        return String(_pzem->power());
    }

    if (var == "ENERGY")
    {
        return String(_pzem->energy());
    }

    if (var == "FREQUENCY")
    {
        return String(_pzem->frequency());
    }

    if (var == "POWER_FACTOR")
    {
        return String(_pzem->powerFactor());
    }

    if (var == "SERVER_TIME")
    {
        return getCurrentDateAndTime('t');
    }

    return String();
}

void webRoutesSetup(AsyncWebServer& server, PtrPZEM pzem)
{

    _pzem = pzem;

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

    // Route for index page
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Route for site icon
    server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/favicon.png", "image/png");
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    // Route to load status.js file
    server.on("/status.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/status.js", "text/javascript");
    });

    // Route to set socket On
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest* request) {
        onPowerSocket();
        request->send(SPIFFS, "/socket.html", String(), false, processorSocket);
    });

    // Route to set socket Off
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest* request) {
        offPowerSocket();
        request->send(SPIFFS, "/socket.html", String(), false, processorSocket);
    });

    // Route to socket file
    server.on("/socket.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/socket.html", String(), false, processorSocket);
    });

    // Route to status.html file
    server.on("/status.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/status.html", String(), false, processorSensor);
    });

    // Route to get status ( sensors and time)
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest* request) {
        JSONVar doc;

        doc["voltage"] = String(_pzem->voltage());
        doc["current"] = String(_pzem->current());
        doc["power"] = String(_pzem->power());
        doc["energy"] = String(_pzem->energy());
        doc["frequency"] = String(_pzem->frequency());
        doc["powerFactor"] = String(_pzem->powerFactor());
        doc["time"] = getCurrentDateAndTime('t');

        String docAsString = String(JSON.stringify(doc));
        request->send(200, "application/json", docAsString);
    });
}
