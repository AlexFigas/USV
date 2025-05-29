#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

// Set up PWM driver on default I2C address (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define ESC_CHANNEL 15  // Channel 15 on PCA9685
#define MIN_PULSE 1000  // microseconds
#define MAX_PULSE 2000  // microseconds
#define PWM_FREQ 50     // Hz for ESCs and servos

constexpr int8_t SDA_PIN = 21;
constexpr int8_t SCL_PIN = 22;

// Helper to convert microseconds to PCA9685 ticks (12-bit)
uint16_t pulseToTicks(int microseconds)
{
    // 1 tick = 20,000 us / 4096 = ~4.88 us
    return (uint16_t)((microseconds * 4096L) / 20000);
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    pwm.begin();
    pwm.setPWMFreq(PWM_FREQ);  // ESCs expect 50 Hz

    delay(10);
}

void loop()
{
    pwm.setPWM(ESC_CHANNEL, 0, pulseToTicks(1852));  // forward
    delay(4000);

    pwm.setPWM(ESC_CHANNEL, 0, pulseToTicks(1488));  // stop
    delay(4000);

    pwm.setPWM(ESC_CHANNEL, 0, pulseToTicks(1070));  // reverse
    delay(4000);

    pwm.setPWM(ESC_CHANNEL, 0, pulseToTicks(1488));  // stop
    delay(4000);
}