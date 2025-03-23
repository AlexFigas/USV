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

void GPS_BN880::smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (sc16is750.available())
            gps.encode(sc16is750.read());
    } while (millis() - start < ms);
}

void GPS_BN880::printFloat(float val, bool valid, int len, int prec)
{
    if (!valid)
    {
        while (len-- > 1)
            Serial.print('*');
        Serial.print(' ');
    }
    else
    {
        Serial.print(val, prec);
        int vi = abs((int)val);
        int flen = prec + (val < 0.0 ? 2 : 1);
        flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
        for (int i = flen; i < len; ++i)
            Serial.print(' ');
    }
    smartDelay(0);
}

void GPS_BN880::printInt(unsigned long val, bool valid, int len)
{
    char sz[32] = "*****************";
    if (valid)
        sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i)
        sz[i] = ' ';
    if (len > 0)
        sz[len - 1] = ' ';
    Serial.print(sz);
    smartDelay(0);
}

void GPS_BN880::printDateTime(TinyGPSDate& d, TinyGPSTime& t)
{
    if (!d.isValid())
        Serial.print(F("********** "));
    else
    {
        char sz[32];
        sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
        Serial.print(sz);
    }
    if (!t.isValid())
        Serial.print(F("******** "));
    else
    {
        char sz[32];
        sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
        Serial.print(sz);
    }
    printInt(d.age(), d.isValid(), 5);
    smartDelay(0);
}

void GPS_BN880::printData()
{
    static bool headerPrinted = false;

    if (!headerPrinted)
    {
        Serial.println(
            F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance "
              "Course  "
              "Card  Chars Sentences Checksum"));
        Serial.println(
            F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to "
              "Reference "
              "----  RX    RX        Fail"));
        Serial.println(
            F("--------------------------------------------------------------------------------------------------------"
              "---"
              "-"));
        headerPrinted = true;
    }

    static const double REF_LAT = 38.7169, REF_LON = -9.1399;  // Lisbon, Portugal

    printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
    printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printInt(gps.location.age(), gps.location.isValid(), 5);
    printDateTime(gps.date, gps.time);
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
    printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
    printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
    Serial.print(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ");
    Serial.print(" ");

    unsigned long distanceKmToRef =
        (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), REF_LAT, REF_LON) / 1000;
    printInt(distanceKmToRef, gps.location.isValid(), 9);

    double courseToRef = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), REF_LAT, REF_LON);
    printFloat(courseToRef, gps.location.isValid(), 7, 2);
    Serial.print(" ");
    Serial.print(gps.location.isValid() ? TinyGPSPlus::cardinal(courseToRef) : "*** ");
    Serial.print(" ");
    printInt(gps.charsProcessed(), true, 6);
    printInt(gps.sentencesWithFix(), true, 10);
    printInt(gps.failedChecksum(), true, 9);
    Serial.println();
}

void GPS_BN880::setup()
{
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    delay(100);

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

void GPS_BN880::loop()
{
#ifdef GPS_DEBUG
    Serial.print(F("GPS loop: "));
    Serial.print(millis());
    Serial.print(F(" GPS_BAUD: "));
    Serial.print(GPS_BAUD);
    Serial.print(F(" I2C_ADD: "));
    Serial.print(I2C_ADD);
    Serial.print(F(" CRYSTAL_FREQ: "));
    Serial.print(CRYSTAL_FREQ);
    Serial.print(F(" SDA_PIN: "));
    Serial.print(SDA_PIN);
    Serial.print(F(" SCL_PIN: "));
    Serial.print(SCL_PIN);
    Serial.println();
#endif
    /// @todo Maybe move printData() to debug
    printData();
    smartDelay(1000);

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println(F("No GPS data received: check wiring"));
    }
}
