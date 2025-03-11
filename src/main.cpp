#include <Arduino.h>
#include <Wire.h>

#define SC16IS750_I2C_ADDRESS 0x4D  // Default I2C address for SC16IS750

// Function to initialize SC16IS750 (you can adjust settings as needed)
bool initializeSC16IS750()
{
    Wire.beginTransmission(SC16IS750_I2C_ADDRESS);
    Wire.write(0x00);  // Example: Access control register
    Wire.write(0x80);  // Example: Enable divisor latch access for baud rate setup
    Wire.endTransmission();

    delay(100);

    // Check the device's response (for simplicity, we can assume it's working)
    Wire.beginTransmission(SC16IS750_I2C_ADDRESS);
    Wire.write(0x00);  // Read register
    Wire.endTransmission();

    Wire.requestFrom(SC16IS750_I2C_ADDRESS, 1);
    return Wire.available();  // Check if we got a response
}

void setup()
{
    Serial.begin(115200);  // Start serial communication for debugging
    Wire.begin();          // Start I2C communication with ESP32

    // Initialize SC16IS750 and print status
    if (initializeSC16IS750())
    {
        Serial.println("SC16IS750 initialized successfully!");
    }
    else
    {
        Serial.println("SC16IS750 initialization failed.");
    }
}

void loop()
{
    // No additional code, just check the initialization once
}
