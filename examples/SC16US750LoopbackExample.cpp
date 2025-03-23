#include "SC16IS7X0.h"
#include <Arduino.h>

constexpr uint32_t CRYSTAL_FREQ = 14745600;
constexpr uint8_t I2C_ADD = 0x4D;
;
constexpr uint32_t UART_BAUD = 115200;

SC16IS7X0 sc16is750(CRYSTAL_FREQ);

constexpr int8_t SDA_PIN = 21;
constexpr int8_t SCL_PIN = 22;

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
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    delay(100);
    Serial.println("setup");
    scanI2C();

    if (sc16is750.begin_I2C(I2C_ADD))
        Serial.println("I2C initialized");
    sc16is750.begin_UART(UART_BAUD);
    sc16is750.enableLoopback();
}

void loop()
{
    static uint32_t n = 0;

    sc16is750.printf("loop %d\n", n++);
    while (sc16is750.available())
        Serial.write(sc16is750.read());

    delay(2000);
}