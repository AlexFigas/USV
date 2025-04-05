#include "LoRaReceiver.h"

LoRaReceiver::LoRaReceiver() : LoRaReceiver(5, 19, 27, 18, 14, 26, 866E6) {}

LoRaReceiver::LoRaReceiver(int SCK, int MISO, int MOSI, int SS, int RST, int DIO0, long BAND)
    : SCK(SCK), MISO(MISO), MOSI(MOSI), SS(SS), RST(RST), DIO0(DIO0), BAND(BAND)
{
}

void LoRaReceiver::setup()
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

void LoRaReceiver::loop()
{
    // Only run the debug logic if debugging is enabled
    if (debug)
        printDebug();

    // Check if there is an incoming LoRa packet
    int packetSize = LoRa.parsePacket();

    if (packetSize)  // A packet was received
    {
        packetCount++;

        String temp = "";  // Temporary string to store the received message

        if (debug)
        {
            Serial.print("Received packet (size: ");
            Serial.print(packetSize);
            Serial.print("): ");
        }

        // Read all available bytes in the packet
        while (LoRa.available())
        {
            char c = (char)LoRa.read();
            temp += c;  // Append character to the message string
            if (debug)
                Serial.print(c);  // Print each character for debugging
        }

        if (debug)
            Serial.println();

        if (temp.length() > 0)
            lastReceivedMessage = temp;  // Store the last received message
        // Optionally, process the received message (e.g., parse it, handle commands, etc.)
    }
}

int LoRaReceiver::getPacketCount()
{
    return packetCount;
}

String LoRaReceiver::getLastReceivedMessage()
{
    return lastReceivedMessage;  // Return the last received message
}

void LoRaReceiver::enableDebug()
{
    debug = true;
    Serial.println(F("LoRaReceiver Debug mode enabled"));
}

void LoRaReceiver::printDebug()
{
    static bool printed = false;

    if (printed)
        return;

    Serial.println(F("LoRaReceiver Debug Info:"));
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
