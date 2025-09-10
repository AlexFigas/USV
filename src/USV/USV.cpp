#include "USV.h"

USV::USV(Expander& expander,
         ThrusterController& leftController,
         ThrusterController& rightController,
         Led& automaticLed,
         Led& manualLed)
    : Robot("usv", &movement),
      expander(expander),
      leftController(leftController),
      rightController(rightController),
      automaticLed(automaticLed),
      manualLed(manualLed),
      thrusters{Thruster(expander, leftController), Thruster(expander, rightController)},
      movement(&thrusters[MOTOR_LEFT], &thrusters[MOTOR_RIGHT], /*track=unused*/ 0.0f),
      control(movement, automaticLed, manualLed),
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

    automaticLed.update();
    manualLed.update();
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
