#ifndef IMUDATA_H
#define IMUDATA_H

#include <ICM_20948.h>

class IMUData
{
  public:
    IMUData();

    void update(ICM_20948_I2C* sensor);
    void print();

  private:
    struct Acceleration
    {
        float x;
        float y;
        float z;
    } acc;
    struct Gyroscope
    {
        float x;
        float y;
        float z;
    } gyr;
    struct Magnetometer
    {
        float x;
        float y;
        float z;
    } mag;
    float temp;

    void printFloat(float value, uint8_t leading, uint8_t decimals);
};

#endif  // IMUDATA_H