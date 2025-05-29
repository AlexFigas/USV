#include "Arduino.h"
#include <ESP32Servo.h>
Servo esc;

#define MIN_PULSE_WIDTH_uS 1000
#define MAX_PULSE_WIDTH_uS 2000
#define PWM_PIN 13
void setup()
{
    esc.attach(PWM_PIN, MIN_PULSE_WIDTH_uS, MAX_PULSE_WIDTH_uS);
}

void loop()
{
    esc.write(1852);  // backward
    delay(4000);

    esc.write(1488);  // stop
    delay(4000);

    esc.write(1070);  // forward
    delay(4000);

    esc.write(1488);  // stop
    delay(4000);
}