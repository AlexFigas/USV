#include "GPS/GPS_BN880.h"
#include "IMU/IMU_ICM_20948.h"
#include "USV/usv.h"
#include "communication/LoRaSender.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define GPS_I2C_ADDR 0x4D

GPS_BN880 gps;
IMU_ICM_20948 imu;
LoRaSender lora;
USV usv;

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
    // lora.enableDebug();

    usv.begin();
}

void loop()
{
    gps.loop();
    imu.loop();

    if (lora.sendPacket("Hello, World! " + String(counter)))
    {
        counter++;
    }

    usv.loop();
}