#ifndef IMUDATA_H
#define IMUDATA_H

#include <ICM_20948.h>

class IMUData
{
  public:
    IMUData();

    struct Acceleration  // m/s^2 (gravitational acceleration)
    {
        float x;
        float y;
        float z;
    } acc;
    struct Gyroscope  // rad/s (angular velocity)
    {
        float x;
        float y;
        float z;
    } gyr;
    struct Magnetometer  // uT (magnetic field strength)
    {
        float x;
        float y;
        float z;
    } mag;
    float temp;

    void update(ICM_20948_I2C* sensor);
    void print();

  private:
    void printFloat(float value, uint8_t leading, uint8_t decimals);
};

#endif  // IMUDATA_H