#ifndef USV_H
#define USV_H

#include "Control/Control.h"
#include "MovementTwoThrusters.h"
#include <Communication/Led/Led.h>
#include <Communication/LoRaDuplex/LoRaDuplex.h>
#include <Communication/LoRaProto/LoRaProto.h>
#include <Robot.h>
#include <USV.pb.h>

class USV : public Robot
{
  public:
    USV(Expander& expander,
        ThrusterController& leftController,
        ThrusterController& rightController,
        Led& automaticLed,
        Led& manualLed);

    void loop(GPSData& gpsData, IMUData& imuData);
    void begin() override;
    void stop();
    void setCourse(double bearingError);
    LoRaProto& getLoRaProto();

  private:
    Expander& expander;
    ThrusterController& leftController;
    ThrusterController& rightController;
    Led& automaticLed;
    Led& manualLed;
    Thruster thrusters[2];
    MovementTwoThrusters movement;
    Control control;
    LoRaDuplex loraDuplex;
    LoRaProto lora;
    const uint8_t SDA_PIN;
    const uint8_t SCL_PIN;
};

#endif  // USV_H
