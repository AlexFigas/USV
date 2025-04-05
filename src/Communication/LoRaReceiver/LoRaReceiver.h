#ifndef LORARECEIVER_H
#define LORARECEIVER_H

#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>

class LoRaReceiver
{
  private:
    int packetCount = 0;
    String lastReceivedMessage = "";  // Store the last received message

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
    LoRaReceiver();
    LoRaReceiver(int SCK, int MISO, int MOSI, int SS, int RST, int DIO0, long BAND);

    void setup();
    void loop();
    void enableDebug();
    int getPacketCount();
    String getLastReceivedMessage();  // Return the last received message
};

#endif  // LORARECEIVER_H
