#include "MovementTwoThrusters.h"

MovementTwoThrusters::MovementTwoThrusters(Thruster* thrusters, float track) : Movement(thrusters, track)
{
    /** @todo: Find why second is not working */
    _numMotors = 1;
}

void MovementTwoThrusters::curve(float speed, float radius, float angle, bool isLeft)
{
    // Stub implementation
    Serial.printf("MovementTwoThrusters::curve(speed: %.2f, radius: %.2f, angle: %.2f, isLeft: %s)\n",
                  speed,
                  radius,
                  angle,
                  isLeft ? "true" : "false");
}

void MovementTwoThrusters::left(float speed, float radius, float angle)
{
    // Stub implementation
    Serial.printf("MovementTwoThrusters::left(speed: %.2f, radius: %.2f, angle: %.2f)\n", speed, radius, angle);
}

void MovementTwoThrusters::right(float speed, float radius, float angle)
{
    // Stub implementation
    Serial.printf("MovementTwoThrusters::right(speed: %.2f, radius: %.2f, angle: %.2f)\n", speed, radius, angle);
}

void MovementTwoThrusters::directionCurveCalibration()
{
    // Stub implementation
    Serial.println("MovementTwoThrusters::directionCurveCalibration()");
}

void MovementTwoThrusters::directionLineCalibration()
{
    // Stub implementation
    Serial.println("MovementTwoThrusters::directionLineCalibration()");
}
