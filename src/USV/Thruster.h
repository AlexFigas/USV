// #ifndef THRUSTER_H
// #define THRUSTER_H

// #include "Motor.h"

// struct ThrusterController : public MotorController
// {
//     uint16_t PWM_MIN_PULSE = 1000;
//     uint16_t PWM_MAX_PULSE = 2000;
//     uint16_t PWM_STOP_PULSE = 1500;
//     uint8_t PWM_FREQ = 50;  // Hz for ESCs and servos
//     uint16_t THROTTLE_THRESHOLD = 100;
// };

// class Thruster : public Motor
// {
//   public:

//     // Public constants

//     // Public variables

//     // Public methods
//     Thruster(Expander expander, ThrusterController controller);

//     void setDirection(bool clockwise) override;

//     void front(float speed, float length = 0) override;

//     void back(float speed, float cm = 0) override;

//     void stop() override;

//     void setSpeed(float speed) override;

//     void setPWM(int pwm) override;

//   private:
//     // Private constants

//     // Private variables
//     ThrusterController controller;

//     // Private methods
// };

// #endif
