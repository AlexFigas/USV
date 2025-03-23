#include "ExpanderIO.h"
#include "Display.h"
#include "Settings.h"
#include "TrainSensor.h"
#include <Utils.h>

PtrExpanderIO* ExpanderIO::_expanders;
int ExpanderIO::_numberOfExpanders;

void ExpanderIO::pinChanged()
{
    this->_flag = true;
}

ExpanderIO::ExpanderIO(int pinIRQ, byte address) : Expander()
{
    DebugMessagePrintf("ExpanderIO::ExpanderIO(%d, 0x%x)\n", pinIRQ, address);

    this->_board = new PCF8574(address);
    this->_pinIRQ = pinIRQ;

    pinMode(this->_pinIRQ, INPUT_PULLUP);

    this->_flag = false;

    attachInterrupt(digitalPinToInterrupt(this->_pinIRQ),
                    std::bind(&ExpanderIO::pinChanged, this),
                    /*CHANGE*/ /*RISING*/ FALLING);
}

bool ExpanderIO::begin()
{
    DebugMessagePrintf("ExpanderIO::begin()\n");

    if (!this->_initialized)
    {
        DebugMessagePrintf("Initializing expander for the first time...\n");

        this->_initialized = this->_board->begin();
    }

    return this->_initialized;
}

void ExpanderIO::addSensor(void* sensor)
{
    this->_sensors.add(sensor);
}

void ExpanderIO::show()
{
    DebugMessagePrintf("{pinIRQ: %d, Sensor size: %d}\n", this->_pinIRQ, this->_sensors.getSize());
}

void ExpanderIO::loop()
{
    if (this->_flag == true)
    {
        this->_flag = false;

        uint8_t currentValue = ~(this->_board->read8());

        for (int idx = 0; idx < this->_sensors.getSize(); ++idx)
        {
            PtrTrainSensor current = (PtrTrainSensor)this->_sensors.get(idx);
            current->setTrainPresent(currentValue);
        }
    }
}

void ExpanderIO::initExpanders()
{
    DebugMessagePrintf("ExpanderIO::initExpanders()\n");

    PtrSettingsExpandersIO settings = getSettingsExpandersIO();
    int size = settings->expandersIO.size();

    DebugMessagePrintf("Going to process %d IO expanders...\n", size);

    ExpanderIO::_expanders = new PtrExpanderIO[size];
    ExpanderIO::_numberOfExpanders = size;

    clearDisplay();
    myPrintf(getDisplay(), "Creating %d IO Expanders...\n", size);

    for (int index = 0; index < size; ++index)
    {
        PtrSettingsExpanderIO currentSettings = settings->expandersIO.get(index);

        PtrExpanderIO currentExpander = new ExpanderIO(currentSettings->pinIRQ, currentSettings->address);
        ExpanderIO::_expanders[index] = currentExpander;

        if (!currentExpander->begin())
        {
            myPrintf(getDisplay(), "IO Expander #%d not set!\n", index);
        }
        else
        {
            myPrintf(getDisplay(), "IO Expander #%d OK!\n", index);
        }
        delay(getMessageTimeout());
        clearDisplay();
    }
}

PtrExpanderIO ExpanderIO::getExpander(int index)
{
    DebugMessagePrintf("ExpanderIO::getExpander(%d)\n", index);

    return ExpanderIO::_expanders[index];
}

void ExpanderIO::loops()
{
    for (int index = 0; index < ExpanderIO::_numberOfExpanders; ++index)
    {
        ExpanderIO::_expanders[index]->loop();
    }
}