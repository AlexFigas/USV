/**
 * @file MovementTwoThrusters.h - Library for controlling the movement of a USV with two thrusters.
 *
 * @author Alexandre Figueiredo
 */

#ifndef MOVEMENTTWOTHRUSTERS_H
#define MOVEMENTTWOTHRUSTERS_H

#include "Movement.h"
#include "Thruster.h"
#include <cmath>

class MovementTwoThrusters : public Movement
{
  public:
    MovementTwoThrusters(Thruster* left, Thruster* right, float track = 0.0f);

    void curve(float speed, float radius, float angle, bool isLeft = true) override;
    void left(float speed, float radius, float angle) override;
    void right(float speed, float radius, float angle) override;
    void directionCurveCalibration() override;
    void directionLineCalibration() override;

  private:
    Thruster* thrusters[2];
};

#endif
