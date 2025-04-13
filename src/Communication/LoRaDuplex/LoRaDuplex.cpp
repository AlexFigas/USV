#include "LoRaDuplex.h"

LoRaDuplex::LoRaDuplex() : LoRaDuplex(5, 19, 27, 18, 14, 26, 866E6) {}

LoRaDuplex::LoRaDuplex(int SCK, int MISO, int MOSI, int SS, int RST, int DIO0, long BAND)
    : SCK(SCK), MISO(MISO), MOSI(MOSI), SS(SS), RST(RST), DIO0(DIO0), BAND(BAND)
{
}

void LoRaDuplex::setup()
{
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(BAND))
    {
        Serial.println("LoRa initialization failed!");
        while (true)
            ;
    }

    LoRa.setSpreadingFactor(12);
    Serial.println("LoRa Initialized Successfully!");
}

void LoRaDuplex::enableDebug()
{
    debug = true;
    Serial.println(F("LoRaDuplex Debug mode enabled"));
}

void LoRaDuplex::printDebug()
{
    static bool printed = false;
    if (printed)
        return;

    Serial.println(F("LoRaDuplex Debug Info:"));
    Serial.print(F("SCK: "));
    Serial.println(SCK);
    Serial.print(F("MISO: "));
    Serial.println(MISO);
    Serial.print(F("MOSI: "));
    Serial.println(MOSI);
    Serial.print(F("SS: "));
    Serial.println(SS);
    Serial.print(F("RST: "));
    Serial.println(RST);
    Serial.print(F("DIO0: "));
    Serial.println(DIO0);
    Serial.print(F("BAND: "));
    Serial.println(BAND);
    printed = true;
}

void LoRaDuplex::receivePacket()
{
    if (debug)
        printDebug();

    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        receivedCount++;

        String temp = "";

        if (debug)
        {
            Serial.print("Received packet (size: ");
            Serial.print(packetSize);
            Serial.print("): ");
        }

        while (LoRa.available())
        {
            char c = (char)LoRa.read();
            temp += c;
            if (debug)
                Serial.print(c);
        }

        if (debug)
            Serial.println();

        if (temp.length() > 0)
            lastReceivedMessage = temp;
    }
}

String LoRaDuplex::getLastReceivedMessage()
{
    return lastReceivedMessage;
}

int LoRaDuplex::getReceivedPacketCount()
{
    return receivedCount;
}

bool LoRaDuplex::sendPacket(const String& message)
{
    if (debug)
        printDebug();

    if (LoRa.beginPacket())
    {
        LoRa.print(message);
        LoRa.endPacket(true);
        sentCount++;
        return true;
    }
    return false;
}

int LoRaDuplex::getSentPacketCount()
{
    return sentCount;
}