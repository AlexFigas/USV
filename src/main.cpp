#include "GPS/GPS_BN880.h"

// #define GPS_DEBUG // Uncomment to enable debug messages
GPS_BN880 gps;

void setup()
{
    gps.setup();
}

void loop()
{
    gps.loop();
}
