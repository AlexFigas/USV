#include "Communication/LoRaDuplex/LoRaDuplex.h"
#include "Display/Display.h"
#include "USV.pb.h"
#include "pb_encode.h"
#include <Arduino.h>

Display display;
LoRaDuplex lora;

// Callback for encoding waypoints (for nanopb repeated fields)
bool waypoints_encode_callback(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
{
    const Waypoint* waypoints = (const Waypoint*)(*arg);
    for (int i = 0; i < 2; ++i)
    {
        if (!pb_encode_tag_for_field(stream, field))
            return false;
        if (!pb_encode_submessage(stream, Waypoint_fields, &waypoints[i]))
            return false;
    }
    return true;
}

void send_state_message(StateMessage_State state)
{
    StateMessage msg = StateMessage_init_zero;
    msg.state = state;

    uint8_t buffer[32];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (pb_encode(&stream, StateMessage_fields, &msg))
    {
        lora.sendPacket(buffer, stream.bytes_written);
        display.printf("Sent State: %s", 0, state == StateMessage_State_MANUAL ? "MANUAL" : "AUTO");
    }
    else
    {
        display.printf("State encode fail", 0);
    }
}

void send_waypoints_message()
{
    WaypointsMessage msg = WaypointsMessage_init_zero;
    static Waypoint waypoints[2] = {{.lat = 38.7169, .lng = -9.1399}, {.lat = 40.4168, .lng = -3.7038}};
    msg.waypoints.arg = waypoints;
    msg.waypoints.funcs.encode = waypoints_encode_callback;

    uint8_t buffer[128];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (pb_encode(&stream, WaypointsMessage_fields, &msg))
    {
        lora.sendPacket(buffer, stream.bytes_written);
        display.printf("Sent 2 waypoints", 1);
        display.printf("WP1: %.6f,%.6f", 2, waypoints[0].lat, waypoints[0].lng);
        display.printf("WP2: %.6f,%.6f", 3, waypoints[1].lat, waypoints[1].lng);
    }
    else
    {
        display.printf("WP encode fail", 1);
    }
}

void setup()
{
    display.setup();
    lora.setup();
    delay(100);
}

void loop()
{
    static bool toggle = false;
    static bool state = false;
    if (toggle)
    {
        if (state)
        {
            send_state_message(StateMessage_State_MANUAL);
            display.printf("State: %s", 0, "MANUAL");
        }
        else
        {
            send_state_message(StateMessage_State_AUTOMATIC);
            display.printf("State: %s", 0, "AUTOMATIC");
        }
        state = !state;
    }
    else
    {
        send_waypoints_message();
    }
    toggle = !toggle;
    delay(5000);
    display.clear();
}