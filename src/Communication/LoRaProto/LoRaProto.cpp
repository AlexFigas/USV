#include "LoRaProto.h"

LoRaProto::LoRaProto(Control& control, LoRaDuplex& lora) : control(control), lora(lora) {}

void LoRaProto::setup()
{
    lora.setup();
}

void LoRaProto::enableDebug()
{
    lora.enableDebug();
}

void LoRaProto::sendStateMessage(StateMessage_State state)
{
    StateMessage msg = StateMessage_init_zero;
    msg.state = state;

    uint8_t buffer[BUFFER_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (pb_encode(&stream, StateMessage_fields, &msg))
    {
        lora.sendPacket(buffer, stream.bytes_written);
    }
}

bool LoRaProto::waypointsDecodeCallback(pb_istream_t* stream, const pb_field_t* field, void** arg)
{
    WaypointDecodeContext* ctx = static_cast<WaypointDecodeContext*>(*arg);

    Waypoint wp = Waypoint_init_zero;
    if (!pb_decode(stream, Waypoint_fields, &wp))
    {
        return false;
    }
    ctx->idx++;

    ctx->self->waypoints[ctx->idx - 1] = wp;
    return true;
}

bool LoRaProto::waypointsEncodeCallback(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
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

void LoRaProto::sendWaypointsMessage()
{
    WaypointsMessage msg = WaypointsMessage_init_zero;
    static Waypoint waypoints[2] = {{.lat = 38.7169, .lng = -9.1399}, {.lat = 40.4168, .lng = -3.7038}};
    msg.waypoints.arg = waypoints;
    msg.waypoints.funcs.encode = &LoRaProto::waypointsEncodeCallback;

    uint8_t buffer[BUFFER_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (pb_encode(&stream, WaypointsMessage_fields, &msg))
    {
        lora.sendPacket(buffer, stream.bytes_written);
    }
}

void LoRaProto::receive()
{
    uint8_t buffer[BUFFER_SIZE];
    size_t msg_size = lora.receivePacket(buffer, sizeof(buffer));
    if (msg_size == 0)
    {
        return;
    }

    // Check if it's the same message as last time
    static uint8_t lastBuffer[BUFFER_SIZE];
    static size_t lastSize = 0;
    if (msg_size == lastSize && memcmp(buffer, lastBuffer, msg_size) == 0)
    {
        return;
    }
    memcpy(lastBuffer, buffer, msg_size);
    lastSize = msg_size;

    // Try to decode as StateMessage
    StateMessage stateMsg = StateMessage_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buffer, msg_size);
    if (pb_decode(&stream, StateMessage_fields, &stateMsg))
    {
        if (stateMsg.state == StateMessage_State::StateMessage_State_AUTOMATIC)
        {
            lastReceivedMessage = "SM: state=AUTOMATIC";
        }
        else
        {
            lastReceivedMessage = "SM: state=MANUAL";
        }

        control.setState(stateMsg.state);
        return;
    }

    // Try to decode as WaypointsMessage using callback
    WaypointsMessage waypointsMsg = WaypointsMessage_init_zero;
    stream = pb_istream_from_buffer(buffer, msg_size);
    size_t idx = 1;
    WaypointDecodeContext ctx = {this, idx};
    waypointsMsg.waypoints.funcs.decode = &LoRaProto::waypointsDecodeCallback;
    waypointsMsg.waypoints.arg = &ctx;

    if (pb_decode(&stream, WaypointsMessage_fields, &waypointsMsg))
    {
        lastReceivedMessage = "WM: " + String(ctx.idx - 1) + " wps";
        control.setWaypoints(waypoints, ctx.idx - 1);
    }

    // Unknown message
}

LoRaDuplex& LoRaProto::getLoRaDuplex()
{
    return lora;
}

const char* LoRaProto::getLastReceivedMessage()
{
    return lastReceivedMessage.c_str();
}