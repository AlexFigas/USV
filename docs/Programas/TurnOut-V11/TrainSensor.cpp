#include "TrainSensor.h"

void TrainSensor::setTrainPresent(uint8_t rawValue)
{
    DebugMessagePrintf("TrainSensor::setTrainPresent(0x%02X)\n", rawValue);

    this->_trainPresent = (bool)(rawValue & this->_mask);
}

TrainSensor::TrainSensor(uint8_t mask, PtrExpanderIO expander)
{
    DebugMessagePrintf("TrainSensor::TrainSensor(0x%02X, PtrExpanderIO)\n", mask);

    this->_trainPresent = false;

    this->_mask = mask;

    expander->addSensor((void*)this);
}

bool TrainSensor::isTrainPresent()
{
    return this->_trainPresent;
}

void TrainSensor::show()
{
    DebugMessagePrintf("{Mask: 0X%02X, Train present: %s}\n", this->_mask, this->_trainPresent ? "true" : "false");
}
