#include "Display.h"
#include <SPIFFS.h>
#include <TFT_eSPI.h>
#include <Utils.h>

#define TOUCH_CS -1

#define FontSize 2

#define BlackLightPin 4

static const unsigned long MessageTimeout = 2000;

static const char* LogoFileName = "/LogoTrain.bmp";

static bool displayReady;

static TFT_eSPI display = TFT_eSPI();

static uint16_t read16(File& f)
{
    uint16_t result;
    ((uint8_t*)&result)[0] = f.read();  // LSB
    ((uint8_t*)&result)[1] = f.read();  // MSB
    return result;
}

static uint32_t read32(fs::File& f)
{
    uint32_t result;
    ((uint8_t*)&result)[0] = f.read();  // LSB
    ((uint8_t*)&result)[1] = f.read();
    ((uint8_t*)&result)[2] = f.read();
    ((uint8_t*)&result)[3] = f.read();  // MSB
    return result;
}

static void drawBmp(TFT_eSPI& display, const char* fileName, int16_t x, int16_t y)
{

    if ((x >= display.width()) || (y >= display.height()))
    {
        return;
    }

    File bmpFS;

    // Open requested file
    bmpFS = SPIFFS.open(fileName, "r");

    if (!bmpFS)
    {
        DebugMessagePrintf("File %s not found.\n", fileName);
        return;
    }

    uint32_t seekOffset;
    uint16_t w, h, row, col;
    uint8_t r, g, b;

    if (read16(bmpFS) == 0x4D42)
    {
        read32(bmpFS);
        read32(bmpFS);
        seekOffset = read32(bmpFS);
        read32(bmpFS);
        w = read32(bmpFS);
        h = read32(bmpFS);

        if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0))
        {
            y += h - 1;

            bool oldSwapBytes = display.getSwapBytes();
            display.setSwapBytes(true);
            bmpFS.seek(seekOffset);

            uint16_t padding = (4 - ((w * 3) & 3)) & 3;
            uint8_t lineBuffer[w * 3 + padding];

            for (row = 0; row < h; row++)
            {
                bmpFS.read(lineBuffer, sizeof(lineBuffer));
                uint8_t* bptr = lineBuffer;
                uint16_t* tptr = (uint16_t*)lineBuffer;
                // Convert 24 to 16 bit colours
                for (uint16_t col = 0; col < w; col++)
                {
                    b = *bptr++;
                    g = *bptr++;
                    r = *bptr++;
                    *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
                }

                // Push the pixel row to screen, pushImage will crop the line if needed
                // y is decremented as the BMP image is drawn bottom up
                display.pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
            }
            display.setSwapBytes(oldSwapBytes);
        }
        else
        {
            DebugMessagePrintf("BMP format not recognized.\n");
        }
    }
    bmpFS.close();
}

static void turnOnBlackLight()
{
    digitalWrite(BlackLightPin, HIGH);
}

static void turnOffBlackLight()
{
    digitalWrite(BlackLightPin, LOW);
}

void initializeDisplay()
{
    display.init();

    // Black background
    display.fillScreen(TFT_BLACK);

    // Set "cursor" at top left corner of display (0,0) and select font size "FontSize"
    display.setCursor(0, 0, FontSize);

    // Set the font colour to be white with a black background
    display.setTextColor(TFT_WHITE, TFT_BLACK);

    display.setRotation(1);

    // Logo size = 182 x 128

    int x = (display.width() - 182) / 2;
    int y = (display.height() - 128) / 2;

    drawBmp(display, LogoFileName, x, y);

    displayReady = true;

    pinMode(BlackLightPin, OUTPUT);

    turnOnBlackLight();

    DebugMessagePrintf("Display is ready.\n");
}

bool isDisplayReady()
{
    return displayReady;
}

void clearDisplay()
{
    if (isDisplayReady())
    {
        turnOffBlackLight();
        display.fillScreen(TFT_BLACK);
        display.setCursor(0, 0, FontSize);
    }
}

Print* getDisplay()
{
    if (isDisplayReady())
    {
        turnOnBlackLight();
        return (Print*)(&display);
    }

    return NULL;
}

unsigned long getMessageTimeout()
{
    return MessageTimeout;
}
