#ifndef CONTROL_H
#define CONTROL_H

#include "GPS/GPSData.h"
#include "IMU/IMUData.h"
#include "USV/USV.h"
#include <vector>

struct Waypoint
{
    double lat;
    double lng;
};

class Control
{
  public:
    Control(USV& usv);

    void setWaypoints(const std::vector<Waypoint>& waypoints);
    void update(GPSData& gps, const IMUData& imu);

  private:
    USV& usv;
    std::vector<Waypoint> waypoints;
    double waypointThreshold = 5.0;  // meters
    size_t currentWaypoint = 0;
    double lastBearingError = 0.0;

    double computeDistance(GPSData& gps, const Waypoint& wp);
    double computeBearing(GPSData& gps, const Waypoint& wp);
};

#endif  // CONTROL_H