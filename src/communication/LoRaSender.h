#ifndef LORASENDER_H
#define LORASENDER_H

#include <LoRa.h>
#include <SPI.h>
#include <Arduino.h>

class LoRaSender {
private:
    int SCK;
    int MISO;
    int MOSI;
    int SS;
    int RST;
    int DIO0;
    long BAND; // 433E6 for Asia, 866E6 for Europe, 915E6 for North America

    bool debug = false;

    void printDebug();
    
    public:
    LoRaSender();
    LoRaSender(int SCK, int MISO, int MOSI, int SS, int RST, int DIO0, long BAND);
    
    void setup();
    void sendPacket(String message);
    void enableDebug();
};

#endif // LORASENDER_H