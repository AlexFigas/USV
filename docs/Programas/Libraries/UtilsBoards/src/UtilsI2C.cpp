#include <Arduino.h>
#include <Utils.h>
#include <UtilsI2C.h>
#include <Wire.h>

void scanBus(TwoWire& bus)
{
    scanBus(bus, (Print*)(&Serial));
}

void scanBus(TwoWire& bus, Print& pr, char* busName, bool showInSerial)
{
    scanBus(bus, &pr, busName, showInSerial);
}

void scanBus(TwoWire& bus, Print* pr, char* busName, bool showInSerial)
{

    if (pr == NULL)
    {
        return;
    }

    int nDevices;

    pr->printf("I2C(%s) scan...\n", busName != NULL ? busName : "Wire");
    if (showInSerial)
        Serial.printf("I2C(%s) scan...\n", busName != NULL ? busName : "Wire");

    nDevices = 0;
    for (byte address = 1, error; address < 127; ++address)
    {
        bus.beginTransmission(address);
        error = bus.endTransmission();

        switch (error)
        {
            case 0:
                ++nDevices;
                pr->printf("I2C 0x%2X\n", address);
                if (showInSerial)
                    Serial.printf("I2C 0x%2X\n", address);
                break;

            case 4:
                pr->printf("Error 0x%2X\n", address);
                if (showInSerial)
                    Serial.printf("Error 0x%2X\n", address);
                break;
        }
    }

    if (nDevices == 0)
    {
        pr->println("No I2C devs.");
        if (showInSerial)
            Serial.println("No I2C devs.");
    }

    pr->println("I2C scan done.");
    if (showInSerial)
        Serial.println("I2C scan done.");
}