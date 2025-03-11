#ifndef _ServoModelRailroad_h_
#define _ServoModelRailroad_h_

#include "ExpanderPWM.h"
#include "Settings.h"

typedef class ServoModelRailroad* PtrServoModelRailroad;

/**
 * Class representing a Servo used in Model Railroad for controlling for example turnout.
 *
 * Written by Carlos Gonçalves
 *
 */
class ServoModelRailroad {
  private:
    /**
     * Possible states for a Servo
     */
    enum ServoModelRailroadState {
      MinimumPosition,
      MaximumPosition,
      CenterPosition,
      MovingToMax,
      MovingToMin
    };

    /**
     * Default PWM incremement value ( in a 0 -> 4096 resolution)
    */
    const static uint16_t StepMove = 5;

    /**
     * Default values for setting the minimum and maximum servo position ( in a 0 -> 4096 resolution)
     */
    const static uint16_t DefaultMinServo = 250;
    const static uint16_t DefaultMaxServo = 350;
    
    /**
     * Amount of time (in milli seconds) for the servo move from the min/max position the max/min position
     */
    static unsigned long StepMoveTime;

    /**
     * Set of servos. Used in the loops method
     */
    static PtrServoModelRailroad* servos;
    static int numberOfServos;

    /**
     * The settings associated with the underlying servo
     */
    PtrSettingsServo _settings;

    /**
     * The expander used to controll the underlying servo
     */
    PtrExpanderPWM _expander;

    /**
     * The index, within the expander, used to controll the underlying servo
    */
    byte _index;

    /**
     * The minimum and maximum position of the servo
     */
    uint16_t _minPositionValue = DefaultMinServo;
    uint16_t _maxPositionValue = DefaultMaxServo;

    /**
     * The current position of the servo
     */
    uint16_t _currentPositionValue;

    /**
     * Last time when state of this crossing changed
     */
    unsigned long _lastMoveTime;

    /**
     * Current state
     */
    ServoModelRailroadState _state;

    /**
     * Constructor for the ServoModelRailroad class.
     *
     * @param settings    - The settings associated with this tornout
     * @param index       - The index of the servo within the expander
     * @param expander    - The Expander used to control the servos.
     */
    ServoModelRailroad(PtrSettingsServo settings, byte index, PtrExpanderPWM expander);

    /**
     * Servo state machine control
     */
    void loop();
  public:
    const static int CommandMin     = 0;
    const static int CommandCenter  = CommandMin + 1;
    const static int CommandMax     = CommandCenter + 1 ;

    /**
     * Update interval values according to the values presented in the settings attribute
     */
    void updateFromSettings();

    /**
     * Adjust (increment/decrement) the value of the minimum position
     *
     * @param delta - the value to increment/decrement to the current minimum position 
     */
    void adjustMinimum(uint16_t delta);

    /**
     * Set the value of the minimum position
     */
    void setMinimum(uint16_t minimumPosition);
  
    /**
     * Get the value of the minimum position
     */
    uint16_t getMinimum();

    /**
     * Adjust (increment/decrement) the value of the maximum position
     *
     * @param delta - the value to increment/decrement to the current maximum position 
     */
    void adjustMaximum(uint16_t delta);

    /**
     * Set the value of the maximum position
     */
    void setMaximum(uint16_t maximumPosition);

    /**
     * Get the value of the maximum position
     */
    uint16_t getMaximum();

    /**
     * Get the value of the center position
     */
    uint16_t getCenter();

    /**
     * Set the position of the associated servo to the minimum position.
     */
    void gotoToMinimum();

    /**
     * Move the position of the associated servo to the minimum position.
     */
    void moveToMinimum();

    /**
     * Set the position of the associated servo to the maximum position.
     */
    void gotoToMaximum();

    /**
     * Move the position of the associated servo to the maximum position.
     */
    void moveToMaximum();

    /**
     * Set the position of the associated servo to the center position.
     */
    void gotoToCenter();

    /**
     * Auxiliary method to show the object ( equivalent to the toString() method of Java)
    */
    void show();

    /**
     * Initialize all the existing servos, given the number of servos in the configuration file
     */
    static void initServosModelRailroad();

    /**
     * Call the loop for all the existing servos, given the number of servos in the configuration file
     */
    static void loops();

    /**
     * Get a servo given its index,
     * 
     * The first servo (given the number of servos in the configuration file) is zero
     */
    static PtrServoModelRailroad getServo(int index);

    static void setStepMoveTime(unsigned long time);

    static int getStepMoveTime();
};

#endif  // _ServoModelRailroad_h_
