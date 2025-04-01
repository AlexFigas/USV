#include "LoRaSender.h"

LoRaSender::LoRaSender() : LoRaSender(5, 19, 27, 18, 14, 26, 866E6) {}
LoRaSender::LoRaSender(int SCK, int MISO, int MOSI, int SS, int RST, int DIO0, long BAND)
    : SCK(SCK), MISO(MISO), MOSI(MOSI), SS(SS), RST(RST), DIO0(DIO0), BAND(BAND)
{
}

void LoRaSender::setup()
{
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(BAND))
    {
        Serial.println("LoRa initialization failed!");
        while (true)
            ;
    }
    /// @todo Change to a variable in the constructor
    LoRa.setSpreadingFactor(12); 

    Serial.println("LoRa Initialized Successfully!");
}

bool LoRaSender::sendPacket(String message)
{
    if (debug)
        printDebug();
    
    if (LoRa.beginPacket()) {
        LoRa.print(message);
        LoRa.endPacket(true);
        return true;
    } else {
        return false;
    }
}

void LoRaSender::enableDebug()
{
    debug = true;
    Serial.println(F("LoRa Debug mode enabled"));
}   

void LoRaSender::printDebug()
{
    static bool printed = false;

    if (printed)
        return;

    Serial.println(F("LoRaSender Debug Info:"));
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
