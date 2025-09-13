#include "MovementTwoThrusters.h"

MovementTwoThrusters::MovementTwoThrusters(Thruster* left, Thruster* right, float track)
    : Movement(reinterpret_cast<Motor**>(thrusters), 2, track)
{
    thrusters[MOTOR_LEFT] = left;
    thrusters[MOTOR_RIGHT] = right;
}

void MovementTwoThrusters::curve(float speed, float radius, float angle, bool isLeft)
{
    /// @note Not needed. Won't be implemented.
}

void MovementTwoThrusters::left(float speed, float radius, float angle)
{
    thrusters[MOTOR_RIGHT]->front(speed, 0);
    thrusters[MOTOR_LEFT]->front(speed / 3, 0);
}

void MovementTwoThrusters::right(float speed, float radius, float angle)
{
    thrusters[MOTOR_LEFT]->front(speed, 0);
    thrusters[MOTOR_RIGHT]->front(speed / 3, 0);
}

void MovementTwoThrusters::directionCurveCalibration()
{
    /// @note Not needed. Won't be implemented.
}

void MovementTwoThrusters::directionLineCalibration()
{
    /// @note Not needed. Won't be implemented.
}
