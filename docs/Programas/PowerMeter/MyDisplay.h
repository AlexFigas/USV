#ifndef MyDisplay_h
#define MyDisplay_h

#include <TFT_eSPI.h>

#define FontName "Calibri30"

#define FontSize 2

#define OrientationPortrait 0
#define OrientationLandspace 1

#define DisplayOrientation OrientationPortrait

void initDisplay(TFT_eSPI& display);

void clearDisplay(TFT_eSPI& display);

void setPositionDisplay(TFT_eSPI& display, int x=0, int y=0);

void offDisplay(void);

void onDisplay(void);

void drawBmpDisplay(TFT_eSPI& display, const char* filename, int16_t x, int16_t y);

#endif	// MyDisplay_h
