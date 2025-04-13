#ifndef LORADUPLEX_H
#define LORADUPLEX_H

#include <LoRa.h>
#include <SPI.h>

class LoRaDuplex
{
  public:
    LoRaDuplex();
    LoRaDuplex(int SCK, int MISO, int MOSI, int SS, int RST, int DIO0, long BAND);

    void setup();
    void enableDebug();
    void printDebug();

    // Receiver
    void receivePacket();
    String getLastReceivedMessage();
    int getReceivedPacketCount();

    // Sender
    bool sendPacket(const String& message);
    int getSentPacketCount();

  private:
    int SCK, MISO, MOSI, SS, RST, DIO0;
    long BAND;
    bool debug = false;
    String lastReceivedMessage;
    int receivedCount = 0;
    int sentCount = 0;
};

#endif