#include "GPSData.h"

GPSData::GPSData() : location(), date(), time(), speed(), course(), altitude(), satellites(), hdop() {}

void GPSData::printFloat(float val, bool valid, int len, int prec)
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
}

void GPSData::printInt(unsigned long val, bool valid, int len)
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
}

void GPSData::printDateTime(TinyGPSDate& d, TinyGPSTime& t)
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
}

void GPSData::update(TinyGPSPlus& gps)
{
    if (gps.location.isUpdated() && gps.location.isValid())
    {
        location = gps.location;
    }
    if (gps.date.isUpdated() && gps.date.isValid())
    {
        date = gps.date;
    }
    if (gps.time.isUpdated() && gps.time.isValid())
    {
        time = gps.time;
    }
    if (gps.speed.isUpdated() && gps.speed.isValid())
    {
        speed = gps.speed;
    }
    if (gps.course.isUpdated() && gps.course.isValid())
    {
        course = gps.course;
    }
    if (gps.altitude.isUpdated() && gps.altitude.isValid())
    {
        altitude = gps.altitude;
    }
    if (gps.satellites.isUpdated() && gps.satellites.isValid())
    {
        satellites = gps.satellites;
    }
    if (gps.hdop.isUpdated() && gps.hdop.isValid())
    {
        hdop = gps.hdop;
    }
    charsProcessed = gps.charsProcessed();
    sentencesWithFix = gps.sentencesWithFix();
    failedChecksum = gps.failedChecksum();
}

void GPSData::print()
{
    /// @todo Should we print the header every time?
    /// @todo add headerPrinted to enable debug
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

    printInt(satellites.value(), satellites.isValid(), 5);
    printFloat(hdop.hdop(), hdop.isValid(), 6, 1);
    printFloat(location.lat(), location.isValid(), 11, 6);
    printFloat(location.lng(), location.isValid(), 12, 6);
    printInt(location.age(), location.isValid(), 5);
    printDateTime(date, time);
    printFloat(altitude.meters(), altitude.isValid(), 7, 2);
    printFloat(course.deg(), course.isValid(), 7, 2);
    printFloat(speed.kmph(), speed.isValid(), 6, 2);
    Serial.print(course.isValid() ? TinyGPSPlus::cardinal(course.deg()) : "*** ");
    Serial.print(" ");

    unsigned long distanceKmToRef =
        (unsigned long)TinyGPSPlus::distanceBetween(location.lat(), location.lng(), REF_LAT, REF_LON) / 1000;
    printInt(distanceKmToRef, location.isValid(), 9);

    double courseToRef = TinyGPSPlus::courseTo(location.lat(), location.lng(), REF_LAT, REF_LON);
    printFloat(courseToRef, location.isValid(), 7, 2);
    Serial.print(" ");
    Serial.print(location.isValid() ? TinyGPSPlus::cardinal(courseToRef) : "*** ");
    Serial.print(" ");
    printInt(charsProcessed, true, 6);
    printInt(sentencesWithFix, true, 10);
    printInt(failedChecksum, true, 9);
    Serial.println();
}
