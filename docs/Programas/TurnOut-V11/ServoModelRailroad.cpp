#include "ServoModelRailroad.h"
#include <Utils.h>

PtrServoModelRailroad* ServoModelRailroad::servos;
int ServoModelRailroad::numberOfServos;

unsigned long ServoModelRailroad::StepMoveTime;

ServoModelRailroad::ServoModelRailroad(PtrSettingsServo settings, byte index, PtrExpanderPWM expander)
{
    DebugMessagePrintf("ServoModelRailroad::ServoModelRailroad(PtrExpanderPWM, %d, PtrSettingsServo)\n", (int)index);

    this->_expander = expander;
    this->_index = index;

    if (expander == NULL)
    {
        return;
    }
    else
    {
        this->_expander->begin();
    }

    this->_settings = settings;

    this->updateFromSettings();

    this->_currentPositionValue = this->_minPositionValue;
    this->_state = MinimumPosition;

    this->gotoToMinimum();
}

void ServoModelRailroad::updateFromSettings()
{
    DebugMessagePrintf("ServoModelRailroad::updateFromSettings()\n");

    if (this->_settings != NULL)
    {
        this->setMinimum(this->_settings->minPosition.value);
        this->setMaximum(this->_settings->maxPosition.value);
    }
    else
    {
        this->setMinimum(this->DefaultMinServo);
        this->setMaximum(this->DefaultMaxServo);
    }
}

void ServoModelRailroad::adjustMinimum(uint16_t delta)
{
    DebugMessagePrintf("ServoModelRailroad::adjustMinimum(%d)\n", delta);

    if (delta > 0)
    {
        this->_minPositionValue += delta;
    }
    else
    {
        this->_minPositionValue -= delta;
    }
}

void ServoModelRailroad::setMinimum(uint16_t minimumPosition)
{
    DebugMessagePrintf("ServoModelRailroad::setMinimum(%d)\n", minimumPosition);

    this->_minPositionValue = minimumPosition;
}

uint16_t ServoModelRailroad::getMinimum()
{
    return this->_minPositionValue;
}

void ServoModelRailroad::adjustMaximum(uint16_t delta)
{
    DebugMessagePrintf("ServoModelRailroad::adjustMaximum(%d)\n", delta);

    if (delta > 0)
    {
        this->_maxPositionValue += delta;
    }
    else
    {
        this->_maxPositionValue -= delta;
    }
}

void ServoModelRailroad::setMaximum(uint16_t maximumPosition)
{
    DebugMessagePrintf("ServoModelRailroad::setMaximum(%d)\n", maximumPosition);

    this->_maxPositionValue = maximumPosition;
}

uint16_t ServoModelRailroad::getMaximum()
{
    return this->_maxPositionValue;
}

uint16_t ServoModelRailroad::getCenter()
{
    return this->_minPositionValue + (this->_maxPositionValue - this->_minPositionValue) / 2;
}

void ServoModelRailroad::gotoToMinimum()
{
    DebugMessagePrintf("ServoModelRailroad::gotoToMinimum()\n");

    this->_expander->setOnValue(_index, this->_minPositionValue);
    this->_state = MinimumPosition;
}

void ServoModelRailroad::moveToMinimum()
{
    DebugMessagePrintf("ServoModelRailroad::moveToMinimum()\n");

    if (this->_state == MinimumPosition)
    {
        // Allready in the minimum position
        return;
    }

    this->_lastMoveTime = millis();
    this->_currentPositionValue = this->_maxPositionValue;

    this->_state = MovingToMin;
}

void ServoModelRailroad::gotoToMaximum()
{
    DebugMessagePrintf("ServoModelRailroad::gotoToMaximum()\n");

    this->_expander->setOnValue(_index, this->_maxPositionValue);
    this->_state = MaximumPosition;
}

void ServoModelRailroad::moveToMaximum()
{
    DebugMessagePrintf("ServoModelRailroad::moveToMaximum()\n");

    if (this->_state == MaximumPosition)
    {
        // Allready in the maximum position
        return;
    }

    this->_lastMoveTime = millis();
    this->_currentPositionValue = this->_minPositionValue;

    this->_state = MovingToMax;
}

