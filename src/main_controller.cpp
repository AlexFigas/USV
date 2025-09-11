// #include "Communication/LoRaDuplex/LoRaDuplex.h"
// #include "Display/Display.h"
// #include "USV.pb.h"
// #include "pb_encode.h"
// #include <Arduino.h>

// Display display;
// LoRaDuplex lora;

// // Callback for encoding waypoints (for nanopb repeated fields)
// bool waypoints_encode_callback(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
// {
//     const Waypoint* waypoints = (const Waypoint*)(*arg);
//     for (int i = 0; i < 3; ++i)
//     {
//         if (!pb_encode_tag_for_field(stream, field))
//             return false;
//         if (!pb_encode_submessage(stream, Waypoint_fields, &waypoints[i]))
//             return false;
//     }
//     return true;
// }

// void send_state_message(StateMessage_State state)
// {
//     StateMessage msg = StateMessage_init_zero;
//     msg.state = state;

//     uint8_t buffer[32];
//     pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
//     if (pb_encode(&stream, StateMessage_fields, &msg))
//     {
//         lora.sendPacket(buffer, stream.bytes_written);

//         display.printf("Sent State: %s", 0, state == StateMessage_State_MANUAL ? "MANUAL" : "AUTOMATIC");
//     }
//     else
//     {
//         display.printf("State encode fail", 0);
//     }
// }

// void send_waypoints_message()
// {
//     WaypointsMessage msg = WaypointsMessage_init_zero;
//     static Waypoint waypoints[3] = {{.lat = 38.473601, .lng = -8.869591},
//                                     {.lat = 38.473388, .lng = -8.869464},
//                                     {.lat = 38.473511, .lng = -8.869186}};
//     msg.waypoints.arg = waypoints;
//     msg.waypoints.funcs.encode = waypoints_encode_callback;

//     uint8_t buffer[128];
//     pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
//     if (pb_encode(&stream, WaypointsMessage_fields, &msg))
//     {
//         lora.sendPacket(buffer, stream.bytes_written);
//         display.printf("Sent 3 waypoints", 2);
//         display.printf("WP1: %.6f,%.6f", 3, waypoints[0].lat, waypoints[0].lng);
//         display.printf("WP2: %.6f,%.6f", 4, waypoints[1].lat, waypoints[1].lng);
//         display.printf("WP3: %.6f,%.6f", 5, waypoints[2].lat, waypoints[2].lng);
//     }
//     else
//     {
//         display.printf("WP encode fail", 2);
//     }
// }

// void send_manual_command(char cmd)
// {
//     StateMessage msg = StateMessage_init_zero;
//     msg.state = StateMessage_State_MANUAL;
//     msg.has_manual = true;

//     switch (cmd)
//     {
//         case 'f':
//             msg.manual.state = StateMessage_Manual_State_FORWARD;
//             display.printf("Manual: FRONT", 2);
//             break;
//         case 'b':
//             msg.manual.state = StateMessage_Manual_State_BACKWARD;
//             display.printf("Manual: BACK", 2);
//             break;
//         case 'l':
//             msg.manual.state = StateMessage_Manual_State_LEFT;
//             display.printf("Manual: LEFT", 2);
//             break;
//         case 'r':
//             msg.manual.state = StateMessage_Manual_State_RIGHT;
//             display.printf("Manual: RIGHT", 2);
//             break;
//         case 's':
//         default:
//             msg.manual.state = StateMessage_Manual_State_STOP;
//             display.printf("Manual: STOP", 2);
//             break;
//     }

//     uint8_t buffer[32];
//     pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
//     if (pb_encode(&stream, StateMessage_fields, &msg))
//     {
//         lora.sendPacket(buffer, stream.bytes_written);
//     }
//     else
//     {
//         display.printf("Manual encode fail", 3);
//     }
// }

// void setup()
// {
//     display.setup();
//     lora.setup();
//     Serial.begin(115200);  // Start serial input
//     display.printf("Type 'manual', 'automatic'", 0);
//     display.printf(", 'waypoints', or f/b/l/r/s", 1);

//     delay(100);
// }

// void loop()
// {
//     static String input = "";

//     while (Serial.available())
//     {
//         char c = Serial.read();
//         if (c == '\n' || c == '\r')
//         {
//             input.trim();
//             if (input.equalsIgnoreCase("manual"))
//             {
//                 send_state_message(StateMessage_State_MANUAL);
//             }
//             else if (input.equalsIgnoreCase("automatic"))
//             {
//                 send_state_message(StateMessage_State_AUTOMATIC);
//             }
//             else if (input.equalsIgnoreCase("waypoints"))
//             {
//                 send_waypoints_message();
//             }
//             else if (input.length() == 1)
//             {
//                 send_manual_command(input.charAt(0));
//             }
//             else if (input.length() > 0)
//             {
//                 display.printf("Unknown: %s", 2, input.c_str());
//             }

//             delay(1000);

//             input = "";
//             display.clear();
//             display.printf("Type 'manual', 'automatic'", 0);
//             display.printf(", 'waypoints', or f/b/l/r/s", 1);
//         }
//         else
//         {
//             input += c;
//             display.printf("Typing: %s", 2, input.c_str());
//         }
//     }
// }
