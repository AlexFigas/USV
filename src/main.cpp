#include "GPS/GPS_BN880.h"
#include "IMU/IMU_ICM_20948.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define GPS_I2C_ADDR 0x42

GPS_BN880 gps(SDA_PIN, SCL_PIN);
IMU_ICM_20948 imu(SDA_PIN, SCL_PIN);

void setup()
{
    gps.setup();
    // gps.enableDebug();
    // gps.enableInfo();

    imu.setup();
    imu.enableDebug();
    imu.enableInfo();
}

void loop()
{
    gps.loop();
    imu.loop();
}
