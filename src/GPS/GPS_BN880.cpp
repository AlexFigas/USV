#include "GPS_BN880.h"

GPS_BN880::GPS_BN880() : GPS_BN880(21, 22, 0x4D, 14745600, 9600) {}

GPS_BN880::GPS_BN880(int8_t sdaPin, int8_t sclPin) : GPS_BN880(sdaPin, sclPin, 0x4D, 14745600, 9600) {}

GPS_BN880::GPS_BN880(int8_t sdaPin, int8_t sclPin, uint8_t i2cAddress)
    : GPS_BN880(sdaPin, sclPin, i2cAddress, 14745600, 9600)
{
}

GPS_BN880::GPS_BN880(int8_t sdaPin, int8_t sclPin, uint8_t i2cAddress, uint32_t crystalFreq)
    : GPS_BN880(sdaPin, sclPin, i2cAddress, crystalFreq, 9600)
{
}

GPS_BN880::GPS_BN880(int8_t sdaPin, int8_t sclPin, uint8_t i2cAddress, uint32_t crystalFreq, uint32_t gpsBaud)
    : SDA_PIN(sdaPin),
      SCL_PIN(sclPin),
      I2C_ADD(i2cAddress),
      CRYSTAL_FREQ(crystalFreq),
      GPS_BAUD(gpsBaud),
      sc16is750(CRYSTAL_FREQ)
{
}

void GPS_BN880::read()
{
    while (sc16is750.available())
    {
        gps.encode(sc16is750.read());
    }
}

void GPS_BN880::setup()
{
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    if (sc16is750.begin_I2C(I2C_ADD))
    {
        Serial.println("SC16IS750 I2C initialized");
    }
    else
    {
        Serial.println("Failed to initialize SC16IS750");
        while (true)
            ;
    }

    sc16is750.begin_UART(GPS_BAUD);
    Serial.println("GPS UART initialized");
}

void GPS_BN880::printDebug()
{
    static bool printed = false;

    if (printed)
        return;

    printed = true;
    Serial.println(F("GPS_BN880 Debug Info:"));
    Serial.print(F("SDA_PIN: "));
    Serial.println(SDA_PIN);
    Serial.print(F("SCL_PIN: "));
    Serial.println(SCL_PIN);
    Serial.print(F("I2C_ADD: "));
    Serial.println(I2C_ADD, HEX);
    Serial.print(F("CRYSTAL_FREQ: "));
    Serial.println(CRYSTAL_FREQ);
    Serial.print(F("GPS_BAUD: "));
    Serial.println(GPS_BAUD);
}

void GPS_BN880::enableDebug()
{
    debug = true;
    Serial.println(F("GPS Debug mode enabled"));
}

void GPS_BN880::enableInfo()
{
    info = true;
    Serial.println(F("GPS Info mode enabled"));
}

void GPS_BN880::loop()
{
    if (debug)
        printDebug();

    if (info)
        currentGPSData.print();

    read();
    currentGPSData.update(gps);

    if (millis() > GPS_TIMEOUT_TIME_THRESHOLD && gps.charsProcessed() < GPS_TIMEOUT_CHAR_COUNT)
    {
        Serial.println(F("No GPS data received: check wiring"));
    }
}

GPSData GPS_BN880::getGPSData()
{
    return currentGPSData;
}
