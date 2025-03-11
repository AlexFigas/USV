#ifndef _Display_h_
#define _Display_h_

#include <Print.h>

/**
 * Initialization of the display
 */
void initializeDisplay();

/**
 * Check if the display was correctly initialized
 */
bool isDisplayReady();

/**
 * Clear the display
 */
void clearDisplay();

/**
 * Get a reference to the display as a pointer to a Print object
 */
Print* getDisplay();

/**
 * Amount of time (in milli seconds) before clear the display
 */
unsigned long getMessageTimeout();

#endif	// _Display_h_
