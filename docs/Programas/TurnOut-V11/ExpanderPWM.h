#ifndef _ExpanderPWM_h_
#define _ExpanderPWM_h_

#include "Expander.h"

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

typedef class ExpanderPWM* PtrExpanderPWM;

/**
 * Class for controlling a PWM pin expander supported by the PCA9685.
 *
 * Written by Alexandre Figueiredo and Daniela Gonçalves
 *
 * Adpated by Carlos Gonçalves to controll model railroad turnouts using servos (9G)
 */
class ExpanderPWM : public Expander {
  private:
    /**
     * Set of PWM expanders. Used in the loops method
     */
    static PtrExpanderPWM* expanders;
    static int numberOfExpanders;

    /**
     * Default frequency to be used when controlling the servos
     */
    const static int DefaultServoFrequency = 50;

    /**
     * The board that supports the pin expanders
     *
     * Adafruit_PWMServoDriver
     */
    Adafruit_PWMServoDriver* _board;

    /**
     * The frequency used to controlling the servo
     */
    int _frequency;

    /**
     * Constructor for the ExpanderPWM class.
     *
     * @param address   - An optional byte indicating the I2C address of the expander.
     *                    Defaults to 0x40.
     * @param frequency - Anoptional int to specify the servo frequency
     *                    Defaults to DefaultServoFrequency
     */
    ExpanderPWM(byte address = 0x40, int frequency = DefaultServoFrequency);

  public:
    /*
     * Initializes the expander.
     * This method should be called once at the beginning of the program.
     */
    bool begin();

    /**
     * Sets the duration of the ON value.
     *
     * @param channel   - A byte indicating the channel number to set the ON value.
     * @param onValue   - A short int indicating ON value.
     */
    int setOnValue(byte channel, uint16_t onValue);

    /**
     * Auxiliary method to show the object ( equivalent to the toString() method of Java)
    */
    void show();

    /**
     * Initialize all the PWM Expanders, given the number of expanders entries in the configuration file
     */
    static void initExpanders();

    /**
     * Get a specific expander from the set all the existing expanders.
     *
     * The first expander (given the number of expanders in the configuration file) is zero
     *
     * @param index - the index of the expander.
     */
    static PtrExpanderPWM getExpander(int index);
};

#endif  // _ExpanderPWM_h_
