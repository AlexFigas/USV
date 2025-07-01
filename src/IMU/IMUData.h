#ifndef IMUDATA_H
#define IMUDATA_H

#include <ICM_20948.h>
#include <MadgwickAHRS.h>

class IMUData
{
  public:
    IMUData();

    struct Acceleration  // g (gravitational force)
    {
        float x;
        float y;
        float z;
    } acc;
    struct Gyroscope  // degrees per second (°/s)
    {
        float x;
        float y;
        float z;
    } gyr;
    struct Magnetometer  // microtesla (µT)
    {
        float x;
        float y;
        float z;
    } mag;
    float temp;
    struct Orientation
    {
        float yaw;
        float pitch;
        float roll;
    } ypr;

    void begin(float sampleFreq);
    void update(ICM_20948_I2C* sensor);
    void print();

  private:
    void printFloat(float value, uint8_t leading, uint8_t decimals);
    Madgwick filter;
    unsigned long lastUpdate;
};

#endif  // IMUDATA_H