void ServoModelRailroad::gotoToCenter()
{
    DebugMessagePrintf("ServoModelRailroad::gotoToCenter()\n");

    this->_expander->setOnValue(_index, this->getCenter());
}

void ServoModelRailroad::loop()
{
    unsigned long currentMillis = millis();

    switch (this->_state)
    {
        case MinimumPosition:
            break;

        case MaximumPosition:
            break;

        case CenterPosition:
            break;

        case MovingToMax:
            if ((currentMillis - this->_lastMoveTime) > ServoModelRailroad::StepMoveTime)
            {
                this->_lastMoveTime = currentMillis;

                this->_currentPositionValue += ServoModelRailroad::StepMove;

                if (this->_currentPositionValue > this->getMaximum())
                {
                    this->_currentPositionValue = this->getMaximum();
                    this->_state = MaximumPosition;
                }

                this->_expander->setOnValue(_index, this->_currentPositionValue);
            }
            break;

        case MovingToMin:
            if ((currentMillis - this->_lastMoveTime) > ServoModelRailroad::StepMoveTime)
            {
                this->_lastMoveTime = currentMillis;

                this->_currentPositionValue -= ServoModelRailroad::StepMove;

                if (this->_currentPositionValue < this->getMinimum())
                {
                    this->_currentPositionValue = this->getMinimum();
                    this->_state = MinimumPosition;
                }

                this->_expander->setOnValue(this->_index, this->_currentPositionValue);
            }

            break;
    }
}

void ServoModelRailroad::show()
{
    DebugMessagePrintf(
        "{Index: %d, MinPos: %d, Pos: %d, MaxPos: %d, <Name: %s, Description: %s, MinPos: <%d, %d, %d>, MaxPos: <%d, "
        "%d, %d>>}\n",
        this->_index,
        this->_minPositionValue,
        this->_currentPositionValue,
        this->_maxPositionValue,
        this->_settings->servoName,
        this->_settings->servoDesc,
        this->_settings->minPosition.min,
        this->_settings->minPosition.value,
        this->_settings->minPosition.max,
        this->_settings->maxPosition.min,
        this->_settings->maxPosition.value,
        this->_settings->maxPosition.max);
}

void ServoModelRailroad::initServosModelRailroad()
{
    DebugMessagePrintf("ServoModelRailroad::initServosModelRailroad(PtrExpanderPWM)\n");

    ServoModelRailroad::setStepMoveTime((unsigned long)getSettingsGlobalMotion()->value);

    PtrSettingsServos settings = getSettingsServos();
    int size = settings->servos.size();

    DebugMessagePrintf("Going to process %d servos...\n", size);

    ServoModelRailroad::servos = new PtrServoModelRailroad[size];
    ServoModelRailroad::numberOfServos = size;

    for (int idx = 0; idx < settings->servos.size(); ++idx)
    {
        PtrSettingsServo currentSettings = settings->servos.get(idx);

        PtrExpanderPWM expander = ExpanderPWM::getExpander(currentSettings->expander);
        int index = currentSettings->index;

        ServoModelRailroad::servos[idx] = new ServoModelRailroad(currentSettings, index, expander);
    }
}

void ServoModelRailroad::loops()
{
    for (int idx = 0; idx < ServoModelRailroad::numberOfServos; ++idx)
    {
        ServoModelRailroad::servos[idx]->loop();
    }
}

PtrServoModelRailroad ServoModelRailroad::getServo(int index)
{
    DebugMessagePrintf("ServoModelRailroad::getServo(%d)\n", index);

    return ServoModelRailroad::servos[index];
}

void ServoModelRailroad::setStepMoveTime(unsigned long time)
{
    DebugMessagePrintf("ServoModelRailroad::setStepMoveTime(%d)\n", time);

    ServoModelRailroad::StepMoveTime = time;
}

int ServoModelRailroad::getStepMoveTime()
{
    DebugMessagePrintf("ServoModelRailroad::getStepMoveTime()\n");

    return ServoModelRailroad::StepMoveTime;
}
