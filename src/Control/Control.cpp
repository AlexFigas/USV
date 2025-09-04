#include "Control.h"

constexpr double EARTH_RADIUS_METERS = 6371000.0;
constexpr double MIN_SATELLITES = 4;
constexpr double MAX_HDOP = 2.5;
constexpr double SPEED_THRESHOLD = 1.0;  // km/h
constexpr double SMOOTHING_ALPHA = 0.2;

Control::Control(Movement& movement)
    : movement(movement),
      waypointThreshold(5.0),
      currentWaypoint(0),
      lastBearingError(0.0),
      distanceToWaypoint(0.0),
      bearingToWaypoint(0.0),
      heading(0.0)
{
}

void Control::setWaypoints(const Waypoint waypoints[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        bool exists = false;

        // Check if this waypoint is already in the vector
        for (const auto& wp : this->waypoints)
        {
            if (wp.lat == waypoints[i].lat && wp.lng == waypoints[i].lng)
            {
                exists = true;
                break;
            }
        }

        // Only add it if it doesn't exist
        if (!exists)
        {
            this->waypoints.push_back(waypoints[i]);
        }
    }
}

void Control::setState(StateMessage_State state)
{
    this->state = state;
}

StateMessage_State Control::getState() const
{
    return state;
}

void Control::update(GPSData& gps, const IMUData& imu)
{
    // If all waypoints reached, stop and exit
    if (currentWaypoint >= waypoints.size())
    {
        movement.stop();
        return;
    }

    // Compute navigation data
    distanceToWaypoint = computeDistance(gps, waypoints[currentWaypoint]);
    bearingToWaypoint = computeBearing(gps, waypoints[currentWaypoint]);

    // Advance to next waypoint if close enough
    if (distanceToWaypoint < waypointThreshold)
    {
        // Testing Only
        movement.stop();
        delay(500);
        movement.front(100, 0);
        delay(500);
        movement.stop();
        delay(500);
        movement.back(100, 0);
        delay(500);
        movement.stop();
        delay(500);

        currentWaypoint++;
        return;
    }

    // Prefer GPS heading if moving fast enough
    if (gps.course.isValid() && gps.speed.kmph() >= SPEED_THRESHOLD)
        heading = gps.course.deg();
    else
        heading = imu.ypr.yaw;

    // Compute smoothed bearing error
    double bearingError = bearingToWaypoint - heading;
    while (bearingError > 180.0)
        bearingError -= 360.0;
    while (bearingError < -180.0)
        bearingError += 360.0;
    bearingError = (1 - SMOOTHING_ALPHA) * lastBearingError + SMOOTHING_ALPHA * bearingError;
    lastBearingError = bearingError;
}

void Control::control()
{
    if (state == StateMessage_State_AUTOMATIC)
    {
        automaticControl();
        Serial.println("Automatic control active");
    }
    else if (state == StateMessage_State_MANUAL)
    {
        manualControl();
        Serial.println("Manual control active");
    }
}

void Control::automaticControl()
{
    if (currentWaypoint >= waypoints.size() && waypoints.size() > 0)
    {
        movement.stop();
        return;
    }
    setCourse(lastBearingError);
}

void Control::setCourse(int bearingError)
{
    if (bearingError > 0)
    {
        movement.right(60, 0, bearingError);
        movement.left(40, 0, bearingError);
    }
    else if (bearingError < 0)
    {
        movement.left(60, 0, -bearingError);
        movement.right(40, 0, -bearingError);
    }
    else
    {
        movement.stop();
    }
}

void Control::manualControl()
{
    /// @todo
    // Here you would handle manual control commands
    // For example, based on RC input or joystick>
    movement.stop();
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
    return fmod((bearing + 360.0), 360.0);
}
