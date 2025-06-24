#ifndef USV_H
#define USV_H

#include "MovementTwoThrusters.h"
#include <Robot.h>

class USV : public Robot
{

  public:
    USV();
    USV(Expander& expander, ThrusterController& leftController, ThrusterController& rightController);

    void loop();
    void begin() override;

  private:
    Expander expander;
    ThrusterController leftController;
    ThrusterController rightController;
    Thruster thrusters[2];
    MovementTwoThrusters movement;
    const uint8_t SDA_PIN;
    const uint8_t SCL_PIN;
};

#endif  // USV_H