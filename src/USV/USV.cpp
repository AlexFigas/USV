#include "USV.h"

USV::USV()
    : leftController(14, 1000, 2000, 1500, 50),
      rightController(15, 1000, 2000, 1500, 50),
      movement(new Thruster[2]{Thruster(expander, leftController), Thruster(expander, rightController)}),
      Robot("usv", &movement),
      SDA_PIN(21),
      SCL_PIN(22)
{
}

void USV::loop()
{
    _movement->front(50, 100);
}

void USV::begin()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    Robot::begin();
}