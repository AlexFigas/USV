#ifndef _Crossing_h_
#define _Crossing_h_

#include "ServoModelRailroad.h"
#include "Settings.h"
#include "TrainSensor.h"
#include <XT_DAC_Audio.h>

typedef class Crossing* PtrCrossing;

/**
 * Class for controlling a Railroad crossing gate.
 *
 * A railroad crossing play a sound (WAV) and can have servos for controlling the fences
 *
 * The WAV souds are supported using the XTronical DAC Audio library
 *
 * https://www.xtronical.com/introduction-to-dac-audio/
 *
 * Written by Carlos Gonçalves
 *
 */
class Crossing
{
  private:
    /**
     * Possible states for a crossing
     */
    enum StateSound
    {
        NoTrain,
        TrainEntering,
        SoundStart,
        SoundUp,
        SoundPlaying,
        TrainLeaving,
        TrainLeaved,
        SoundDown,
        SoundStop
    };

    /**
     * Default DAC values to use
     */
    const static int DefaultPinDAC = 25;
    const static int DefaultTimerDAC = 0;

    /**
     * Default volume values to use
     */
    const static int DefaultStepVolume = 10;
    const static int DefaultMinVolume = 20;
    const static int DefaultMaxVolume = 70;

    /**
     * Duration (in ms) to increase/decrease the sound volume from the min/max to the max/min values
     */
    const static unsigned long DeltaTime = 250;

    /**
     * Set of crossings. Used in the loops method
     */
    static PtrCrossing* _crossings;
    static int _numberOfCrossings;

    /**
     * Volume settings
     */
    int _stepVolume = DefaultStepVolume;
    int _minVolume = DefaultMinVolume;
    int _maxVolume = DefaultMaxVolume;
    int _volume;

    /**
     * Current state
     */
    StateSound _state;

    /**
     * Last time when state of this crossing changed
     */
    unsigned long _lastTime;

    /**
     * Main player class object
     */
    XT_DAC_Audio_Class* _dacAudio;

    /**
     * An object of type XT_Wav_Class that is used by the dac audio class
     *
     * Passing wav data as parameter
     */
    XT_Wav_Class* _sound;

    /**
     * The data that supports the sound played when the train is passing in the crossing
     */
    unsigned char* _data;

    /**
     * Set of sensors used to detected a incoming train
     */
    PtrTrainSensor* _enter;
    int _enterSize;

    /**
     * Set of sensors used to detected a leaving train
     */
    PtrTrainSensor* _leave;
    int _leaveSize;

    /**
     * Set of actuators used to controll the lights associated width a crossing
     */
    PtrServoModelRailroad* _lights;
    int _lightsSize;

    /**
     * Set of servos used to controll the fences associated width a crossing
     */
    PtrServoModelRailroad* _fences;
    int _fencesSize;

    /**
     * Auxiliary function to initialize a list of input ports (sensors) of type SettingsPortIn
     */
    PtrTrainSensor* initSensors(int& outSize, SimpleLinkedList<PtrSettingsPortIn>& input);

    /**
     * Auxiliary function to initialize a list of output ports (actuators) of type PtrSettingsPortOut
     */
    PtrServoModelRailroad* initSensors(int& outSize, SimpleLinkedList<PtrSettingsPortOut>& input);

    /**
     * Open all the fences associated with the crossing
     */
    void openFences();

    /**
     * Close all the fences associated with the crossing
     */
    void closeFences();

    /**
     * Turn on all the lights associated with the crossing
     */
    void lightsOn();

    /**
     * Turn off all the lights associated with the crossing
     */
    void lightsOff();

    /**
     * Check if a set of sensors are active or not
     */
    bool isSensorActive(PtrTrainSensor* sensors, int size);

    /**
     * Set the volume to a minimum value
     */
    void setMinVolume(int minVolume);

    /**
     * Set the volume to a maximum value
     */
    void setMaxVolume(int maxVolume);

    /**
     * Increase the volume by a step
     */
    void setStepVolume(int stepVolume);

    /**
     * Crossing state machine control
     */
    void loop();

    /**
     * Constructor for the Crossing class.
     *
     * @param settings  - The configurations/settings for the current crossing
     */
    Crossing(PtrSettingsCrossing& settings);

  public:
    const static int CommandOpen = 0;
    const static int CommandClose = CommandOpen + 1;

    /**
     * Close the crosing using the web handler
     */
    void close();

    /**
     * Open the crosing using the web handler
     */
    void open();

    /**
     * Initialize all the Crossings, given the number of crossing entries in the configuration file
     */
    static void initCrossings();

    /**
     * Get a specific crossing from the set all the existing crossings.
     *
     * The first crossing (given the number of crossings in the configuration file) is zero
     *
     * @param index - the index of the crossing.
     */
    static PtrCrossing getCrossing(int index);

    /**
     * Call the loop method on all the existing crossings
     */
    static void loops();
};

#endif  // _Crossing_h_