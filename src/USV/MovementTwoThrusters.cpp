#include "MovementTwoThrusters.h"

MovementTwoThrusters::MovementTwoThrusters(Thruster* left, Thruster* right, float track)
    : Movement(reinterpret_cast<Motor**>(thrusters), 2, track)
{
    thrusters[MOTOR_LEFT] = left;
    thrusters[MOTOR_RIGHT] = right;
}

void MovementTwoThrusters::curve(float speed, float radius, float angle, bool isLeft)
{
    // Stub implementation
}

void MovementTwoThrusters::left(float speed, float radius, float angle)
{
    // Stub implementation
    motors[MOTOR_RIGHT]->front(speed, 0);
}

void MovementTwoThrusters::right(float speed, float radius, float angle)
{
    // Stub implementation
    motors[MOTOR_LEFT]->front(speed, 0);
}

void MovementTwoThrusters::directionCurveCalibration()
{
    // Stub implementation
}

void MovementTwoThrusters::directionLineCalibration()
{
    // Stub implementation
}
