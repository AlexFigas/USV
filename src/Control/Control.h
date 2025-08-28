#ifndef CONTROL_H
#define CONTROL_H

#include "GPS/GPSData.h"
#include "IMU/IMUData.h"
#include "Movement.h"
#include "USV.pb.h"
#include <cmath>
#include <vector>

class Control
{
  public:
    Control(Movement& movement);

    void setWaypoints(const Waypoint waypoints[], size_t size);
    void setState(StateMessage_State state);
    StateMessage_State getState() const;

    // Update navigation data from sensors
    void update(GPSData& gps, const IMUData& imu);

    // Decide which control mode to use
    void control();

  private:
    // Control modes
    void automaticControl();
    void manualControl();

    // Navigation helpers
    double computeDistance(GPSData& gps, const Waypoint& wp);
    double computeBearing(GPSData& gps, const Waypoint& wp);

    void setCourse(int bearingError);

  private:
    Movement& movement;
    std::vector<Waypoint> waypoints;
    double waypointThreshold = 5.0;  // meters
    size_t currentWaypoint = 0;

    // Navigation state
    double lastBearingError = 0.0;
    double distanceToWaypoint = 0.0;
    double bearingToWaypoint = 0.0;
    double heading = 0.0;

    StateMessage_State state = StateMessage_State_MANUAL;
};

#endif  // CONTROL_H
