#include <Arduino.h>
#include <Wire.h>

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

void scanI2C()
{
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            // Device found
            Serial.print("I2C device found at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.print(address, HEX);
            Serial.println(" !");
            nDevices++;
            delay(500);  // Slow down the scanning for readability
        }
        else if (error == 4)
        {
            // Error reading from the device
            Serial.print("Unknown error at address 0x");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }

    if (nDevices == 0)
    {
        Serial.println("No I2C devices found");
    }
    else
    {
        Serial.println("Scan complete");
    }
}

void setup()
{
    // Start the serial communication
    Serial.begin(115200);
    delay(1000);

    // Initialize I2C with custom SDA and SCL pins
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    // Begin scanning
    Serial.println("I2C Scanner started...");
    scanI2C();
}

void loop() {}
