#include "Communication/LoRaDuplex/LoRaDuplex.h"
#include "Display/Display.h"
#include "USV.pb.h"
#include "pb_decode.h"
#include <Arduino.h>

Display display;
LoRaDuplex lora;

String lastMsg;  // To track if the message changed

// Callback function for decoding waypoints
bool waypoint_callback(pb_istream_t* stream, const pb_field_t* field, void** arg)
{
    Waypoint wp = Waypoint_init_zero;
    if (!pb_decode(stream, Waypoint_fields, &wp))
    {
        return false;
    }
    int* idx = (int*)(*arg);
    display.printf("WP%d: %.6f,%.6f", *idx, *idx, wp.lat, wp.lng);
    (*idx)++;
    return true;
}

void setup()
{
    display.setup();
    lora.setup();
    Serial.begin(115200);
    delay(100);
}

void loop()
{
    uint8_t buffer[128];
    size_t msg_size = lora.receivePacket(buffer, sizeof(buffer));
    if (msg_size == 0)
    {
        delay(100);
        return;
    }

    // Check if it's the same message as last time
    static uint8_t lastBuffer[128];
    static size_t lastSize = 0;
    if (msg_size == lastSize && memcmp(buffer, lastBuffer, msg_size) == 0)
    {
        return;
    }
    memcpy(lastBuffer, buffer, msg_size);
    lastSize = msg_size;
    display.clear();

    // Try to decode as StateMessage
    StateMessage stateMsg = StateMessage_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buffer, msg_size);
    if (pb_decode(&stream, StateMessage_fields, &stateMsg))
    {
        if (stateMsg.state == StateMessage_State_MANUAL)
            display.printf("State: MANUAL", 0);
        else if (stateMsg.state == StateMessage_State_AUTOMATIC)
            display.printf("State: AUTOMATIC", 0);
        else
            display.printf("State: UNKNOWN", 0);
        return;
    }

    // Try to decode as WaypointsMessage using callback
    WaypointsMessage waypointsMsg = WaypointsMessage_init_zero;
    stream = pb_istream_from_buffer(buffer, msg_size);
    int idx = 1;
    waypointsMsg.waypoints.funcs.decode = waypoint_callback;
    waypointsMsg.waypoints.arg = &idx;

    if (pb_decode(&stream, WaypointsMessage_fields, &waypointsMsg))
    {
        display.printf("Received %d waypoints", idx, idx - 1);
        return;
    }

    // Unknown message
    display.printf("Unknown msg", 0);
}