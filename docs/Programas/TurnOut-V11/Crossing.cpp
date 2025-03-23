#include "Crossing.h"
#include "FileSystem.h"
#include <Utils.h>

PtrCrossing* Crossing::_crossings;
int Crossing::_numberOfCrossings;

PtrTrainSensor* Crossing::initSensors(int& outSize, SimpleLinkedList<PtrSettingsPortIn>& input)
{
    outSize = input.size();

    PtrTrainSensor* result = new PtrTrainSensor[outSize];

    for (int idx = 0; idx < outSize; ++idx)
    {
        PtrSettingsPortIn currentSettings = input.get(idx);

        PtrExpanderIO expander = ExpanderIO::getExpander(currentSettings->expander);

        PtrTrainSensor currentSensor = new TrainSensor(currentSettings->mask, expander);

        result[idx] = currentSensor;
    }

    return result;
}

PtrServoModelRailroad* Crossing::initSensors(int& outSize, SimpleLinkedList<PtrSettingsPortOut>& input)
{
    outSize = input.size();

    PtrServoModelRailroad* result = new PtrServoModelRailroad[outSize];

    for (int idx = 0; idx < outSize; ++idx)
    {
        PtrSettingsPortOut currentSettings = input.get(idx);

        PtrServoModelRailroad currentServo = ServoModelRailroad::getServo(currentSettings->servo);

        result[idx] = currentServo;
    }

    return result;
}

Crossing::Crossing(PtrSettingsCrossing& settings)
{
    DebugMessagePrintf("Crossing::Crossing(PtrSettingsCrossing)\n");

    this->_dacAudio = new XT_DAC_Audio_Class(settings->pinDAC, settings->timerDAC);

    this->_data = readWavData(settings->soundFileName);

    this->_sound = new XT_Wav_Class(this->_data);

    this->_state = NoTrain;

    this->_enter = this->initSensors(this->_enterSize, settings->enter);
    this->_leave = this->initSensors(this->_leaveSize, settings->leave);
    this->_lights = this->initSensors(this->_lightsSize, settings->light);

    this->_fencesSize = settings->fences.size();
    this->_fences = new PtrServoModelRailroad[this->_fencesSize];

    for (int idx = 0; idx < this->_fencesSize; ++idx)
    {
        PtrSettingsFence currentSettings = settings->fences.get(idx);

        this->_fences[idx] = ServoModelRailroad::getServo(currentSettings->servoIndex);
    }

    this->_minVolume = settings->minVolume.value;
    this->_maxVolume = settings->maxVolume.value;
}

void Crossing::openFences()
{
    DebugMessagePrintf("Crossing::openFences()\n");

    for (int idxFence = 0; idxFence < this->_fencesSize; ++idxFence)
    {
        if (this->_fences[idxFence] != NULL)
        {
            this->_fences[idxFence]->moveToMaximum();
        }
    }
}

void Crossing::closeFences()
{
    DebugMessagePrintf("Crossing::closeFences()\n");

    for (int idxFence = 0; idxFence < this->_fencesSize; ++idxFence)
    {
        if (this->_fences[idxFence] != NULL)
        {
            this->_fences[idxFence]->moveToMinimum();
        }
    }
}

void Crossing::lightsOn()
{
    DebugMessagePrintf("Crossing::lightsOn()\n");

    for (int idx = 0; idx < this->_lightsSize; ++idx)
    {
        this->_lights[idx]->gotoToMaximum();
    }
}

void Crossing::lightsOff()
{
    DebugMessagePrintf("Crossing::lightsOff()\n");

    for (int idx = 0; idx < this->_lightsSize; ++idx)
    {
        this->_lights[idx]->gotoToMinimum();
    }
}

void Crossing::close()
{
    DebugMessagePrintf("Crossing::close()\n");

    this->_state = TrainEntering;
}

void Crossing::open()
{
    DebugMessagePrintf("Crossing::open()\n");

    this->_state = TrainLeaved;
}

bool Crossing::isSensorActive(PtrTrainSensor* sensors, int size)
{
    for (int index = 0; index < size; ++index)
    {
        if (sensors[index]->isTrainPresent())
        {
            return true;
        }
    }

    return false;
}

void Crossing::setMinVolume(int minVolume)
{
    DebugMessagePrintf("Crossing::setMinVolume(%d)\n", minVolume);

    this->_minVolume = minVolume;
}

void Crossing::setMaxVolume(int maxVolume)
{
    DebugMessagePrintf("Crossing::setMaxVolume(%d)\n", maxVolume);
    this->_maxVolume = maxVolume;
}

