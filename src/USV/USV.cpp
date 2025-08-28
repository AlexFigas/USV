#include "USV.h"

USV::USV()
    : Robot("usv", &movement),
      expander(0x40),
      leftController(14, 1000, 2000, 1500, 50),
      rightController(15, 1000, 2000, 1500, 50),
      thrusters{Thruster(expander, leftController), Thruster(expander, rightController)},
      movement(&thrusters[MOTOR_LEFT], &thrusters[MOTOR_RIGHT], /*track=unused*/ 0.0f),
      control(movement),
      loraDuplex(),
      lora(control, loraDuplex),
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
      control(movement),
      loraDuplex(),
      lora(control, loraDuplex),
      SDA_PIN(21),
      SCL_PIN(22)
{
}

void USV::loop(GPSData& gpsData, IMUData& imuData)
{
    control.update(gpsData, imuData);
    control.control();
}

void USV::begin()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    Robot::begin();
    Serial.println("USV initialized");
}

void USV::stop()
{
    movement.stop();
}

LoRaProto& USV::getLoRaProto()
{
    return lora;
}
