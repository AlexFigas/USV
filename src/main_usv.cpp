#include "Communication/LoRaDuplex/LoRaDuplex.h"
#include "Display/Display.h"
#include "GPS/GPS_BN880.h"
#include "IMU/IMU_ICM_20948.h"
#include "USV/Thruster.h"
#include "USV/USV.h"

GPS_BN880 gps;
IMU_ICM_20948 imu;
LoRaDuplex lora;
USV usv;

Display display;

/// @todo put all strings with an F so they are stored in flash

unsigned long lastSendTime = 0;        // Store the last time "Hello, World!" was sent
const unsigned long interval = 10000;  // 1 second interval (in milliseconds)

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

    display.setup();

    usv.begin();
}

void loop()
{
    gps.loop();
    imu.loop();

    unsigned long currentMillis = millis();  // Get the current time

    /// @todo this is a possibility, since always sending takes too much time most
    /// messages are not received, but we should think about this
    // Only send "Hello, World!" once per interval
    if (currentMillis - lastSendTime >= interval)
    {
        lora.sendPacket("Hello, World!");
        lastSendTime = currentMillis;  // Update the last send time
    }

    lora.receivePacket();

    usv.loop();

    GPSData gpsData = gps.getGPSData();
    IMUData imuData = imu.getIMUData();
    // usv.update(gpsData, imuData);

    display.printf("GPS: %.2f %.2f %.2f", 0, gpsData.location.lat(), gpsData.location.lng(), gpsData.altitude.meters());
    display.printf("Acc: %.2f %.2f %.2f", 1, imuData.acc.x, imuData.acc.y, imuData.acc.z);
    display.printf("Gyr: %.2f %.2f %.2f", 2, imuData.gyr.x, imuData.gyr.y, imuData.gyr.z);
    display.printf("Mag: %.2f %.2f %.2f", 3, imuData.mag.x, imuData.mag.y, imuData.mag.z);
    display.printf("Temp: %.2f", 4, imuData.temp);
    display.printf("LoRaSender: %d", 5, lora.getSentPacketCount());
    display.printf("LoRaReceiver: %s", 6, lora.getLastReceivedMessage());
}