void Crossing::setStepVolume(int stepVolume)
{
    DebugMessagePrintf("Crossing::setStepVolume(%d)\n", stepVolume);

    this->_stepVolume = stepVolume;
}

void Crossing::loop()
{
    this->_dacAudio->FillBuffer();

    unsigned long currentTime = millis();

    switch (this->_state)
    {
        case NoTrain:
            if (this->isSensorActive(this->_enter, this->_enterSize) == true)
            {
                DebugMessagePrintf("Crossing::loop()#NoTrain\n");

                DebugMessagePrintf("Train detected...\n");
                this->_state = TrainEntering;
            }
            break;

        case TrainEntering:
            DebugMessagePrintf("Crossing::loop()#TrainEntering\n");

            this->lightsOn();
            this->_state = SoundStart;
            break;

        case SoundStart:
            DebugMessagePrintf("Crossing::loop()#SoundStart\n");

            this->_sound->RepeatForever = true;
            this->_dacAudio->Play(this->_sound);

            this->_volume = this->_minVolume;
            this->_dacAudio->DacVolume = this->_volume;

            this->_lastTime = currentTime;

            this->_state = SoundUp;
            break;

        case SoundUp:
            this->_sound->RepeatForever = true;

            if ((currentTime - this->_lastTime) > DeltaTime)
            {
                DebugMessagePrintf("Crossing::loop()#SoundUp\n");

                this->_lastTime = currentTime;

                this->_volume += this->_stepVolume;
                if (this->_volume >= this->_maxVolume)
                {
                    DebugMessagePrintf("Volume is max!\n");

                    this->_volume = this->_maxVolume;
                    this->_state = SoundPlaying;

                    this->closeFences();
                }
                this->_dacAudio->DacVolume = this->_volume;
            }
            break;

        case SoundPlaying:
            this->_sound->RepeatForever = true;

            if (this->isSensorActive(this->_leave, this->_leaveSize) == true)
            {
                DebugMessagePrintf("Crossing::loop()#SoundPlaying\n");
                this->_state = TrainLeaving;
            }
            break;

        case TrainLeaving:
            this->_sound->RepeatForever = true;

            if (this->isSensorActive(this->_leave, this->_leaveSize) == false)
            {
                DebugMessagePrintf("Crossing::loop()#TrainLeaving\n");
                this->_state = TrainLeaved;
            }
            break;

        case TrainLeaved:
            DebugMessagePrintf("Crossing::loop()#TrainLeaved\n");

            this->_sound->RepeatForever = true;

            this->_state = SoundDown;
            break;

        case SoundDown:
            this->_sound->RepeatForever = true;

            if ((currentTime - this->_lastTime) > DeltaTime)
            {
                DebugMessagePrintf("Crossing::loop()#SoundDown\n");
                this->_lastTime = currentTime;

                this->_volume -= this->_stepVolume;
                if (this->_volume <= this->_minVolume)
                {
                    DebugMessagePrintf("Volume is min...\n");

                    this->_volume = this->_minVolume;
                    this->_state = SoundStop;
                }

                this->_dacAudio->DacVolume = this->_volume;
            }
            break;

        case SoundStop:
            DebugMessagePrintf("Crossing::loop()#SoundStop\n");

            this->_sound->RepeatForever = false;

            this->openFences();
            this->lightsOff();

            this->_state = NoTrain;
            break;

        default:
            DebugMessagePrintf("Crossing::loop()#Invalid!!!\n");
            break;
    }
}

void Crossing::initCrossings()
{
    DebugMessagePrintf("Crossing::initCrossings()\n");

    PtrSettingsCrossings settings = getSettingsCrossings();

    int size = settings->crossings.size();

    DebugMessagePrintf("Processing %d crossings...\n", size);

    Crossing::_crossings = new PtrCrossing[size];
    Crossing::_numberOfCrossings = size;

    for (int index = 0; index < size; ++index)
    {
        PtrSettingsCrossing currentSettings = settings->crossings.get(index);

        PtrCrossing currentCrossing = new Crossing(currentSettings);

        Crossing::_crossings[index] = currentCrossing;
    }
}

PtrCrossing Crossing::getCrossing(int index)
{
    return Crossing::_crossings[index];
}

void Crossing::loops()
{
    for (int index = 0; index < Crossing::_numberOfCrossings; ++index)
    {
        Crossing::_crossings[index]->loop();
    }
}
