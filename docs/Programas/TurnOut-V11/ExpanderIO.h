#ifndef _ExpanderIO_h_
#define _ExpanderIO_h_

#include "Expander.h"
#include <FunctionalInterrupt.h>
#include <List.hpp>
#include <PCF8574.h>

typedef class ExpanderIO* PtrExpanderIO;

/**
 * Class representing an Input/Output pin expander supported by the PCF8574.
 */
class ExpanderIO : public Expander
{
  private:
    /**
     * Set of IO expanders. Used in the loops method
     */
    static PtrExpanderIO* _expanders;
    static int _numberOfExpanders;

    /**
     * The board that supports the pin expanders
     *
     * https://github.com/RobTillaart/PCF8574
     */
    PCF8574* _board;

    /**
     * The board uses a pin to notify the micro controller every time that a pin change its value
     */
    int _pinIRQ;

    /**
     * This flag is update by the interrupt routine.
     *
     * If this flag is active it means that at least a pin chnaged it value
     */
    volatile bool _flag;

    /**
     * List of sensors associated width this expander
     */
    List<void*> _sensors;

    /**
     * The interrupt routine
     */
    IRAM_ATTR void pinChanged();

    /**
     * Constructor for the ExpanderIO class.
     *
     * @param address - An optional byte indicating the I2C address of the expander.
     *                  Defaults to 0x20.
     */
    ExpanderIO(int pinIRQ, byte address = 0x20);

    /**
     * Expander state machine control
     */
    void loop();

  public:
    /*
     * Initializes the expander.
     * This method should be called once at the beginning of the program.
     */
    bool begin();

    /**
     * @param sensor  - The sensor to add
     *                  The void* is of type PtrTrainSensor (TrainSensor*)
     */
    void addSensor(void* sensor);

    /**
     * Auxiliary method to show the object ( equivalent to the toString() method of Java)
     */
    void show();

    /**
     * Initialize all the IO Expanders, given the number of expanders entries in the configuration file
     */
    static void initExpanders();

    /**
     * Get a specific expander from the set all the existing expanders.
     *
     * The first expander (given the number of expanders in the configuration file) is zero
     *
     * @param index - the index of the expander.
     */
    static PtrExpanderIO getExpander(int index);

    /**
     * Call the loop method on all the existing expanders
     */
    static void loops();
};

#endif  // _ExpanderIO_h_
