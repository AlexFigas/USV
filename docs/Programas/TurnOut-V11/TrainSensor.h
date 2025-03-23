#ifndef _TrainSensor_h_
#define _TrainSensor_h_

#include "ExpanderIO.h";
#include "Utils.h";

typedef class TrainSensor* PtrTrainSensor;

class TrainSensor
{
    friend class ExpanderIO;

  private:
    void setTrainPresent(uint8_t rawValue);

  protected:
    uint8_t _mask;

    bool _trainPresent;

  public:
    TrainSensor(uint8_t mask, PtrExpanderIO expander);

    bool isTrainPresent();

    /**
     * Auxiliary method to show the object ( equivalent to the toString() method of Java)
     */
    void show();
};

#endif  // _TrainSensor_h_