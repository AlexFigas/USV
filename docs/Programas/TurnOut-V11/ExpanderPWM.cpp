#include "ExpanderPWM.h"
#include "Display.h"
#include "Settings.h"
#include <Utils.h>

PtrExpanderPWM* ExpanderPWM::expanders;
int ExpanderPWM::numberOfExpanders;

ExpanderPWM::ExpanderPWM(byte address, int frequency) : Expander()
{
    DebugMessagePrintf("ExpanderPWM::ExpanderPWM(0x%x)\n", address);

    this->_board = new Adafruit_PWMServoDriver(address);
    this->_frequency = frequency;
}

bool ExpanderPWM::begin()
{
    DebugMessagePrintf("ExpanderPWM::begin()\n");

    if (!this->_initialized)
    {
        DebugMessagePrintf("Initializing expander for the first time...\n");

        this->_initialized = this->_board->begin();
        if (this->_initialized)
        {
            DebugMessagePrintf("Setting expander frequency to %d Hz...\n", this->_frequency);
            _board->setPWMFreq(this->_frequency);
        }
    }

    return this->_initialized;
}

int ExpanderPWM::setOnValue(byte channel, uint16_t onValue)
{
    // DebugMessagePrintf( "ExpanderPWM::setOnValue(%d, %d)\n", channel, onValue );

    this->_board->setPWM(channel, 0, onValue);

    return onValue;
}

void ExpanderPWM::show()
{
    DebugMessagePrintf("{Frequency: %d}\n", this->_frequency);
}

void ExpanderPWM::initExpanders()
{
    DebugMessagePrintf("ExpanderPWM::initExpanders()\n");

    PtrSettingsExpandersPWM settings = getSettingsExpandersPWM();
    int size = settings->expandersPWM.size();

    DebugMessagePrintf("Going to process %d PWM expanders...\n", size);

    ExpanderPWM::expanders = new PtrExpanderPWM[size];
    ExpanderPWM::numberOfExpanders = size;

    clearDisplay();
    myPrintf(getDisplay(), "Creating %d IO Expanders...\n", size);

    for (int idx = 0; idx < size; ++idx)
    {
        PtrSettingsExpanderPWM currentSettings = settings->expandersPWM.get(idx);

        PtrExpanderPWM currentExpander = new ExpanderPWM(currentSettings->address);

        ExpanderPWM::expanders[idx] = currentExpander;

        if (!currentExpander->begin())
        {
            myPrintf(getDisplay(), "PWM Expander #%d not set!\n", idx);
        }
        else
        {
            myPrintf(getDisplay(), "PWM Expander #%d OK!\n", idx);
        }
        delay(getMessageTimeout());
        clearDisplay();
    }
}

PtrExpanderPWM ExpanderPWM::getExpander(int index)
{
    DebugMessagePrintf("ExpanderPWM::getExpander(%d)\n", index);

    return ExpanderPWM::expanders[index];
}
