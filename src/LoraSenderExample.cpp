// Libraries for LoRa
#include <LoRa.h>
#include <SPI.h>

// Libraries for OLED Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// LoRa Transceiver Pins
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

// Frequency Bands
constexpr long BAND = 866E6;  // 433E6 for Asia, 866E6 for Europe, 915E6 for North America

// OLED Display Pins
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Packet counter
int packetCounter = 0;

// OLED Display Object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void initializeOLED()
{
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, LOW);
    delay(20);
    digitalWrite(OLED_RST, HIGH);

    Wire.begin(OLED_SDA, OLED_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false))
    {
        Serial.println(F("SSD1306 initialization failed"));
        while (true)
            ;  // Halt execution
    }

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("LoRa Sender");
    display.display();
}

void initializeLoRa()
{
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(BAND))
    {
        Serial.println("LoRa initialization failed!");
        while (true)
            ;  // Halt execution
    }

    Serial.println("LoRa Initialized Successfully!");
    display.setCursor(0, 10);
    display.print("LoRa Ready!");
    display.display();
    delay(2000);
}

void sendPacket()
{
    Serial.print("Sending packet: ");
    Serial.println(packetCounter);

    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(packetCounter);
    LoRa.endPacket();

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("LoRa Sender");
    display.setCursor(0, 20);
    display.print("Packet Sent!");
    display.setCursor(0, 30);
    display.print("Counter: ");
    display.print(packetCounter);
    display.display();

    packetCounter++;
}

void setup()
{
    Serial.begin(115200);

    initializeOLED();
    initializeLoRa();
}

void loop()
{
    sendPacket();
    delay(10000);  // Wait 10 seconds before sending the next packet
}
