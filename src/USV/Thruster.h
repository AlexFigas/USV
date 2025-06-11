#ifndef THRUSTER_H
#define THRUSTER_H

#include "Motor.h"

struct ThrusterController : public MotorController
{
    uint16_t PWM_MIN_PULSE;
    uint16_t PWM_MAX_PULSE;
    uint16_t PWM_STOP_PULSE;
    uint8_t PWM_FREQ;

    ThrusterController(byte pin_en,
                       uint16_t min_pulse = 1000,
                       uint16_t max_pulse = 2000,
                       uint16_t stop_pulse = 1500,
                       uint8_t freq = 50)
        : MotorController{pin_en}
    {
        PIN_EN = pin_en;
        PWM_MIN_PULSE = min_pulse;
        PWM_MAX_PULSE = max_pulse;
        PWM_STOP_PULSE = stop_pulse;
        PWM_FREQ = freq;
    }
};

class Thruster : public Motor
{
  public:
    // Public constants

    // Public variables

    // Public methods
    Thruster(Expander& expander, const ThrusterController& controller);

    void begin() override;

    void setDirection(bool clockwise) override;

    void stop() override;

  private:
    // Private constants

    // Private variables
    ThrusterController controller;

    // Private methods
    uint16_t pulseToTicks(int microseconds);
};

#endif
