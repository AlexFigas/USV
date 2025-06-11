#ifndef USV_H
#define USV_H

#include "MovementTwoThrusters.h"
#include <Robot.h>

class USV : public Robot
{
  private:
    Expander expander;
    ThrusterController leftController;
    ThrusterController rightController;
    MovementTwoThrusters movement;

  public:
    USV();

    void loop();
    void begin() override;

  private:
    const uint8_t SDA_PIN;
    const uint8_t SCL_PIN;
};

#endif  // USV_H