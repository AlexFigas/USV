#ifndef GPS_BN880_H
#define GPS_BN880_H

#include <Arduino.h>
#include <SC16IS7X0.h>
#include <TinyGPSPlus.h>

class GPS_BN880
{
  private:
    // BN-880 GPS module
    uint32_t GPS_BAUD;
    TinyGPSPlus gps;

    // SC16IS750 I2C UART bridge
    uint32_t CRYSTAL_FREQ;
    uint8_t I2C_ADD;
    int8_t SDA_PIN;
    int8_t SCL_PIN;
    SC16IS7X0 sc16is750;

    void smartDelay(unsigned long milliseconds);
    void printFloat(float value, bool isValid, int totalWidth, int decimalPlaces);
    void printInt(unsigned long value, bool isValid, int totalWidth);
    void printDateTime(TinyGPSDate& date, TinyGPSTime& time);

  public:
    GPS_BN880();
    GPS_BN880(int8_t sdaPin, int8_t sclPin);
    GPS_BN880(int8_t sdaPin, int8_t sclPin, uint8_t i2cAddress);
    GPS_BN880(int8_t sdaPin, int8_t sclPin, uint8_t i2cAddress, uint32_t crystalFreq);
    GPS_BN880(int8_t sdaPin, int8_t sclPin, uint8_t i2cAddress, uint32_t crystalFreq, uint32_t gpsBaud);

    void setup();
    void loop();
    void printData();
};

#endif // GPS_BN880_H