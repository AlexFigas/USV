#include "Thruster.h"

Thruster::Thruster(Expander expander, const ThrusterController& controller)
    : Motor(expander, controller), controller(controller)
{
}

/**
 * For ESCs we need to arm the thrusters before we can use them
 */
void Thruster::begin()
{
    _expander.setPWM(0, 1000);
    delay(2000);
}

void Thruster::setDirection(bool clockwise)
{
    if (clockwise)
    {
        _expander.setMinPwmOn(pulseToTicks(controller.PWM_STOP_PULSE));
        _expander.setMaxPwmOn(pulseToTicks(controller.PWM_MAX_PULSE));
    }
    else
    {
        _expander.setMinPwmOn(pulseToTicks(controller.PWM_STOP_PULSE));
        _expander.setMaxPwmOn(pulseToTicks(controller.PWM_MIN_PULSE));
    }
}

void Thruster::stop()
{
    _expander.setDutyCycle(controller.PIN_EN, controller.PWM_STOP_PULSE);
}

uint16_t Thruster::pulseToTicks(int microseconds)
{
    return (uint16_t)((microseconds * 4096L) / (1000000 / controller.PWM_FREQ));
}