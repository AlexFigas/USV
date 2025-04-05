#include "Communication/LoRaSender/LoRaSender.h"
#include "Display/Display.h"
#include "GPS/GPS_BN880.h"
#include "IMU/IMU_ICM_20948.h"
#include "USV/USV.h"

GPS_BN880 gps;
IMU_ICM_20948 imu;
LoRaSender lora;
USV usv;
Display display;

/// @todo put all strings with an F so they are stored in flash

void setup()
{
    gps.setup();
    // gps.enableDebug();
    // gps.enableInfo();

    imu.setup();
    // imu.enableDebug();
    // imu.enableInfo();

    lora.setup();
    // lora.enableDebug();

    usv.begin();

    display.setup();
}

void loop()
{
    gps.loop();
    imu.loop();

    lora.sendPacket("Hello, World!");

    usv.loop();

    GPSData gpsData = gps.getGPSData();
    IMUData imuData = imu.getIMUData();
    // usv.update(gpsData, imuData);

    display.printf("gps: %.2f %.2f %.2f", 0, gpsData.location.lat(), gpsData.location.lng(), gpsData.altitude.meters());
    display.printf("acc: %.2f %.2f %.2f", 1, imuData.acc.x, imuData.acc.y, imuData.acc.z);
    display.printf("gyr: %.2f %.2f %.2f", 2, imuData.gyr.x, imuData.gyr.y, imuData.gyr.z);
    display.printf("mag: %.2f %.2f %.2f", 3, imuData.mag.x, imuData.mag.y, imuData.mag.z);
    display.printf("temp: %.2f", 4, imuData.temp);
    display.printf("lora: %d", 5, lora.getPacketCount());
}
