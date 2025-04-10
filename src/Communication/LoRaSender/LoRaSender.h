#ifndef LORASENDER_H
#define LORASENDER_H

#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>

class LoRaSender
{
  private:
    int packetCount = 0;

    int SCK;
    int MISO;
    int MOSI;
    int SS;
    int RST;
    int DIO0;
    long BAND;  // 433E6 for Asia, 866E6 for Europe, 915E6 for North America

    bool debug = false;

    void printDebug();

  public:
    LoRaSender();
    LoRaSender(int SCK, int MISO, int MOSI, int SS, int RST, int DIO0, long BAND);

    void setup();
    bool sendPacket(String message);
    void enableDebug();
    int getPacketCount();
};

#endif  // LORASENDER_H