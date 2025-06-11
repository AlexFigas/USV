#include "Thruster.h"

Thruster::Thruster(Expander& expander, const ThrusterController& controller)
    : Motor(expander, controller), controller(controller)
{
}

/**
 * For ESCs we need to arm the thrusters before we can use them
 */
void Thruster::begin()
{
    _expander.begin();
    _expander.setDutyCycle(controller.PIN_EN, 0);
    _expander.setPWMFreq(controller.PWM_FREQ);
    setDirection(true);

    // Arm the thruster
    _expander.setPWM(controller.PIN_EN, 1000);
    delay(2000);

    Serial.println("Thruster initialized and armed");
}

void Thruster::front(float speed, float length)
{
    setDirection(true);
    setSpeed(speed);
}

void Thruster::back(float speed, float length)
{
    setDirection(false);
    setSpeed(speed);
}

void Thruster::stop()
{
    _expander.setPWM(controller.PIN_EN, controller.PWM_STOP_PULSE);
}

void Thruster::setDirection(bool clockwise)
{
    if (clockwise)
    {
        _expander.setMinPwmOn(pulseToTicks(controller.PWM_STOP_PULSE));
        _expander.setMaxPwmOn(pulseToTicks(controller.PWM_MIN_PULSE));
    }
    else
    {
        _expander.setMinPwmOn(pulseToTicks(controller.PWM_STOP_PULSE));
        _expander.setMaxPwmOn(pulseToTicks(controller.PWM_MAX_PULSE));
    }
}

uint16_t Thruster::pulseToTicks(int microseconds)
{
    return (uint16_t)((microseconds * 4096L) / (1000000 / controller.PWM_FREQ));
}