#include "Control.h"
#include <cmath>

constexpr double EARTH_RADIUS_METERS = 6371000.0;
constexpr double MIN_SATELLITES = 4;
constexpr double MAX_HDOP = 2.5;
constexpr double SPEED_THRESHOLD = 1.0;  // km/h
constexpr double SMOOTHING_ALPHA = 0.2;

Control::Control(USV& usv) : usv(usv), waypointThreshold(5.0), currentWaypoint(0), lastBearingError(0.0) {}

void Control::setWaypoints(const std::vector<Waypoint>& waypoints)
{
    this->waypoints = waypoints;
    currentWaypoint = 0;
}

void Control::update(GPSData& gps, const IMUData& imu)
{
    if (currentWaypoint >= waypoints.size())
    {
        usv.stop();
        return;
    }

    // // Skip update if GPS quality is poor
    // if (!gps.hdop.isValid() || gps.hdop.hdop() > MAX_HDOP ||
    //     !gps.satellites.isValid() || gps.satellites.value() < MIN_SATELLITES) {
    //     usv.stop();
    //     return;
    // }

    // // Skip control if not moving
    // if (!gps.speed.isValid() || gps.speed.kmph() < SPEED_THRESHOLD) {
    //     usv.stop();
    //     return;
    // }

    double distance = computeDistance(gps, waypoints[currentWaypoint]);
    double targetBearing = computeBearing(gps, waypoints[currentWaypoint]);

    if (distance < waypointThreshold)
    {
        currentWaypoint++;
        return;
    }

    double heading;

    // Prefer IMU yaw, fallback to GPS course if moving fast enough
    if (gps.course.isValid() && gps.speed.kmph() >= SPEED_THRESHOLD)
    {
        heading = gps.course.deg();
    }
    else
    {
        heading = imu.ypr.yaw;
    }

    double bearingError = targetBearing - heading;

    // Normalize error to [-180, 180]
    while (bearingError > 180.0)
        bearingError -= 360.0;
    while (bearingError < -180.0)
        bearingError += 360.0;

    // Smooth bearing error (low-pass filter)
    bearingError = (1 - SMOOTHING_ALPHA) * lastBearingError + SMOOTHING_ALPHA * bearingError;
    lastBearingError = bearingError;

    usv.setCourse(bearingError);
}

double Control::computeDistance(GPSData& gps, const Waypoint& wp)
{
    double lat1 = gps.location.lat() * DEG_TO_RAD;
    double lon1 = gps.location.lng() * DEG_TO_RAD;
    double lat2 = wp.lat * DEG_TO_RAD;
    double lon2 = wp.lng * DEG_TO_RAD;

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = std::pow(std::sin(dlat / 2), 2) + std::cos(lat1) * std::cos(lat2) * std::pow(std::sin(dlon / 2), 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    return EARTH_RADIUS_METERS * c;
}

double Control::computeBearing(GPSData& gps, const Waypoint& wp)
{
    double lat1 = gps.location.lat() * DEG_TO_RAD;
    double lon1 = gps.location.lng() * DEG_TO_RAD;
    double lat2 = wp.lat * DEG_TO_RAD;
    double lon2 = wp.lng * DEG_TO_RAD;

    double dlon = lon2 - lon1;
    double x = std::sin(dlon) * std::cos(lat2);
    double y = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(dlon);

    double bearing = std::atan2(x, y) * RAD_TO_DEG;
    return fmod((bearing + 360.0), 360.0);  // Normalize to [0, 360)
}
