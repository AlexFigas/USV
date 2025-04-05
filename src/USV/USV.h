#ifndef USV_H
#define USV_H

#include <MovementTwoMotors.h>
#include <Robot.h>

class USV : public Robot
{
  private:
    Expander expander;
    MotorController leftController;
    MotorController rightController;
    MovementTwoMotors movement;

  public:
    USV();

    void loop();
};

#endif  // USV_H