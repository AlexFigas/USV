#include "IMUData.h"

IMUData::IMUData() : acc{}, gyr{}, mag{}, temp(0) {}

void IMUData::update(ICM_20948_I2C* sensor)
{
    if (sensor->dataReady())
    {
        sensor->getAGMT();
    }

    acc = {sensor->accX(), sensor->accY(), sensor->accZ()};
    gyr = {sensor->gyrX(), sensor->gyrY(), sensor->gyrZ()};
    mag = {sensor->magX(), sensor->magY(), sensor->magZ()};
    temp = sensor->temp();
}

void IMUData::print()
{
    Serial.print("IMU: ");
    Serial.print("Acc: [");
    printFloat(acc.x, 5, 2);
    Serial.print(", ");
    printFloat(acc.y, 5, 2);
    Serial.print(", ");
    printFloat(acc.z, 5, 2);
    Serial.print("], Gyr: [");
    printFloat(gyr.x, 5, 2);
    Serial.print(", ");
    printFloat(gyr.y, 5, 2);
    Serial.print(", ");
    printFloat(gyr.z, 5, 2);
    Serial.print("], Mag: [");
    printFloat(mag.x, 5, 2);
    Serial.print(", ");
    printFloat(mag.y, 5, 2);
    Serial.print(", ");
    printFloat(mag.z, 5, 2);
    Serial.print("], Temp: ");
    printFloat(temp, 5, 2);
    Serial.println();
}

void IMUData::printFloat(float value, uint8_t leading, uint8_t decimals)
{
    Serial.print(value < 0 ? "-" : " ");
    float absValue = abs(value);
    uint32_t tenpow = pow(10, leading - 1);
    while (tenpow > 1 && absValue < tenpow)
    {
        Serial.print("0");
        tenpow /= 10;
    }
    Serial.print(absValue, decimals);
}
