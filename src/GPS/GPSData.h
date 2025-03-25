#ifndef GPSDATA_H
#define GPSDATA_H

#include <TinyGPSPlus.h>

class GPSData
{
  public:
    GPSData();

    void update(TinyGPSPlus& gps);
    void print();

  private:
    TinyGPSLocation location;
    TinyGPSDate date;
    TinyGPSTime time;
    TinyGPSSpeed speed;
    TinyGPSCourse course;
    TinyGPSAltitude altitude;
    TinyGPSInteger satellites;
    TinyGPSHDOP hdop;
    uint32_t charsProcessed;
    uint32_t sentencesWithFix;
    uint32_t failedChecksum;
    bool isUpdated;

    void printFloat(float value, bool isValid, int totalWidth, int decimalPlaces);
    void printInt(unsigned long value, bool isValid, int totalWidth);
    void printDateTime(TinyGPSDate& date, TinyGPSTime& time);
};

#endif  // GPSDATA_H
