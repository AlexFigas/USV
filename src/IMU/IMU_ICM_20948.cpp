#include "IMU/IMU_ICM_20948.h"

IMU_ICM_20948::IMU_ICM_20948() : IMU_ICM_20948(21, 22) {}

IMU_ICM_20948::IMU_ICM_20948(int8_t sdaPin, int8_t sclPin) : SDA_PIN(sdaPin), SCL_PIN(sclPin) {}

void IMU_ICM_20948::setup()
{
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(400000);

    /// @todo Add a way to change the I2C address (0 means 0x68, 1 means 0x69)
    while (imu.begin(Wire, 0) != ICM_20948_Stat_Ok)
    {
        Serial.print(F("IMU initialization failed: "));
        Serial.println(imu.statusString());
        Serial.println("Retrying...");
    }
}

// void IMU_ICM_20948::setup()
// {
//     Serial.begin(115200);
//     Wire.begin(SDA_PIN, SCL_PIN);
//     Wire.setClock(400000);

//     imu.begin(Wire, 1);
//     Serial.print(F("Initialization returned: "));
//     Serial.println(imu.statusString());

//     if (imu.status != ICM_20948_Stat_Ok)
//     {
//         Serial.println("Initialization failed.");
//     }
// }

void IMU_ICM_20948::printDebug()
{
    static bool printed = false;

    if (printed)
        return;

    Serial.println(F("ICM_20948 Debug Info:"));
    Serial.print(F("SDA_PIN: "));
    Serial.println(SDA_PIN);
    Serial.print(F("SCL_PIN: "));
    Serial.println(SCL_PIN);
    Serial.print(F("Status: "));
    Serial.println(imu.statusString());
    printed = true;
}

void IMU_ICM_20948::enableDebug()
{
    debug = true;
    Serial.println(F("IMU Debug mode enabled"));
}

void IMU_ICM_20948::enableInfo()
{
    info = true;
    Serial.println(F("IMU Info mode enabled"));
}

void IMU_ICM_20948::loop()
{
    if (debug)
        printDebug();

    if (info)
        currentIMUData.print();

    currentIMUData.update(&imu);
}
