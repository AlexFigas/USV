#ifndef _MyTime_h_
#define _MyTime_h_

#include <Arduino.h>
#include <UtilsBoards.h>

void initTime();

String getCurrentDateAndTime(char request = '.', time_t timeStamp = time(nullptr));

int getCurrentHours(time_t timeStamp = time(nullptr));

int getCurrentMinutes(time_t timeStamp = time(nullptr));

int getCurrentSeconds(time_t timeStamp = time(nullptr));

void loopTime(void);

#endif  // _MyTime_h_
