#include "MovementTwoThrusters.h"

MovementTwoThrusters::MovementTwoThrusters(Thruster* thrusters, float track) : Movement(thrusters, track, 2) {}

void MovementTwoThrusters::curve(float speed, float radius, float angle, bool isLeft)
{
    // Stub implementation
}

void MovementTwoThrusters::left(float speed, float radius, float angle)
{
    // Stub implementation
}

void MovementTwoThrusters::right(float speed, float radius, float angle)
{
    // Stub implementation
}

void MovementTwoThrusters::directionCurveCalibration()
{
    // Stub implementation
}

void MovementTwoThrusters::directionLineCalibration()
{
    // Stub implementation
}
