#include "Communication/Led/Led.h"
#include "Expander.h"

Expander expander(0x40);   // Create an Expander object with the I2C address 0x40
Led green(expander, 7);    // Create a Led object for pin 7
Led yellow(expander, 11);  // Create a Led object for pin 11

unsigned long startTime = 0;
bool turnedOn1 = false;
bool turnedOff1 = false;
bool startedBlink1 = false;
bool turnedOn2 = false;
bool turnedOff2 = false;
bool startedBlink2 = false;

void setup()
{
    expander.begin();
    green.begin();
    yellow.begin();
    startTime = millis();
}

void loop()
{
    unsigned long now = millis();
    unsigned long elapsed = now - startTime;

    if (!turnedOn1 && elapsed >= 2000)
    {
        green.on();
        turnedOn1 = true;
    }
    if (!turnedOff1 && elapsed >= 4000)
    {
        green.off();
        turnedOff1 = true;
    }
    if (!startedBlink1 && elapsed >= 6000)
    {
        green.blink(2000, 250);  // Blink for 2 seconds, 250ms interval
        startedBlink1 = true;
    }

    if (!turnedOn2 && elapsed >= 1000)
    {
        yellow.on();
        turnedOn2 = true;
    }
    if (!turnedOff2 && elapsed >= 3000)
    {
        yellow.off();
        turnedOff2 = true;
    }
    if (!startedBlink2 && elapsed >= 5000)
    {
        yellow.blink(2000, 250);  // Blink for 2 seconds, 250ms interval
        startedBlink2 = true;
    }

    green.update();
    yellow.update();
}