#include "usv.h"

USV::USV()
    : leftController{.PIN_EN = 3, .PIN_IN1 = 4, .PIN_IN2 = 5, .interrupt = Interrupt{}, .wheelRadius = 3.3f},
      rightController{.PIN_EN = 0, .PIN_IN1 = 1, .PIN_IN2 = 2, .interrupt = Interrupt{}, .wheelRadius = 3.3f},
      movement(new Motor[2]{Motor(expander, leftController), Motor(expander, rightController)}, 13.0f),
      Robot("usv", &movement)
{
}

void USV::loop()
{
    _movement->front(50, 100);
}