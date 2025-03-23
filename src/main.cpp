#include "GPS/GPS_BN880.h"

GPS_BN880 gps;

void setup()
{
    gps.setup();
    gps.enableDebug();
    gps.enableInfo();
}

void loop()
{
    gps.loop();
}
