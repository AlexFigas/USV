#include "USV.h"

USV::USV()
    : Robot("usv", &movement),
      expander(0x40),
      leftController(14, 1000, 2000, 1500, 50),
      rightController(15, 1000, 2000, 1500, 50),
      thrusters{Thruster(expander, leftController), Thruster(expander, rightController)},
      movement(&thrusters[MOTOR_LEFT], &thrusters[MOTOR_RIGHT], /*track=unused*/ 0.0f),
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
      movement(&thrusters[MOTOR_LEFT], &thrusters[MOTOR_RIGHT], /*track=unused*/ 0.0f),
      SDA_PIN(21),
      SCL_PIN(22)
{
}

void USV::loop()
{
    /** @todo: Add variable for min speed of 25% */
    Serial.println("Front");
    movement.front(25, 0);
    delay(1000);
    Serial.println("Stop");
    movement.stop();
    delay(1000);
    Serial.println("Back");
    movement.back(25, 0);
    delay(1000);
    Serial.println("Stop");
    movement.stop();
    delay(1000);
    Serial.println("Left");
    movement.left(25, 0, 0);
    delay(1000);
    Serial.println("Stop");
    movement.stop();
    delay(1000);
    Serial.println("Right");
    movement.right(25, 0, 0);
    delay(1000);
    Serial.println("Stop");
    movement.stop();
    delay(1000);
}

void USV::begin()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    Robot::begin();
    Serial.println("USV initialized");
}