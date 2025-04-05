#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class Display
{
  public:
    Display(int width, int height, int rstPin, int sdaPin = 4, int sclPin = 15, uint8_t addr = 0x3C);
    Display();

    void setup();
    void printf(const char* format, int line, ...);

  private:
    int width, height, rstPin, sdaPin, sclPin;
    uint8_t addr;
    Adafruit_SSD1306* display;

    uint8_t textSize;
    uint16_t textColor;
    int cursorX, cursorY;

    unsigned long lastClearTime = 0;
    bool needsUpdate = false;  // If true, update the display

    String buffer;  // Store all text before displaying

    int maxCharsPerLine;

    void applySettings();
    void printLine(String& str, int line);
};

#endif
