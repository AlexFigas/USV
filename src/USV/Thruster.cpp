// #include "Thruster.h"

// Thruster::Thruster(Expander expander, ThrusterController controller) : Motor(expander, controller)
// {
// }

// void Thruster::setDirection(bool clockwise)
// {
//     if (clockwise)
//     {
//         _expander.setMinPwmOn(controller.PWM_STOP_PULSE + controller.THROTTLE_THRESHOLD);
//         _expander.setMaxPwmOn(controller.PWM_MAX_PULSE - controller.THROTTLE_THRESHOLD);
//     }
//     else
//     {
//         _expander.setMinPwmOn(controller.PWM_STOP_PULSE - controller.THROTTLE_THRESHOLD);
//         _expander.setMaxPwmOn(controller.PWM_MIN_PULSE + controller.THROTTLE_THRESHOLD);
//     }
// }

// void Thruster::stop()
// {
//     _expander.setDutyCycle(controller.PIN_EN, controller.PWM_STOP_PULSE);
// }
