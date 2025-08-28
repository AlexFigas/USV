#ifndef LORAPROTO_H
#define LORAPROTO_H

#include "Communication/LoRaDuplex/LoRaDuplex.h"
#include "USV.pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include <Arduino.h>
#include <Control/Control.h>

class LoRaProto
{
  public:
    LoRaProto(Control& control, LoRaDuplex& lora);

    void setup();
    void enableDebug();

    void sendStateMessage(StateMessage_State state);
    void sendWaypointsMessage();

    void receive();

    LoRaDuplex& getLoRaDuplex();
    const char* getLastReceivedMessage();

  private:
    struct WaypointDecodeContext
    {
        LoRaProto* self;
        size_t idx;
    };

    Control& control;
    LoRaDuplex& lora;
    static bool waypointsEncodeCallback(pb_ostream_t* stream, const pb_field_t* field, void* const* arg);
    static bool waypointsDecodeCallback(pb_istream_t* stream, const pb_field_t* field, void** arg);

    Waypoint waypoints[6];

    static const uint8_t BUFFER_SIZE = 128;

    String lastReceivedMessage;
};

#endif