#include "USV.h"

USV::USV()
    : Robot("usv", &movement),
      expander(0x40),
      leftController(14, 1000, 2000, 1500, 50),
      rightController(15, 1000, 2000, 1500, 50),
      thrusters{Thruster(expander, leftController), Thruster(expander, rightController)},
      movement(thrusters, /*track=unused*/ 0.0f),
      SDA_PIN(21),
      SCL_PIN(22)
{
}

USV::USV(Expander& expander, ThrusterController& leftController, ThrusterController& rightController)
    : Robot("usv", &movement),
      expander(expander),
      leftController(leftController),
      rightController(rightController),
      thrusters{Thruster(expander, leftController), Thruster(expander, rightController)},
      movement(&thrusters[0], &thrusters[1], /*track=unused*/ 0.0f),  // test this
      SDA_PIN(21),
      SCL_PIN(22)
{
}

void USV::loop()
{
    /** @todo: Add variable for min speed of 25% */
    movement.front(5, 0);
}

void USV::begin()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    Robot::begin();
    Serial.println("USV initialized");
}