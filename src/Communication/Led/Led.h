#pragma once
#include "Expander.h"
#include <chrono>

class Led
{
  public:
    enum class State
    {
        OFF,
        ON,
        BLINK_ON,
        BLINK_OFF,
        IDLE
    };

    Led(Expander& expander, int pin);

    void begin();
    void on();
    void off();
    void blink(int duration, int interval);  // duration and interval in ms
    void update();                           // Call this periodically

    bool isOn() const;

  private:
    Expander& expander;
    int pin;
    State state;
    State lastState = State::IDLE;
    bool isBlinking;
    unsigned long blinkDuration;
    unsigned long blinkInterval;
    unsigned long blinkStart;
    unsigned long lastToggle;
};