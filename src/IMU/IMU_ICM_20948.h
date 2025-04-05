#ifndef IMU_ICM_20948_H
#define IMU_ICM_20948_H

#include "IMUData.h"
#include <ICM_20948.h>

class IMU_ICM_20948
{
  private:
    ICM_20948_I2C imu;
    IMUData currentIMUData;

    int8_t SDA_PIN;
    int8_t SCL_PIN;

    bool debug = false;
    bool info = false;

    void printDebug();

  public:
    IMU_ICM_20948();
    IMU_ICM_20948(int8_t sdaPin, int8_t sclPin);

    void setup();
    void loop();
    void enableDebug();
    void enableInfo();
    IMUData getIMUData();
};

#endif  // IMU_ICM_20948_H