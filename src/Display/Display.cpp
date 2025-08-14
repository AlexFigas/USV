#include "Display.h"

Display::Display(int width, int height, int rstPin, int sdaPin, int sclPin, uint8_t addr)
    : width(width), height(height), rstPin(rstPin), sdaPin(sdaPin), sclPin(sclPin), addr(addr)
{
    textSize = 1;
    textColor = SSD1306_WHITE;
    cursorX = 0;
    cursorY = 0;
    display = new Adafruit_SSD1306(width, height, &Wire, rstPin);
}

Display::Display()
{
    width = 128;
    height = 64;
    rstPin = 16;
    sdaPin = 4;
    sclPin = 15;
    addr = 0x3C;
    textSize = 1;
    textColor = SSD1306_WHITE;
    cursorX = 0;
    cursorY = 0;
    maxCharsPerLine = (width / (6 * textSize));  // Default font width is 6px
    display = new Adafruit_SSD1306(width, height, &Wire1, rstPin);
}

void Display::setup()
{
    Wire1.begin(sdaPin, sclPin);
    pinMode(rstPin, OUTPUT);
    digitalWrite(rstPin, LOW);
    delay(20);
    digitalWrite(rstPin, HIGH);

    if (!display->begin(SSD1306_SWITCHCAPVCC, addr))
    {
        Serial.println(F("SSD1306 allocation failed"));
        while (true)
            ;
    }

    display->clearDisplay();
    applySettings();
    display->display();
    Serial.println(F("Display initialized"));
}

void Display::applySettings()
{
    display->setTextSize(textSize);
    display->setTextColor(textColor);
    display->setCursor(cursorX, cursorY);
}

void Display::printf(const char* format, int line, ...)
{
    char tempBuffer[128];
    va_list args;
    va_start(args, line);
    vsnprintf(tempBuffer, sizeof(tempBuffer), format, args);
    va_end(args);

    String tempString = String(tempBuffer);
    printLine(tempString, line);
}

void Display::printLine(String& str, int line)
{
    if (str.length() > maxCharsPerLine)
    {
        // Truncate the string to fit the display
        String truncatedStr = str.substring(0, maxCharsPerLine);
        str = truncatedStr;
    }

    int lineHeight = 8 * textSize;
    int y = line * lineHeight;

    // Clear the line area
    display->fillRect(0, y, width, lineHeight, SSD1306_BLACK);

    // Set cursor and print
    display->setCursor(0, y);
    display->print(str);
    display->display();
}

void Display::clear()
{
    display->clearDisplay();
    cursorX = 0;
    cursorY = 0;
    display->display();
}