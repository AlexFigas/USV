#ifndef _Buttons_h_
#define _Buttons_h_

#include <Button2.h>

/**
 * Initialization of the button used to show the satus
 */
void initializeButtons(Button2& bShowStatus);

/**
 * Handler associated with the pressing of the button used to show the status
 */
void handleClick(Button2& bShowStatus);

#endif  // _Buttons_h_
