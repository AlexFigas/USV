#include "GPS/GPS_BN880.h"
#include "IMU/IMU_ICM_20948.h"
#include "communication/LoRaSender.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define GPS_I2C_ADDR 0x4D

GPS_BN880 gps(SDA_PIN, SCL_PIN, GPS_I2C_ADDR);
IMU_ICM_20948 imu(SDA_PIN, SCL_PIN); 
LoRaSender lora;

/// @todo put all strings with an F so they are stored in flash

int counter = 0;

void setup()
{
    gps.setup();
    // gps.enableDebug();
    // gps.enableInfo();

    imu.setup();
    // imu.enableDebug();
    // imu.enableInfo();

    lora.setup();
    lora.enableDebug();
}

void loop()
{
    gps.loop();
    imu.loop();
    lora.sendPacket("Hello, World! " + String(counter++));
}
