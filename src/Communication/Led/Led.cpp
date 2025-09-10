#include "Led.h"

Led::Led(Expander& expander, int pin) : expander(expander), pin(pin), isBlinking(false), state(State::OFF) {}

void Led::begin()
{
    expander.begin();
    off();
}

void Led::on()
{
    expander.setPWM(pin, 4095);
    state = State::ON;
    isBlinking = false;

    Serial.println("LED ON");
}

void Led::off()
{
    expander.setPWM(pin, 0);
    state = State::OFF;
    isBlinking = false;

    Serial.println("LED OFF");
}

void Led::blink(int duration, int interval)
{
    blinkDuration = duration;
    blinkInterval = interval;
    blinkStart = millis();
    lastToggle = blinkStart;
    lastState = state;
    state = State::BLINK_ON;
    isBlinking = true;
    expander.setPWM(pin, 4095);
}

bool Led::isOn() const
{
    return state == State::ON || state == State::BLINK_ON;
}

void Led::update()
{
    if (!isBlinking)
        return;

    unsigned long now = millis();

    if ((now - blinkStart) >= blinkDuration)
    {
        isBlinking = false;
        if (lastState == State::ON)
        {
            on();
        }
        else
        {
            off();
        }
        Serial.println("Blinking stopped");
        return;
    }

    if (state == State::BLINK_ON && (now - lastToggle) >= blinkInterval)
    {
        expander.setPWM(pin, 0);
        state = State::BLINK_OFF;
        lastToggle = now;
        Serial.println("Blinking On to Off");
    }
    else if (state == State::BLINK_OFF && (now - lastToggle) >= blinkInterval)
    {
        expander.setPWM(pin, 4095);
        state = State::BLINK_ON;
        lastToggle = now;
        Serial.println("Blinking Off to On");
    }
}