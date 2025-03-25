#ifndef USV_H
#define USV_H

#include <Robot.h>
#include <MovementTwoMotors.h>

class USV : public Robot {
  private:
    Expander expander;
    MotorController leftController;
    MotorController rightController;
    MovementTwoMotors movement;

  public:
    USV();
    
    void loop();
};

#endif // USV_H