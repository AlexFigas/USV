#include "Communication/Led/Led.h"
#include "Communication/LoRaDuplex/LoRaDuplex.h"
#include "Communication/LoRaProto/LoRaProto.h"
#include "Display/Display.h"
#include "GPS/GPS_BN880.h"
#include "IMU/IMU_ICM_20948.h"
#include "USV/Thruster.h"
#include "USV/USV.h"

GPS_BN880 gps;
IMU_ICM_20948 imu;

Expander expander(0x40);                                       // Default I2C address for the expander
ThrusterController leftController(14, 1000, 2000, 1500, 50);   // Pin, min, max, neutral, fq
ThrusterController rightController(15, 1000, 2000, 1500, 50);  // Pin, min, max, neutral, fq
Led yellow(expander, 7);                                       // Manual
Led green(expander, 11);                                       // Automatic
// Led red(expander, 3); // Error
USV usv = USV(expander, leftController, rightController, green, yellow);

Display display;

unsigned long lastSendTime = 0;        // Store the last time "Hello, World!" was sent
const unsigned long interval = 10000;  // 1 second interval (in milliseconds)

void setup()
{
    delay(2000);

    Serial.begin(115200);

    gps.setup();
    // gps.enableDebug();
    // gps.enableInfo();

    imu.setup();
    // imu.enableDebug();
    // imu.enableInfo();

    usv.getLoRaProto().setup();
    // usv.getLoRaProto().enableDebug();

    display.setup();

    usv.begin();
}

void loop()
{
    gps.loop();
    imu.loop();

    unsigned long currentMillis = millis();

    usv.getLoRaProto().receive();

    GPSData gpsData = gps.getGPSData();
    IMUData imuData = imu.getIMUData();
    usv.loop(gpsData, imuData);

    display.printf(
        "%.6f %.6f", 0, gpsData.location.lat(), gpsData.location.lng() /*, gpsData.altitude.meters() // Altitude */);
    display.printf("Acc: %.2f %.2f %.2f", 1, imuData.acc.x, imuData.acc.y, imuData.acc.z);
    display.printf("Gyr: %.2f %.2f %.2f", 2, imuData.gyr.x, imuData.gyr.y, imuData.gyr.z);
    display.printf("Mag: %.2f %.2f %.2f", 3, imuData.mag.x, imuData.mag.y, imuData.mag.z);
    display.printf("Temp: %.2f", 4, imuData.temp);
    display.printf("LS: %d", 5, usv.getLoRaProto().getLoRaDuplex().getSentPacketCount());
    display.printf("LR: %s", 6, usv.getLoRaProto().getLastReceivedMessage());
}
