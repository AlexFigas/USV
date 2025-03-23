#ifndef _Utils_h_
#define _Utils_h_

#include <Arduino.h>
#include <Print.h>

/**
        \brief
        Print formatted data (equivalent to standard C printf) to the default <b><code>Serial</code></b> port.

        If format includes format specifiers (subsequences beginning with %),
        the additional arguments following format are formatted and inserted in
        the resulting string replacing their respective specifiers.

        \param sFmt C string that contains the text to be written to the default <b><code>Serial</code></b> port
*/
#define myPrintf(...) Serial.printf(__VA_ARGS__)

/**
        \brief
        Print formatted data (equivalent to standard C printf) to a C string

        Writes the C string pointed by <b>sFmt</b> to a C string

        If format includes format specifiers (subsequences beginning with %),
        the additional arguments following format are formatted and inserted in
        the resulting string replacing their respective specifiers.

        \param sFmt Arduino String object that contains the text to be written to Serial

        \return the formated string. Caller must delete the returned value in order to free allocated memory
*/
char* format(const char* sFmt, ...);

/**
        \brief
        An utility macro that initialize the default Serial port and waits until the port is ready
*/
#define serialBegin(speed)   \
    {                        \
        Serial.begin(speed); \
        while (!Serial)      \
            ;                \
    }

#define serialBeginNoBlock(speed) \
    {                             \
        Serial.begin(speed);      \
    }

#ifdef DebugMode
/**
        \brief
        An utility macro that is expanded to myPrintf(...) only if the define <b>DebugMode</b> is active.
*/
#define DebugMessagePrintf(...)     \
    {                               \
        Serial.printf(__VA_ARGS__); \
    }

/**
        \brief
        An utility macro that is expanded to serialBegin(...) only if the define <b>DebugMode</b> is active.
*/
#define DebugSerialBegin(speed) \
    {                           \
        serialBegin(speed);     \
    }

/**
        \brief
        An utility macro that is expanded to serialBeginNoBlock(...) only if the define <b>DebugMode</b> is active.
*/
#define DebugSerialBeginNoBlock(speed) \
    {                                  \
        serialBeginNoBlock(speed);     \
    }

/**
        \brief
        An utility macro that is expanded to delay(...) only if the define <b>DebugMode</b> is active.
*/
#define DebugDelay(amount) \
    {                      \
        delay(amount);     \
    }
#else
/**
        \brief
        An utility macro that is expanded to myPrintf(...) only if the define <b>DebugMode</b> is active.
*/
#define DebugMessagePrintf(...) \
    {                           \
    }

/**
        \brief
        An utility macro that is expanded to serialBegin(...) only if the define <b>DebugMode</b> is active.
*/
#define DebugSerialBegin(speed) \
    {                           \
    }

/**
        \brief
        An utility macro that is expanded to serialBeginNoBlock(...) only if the define <b>DebugMode</b> is active.
*/
#define DebugSerialBeginNoBlock(speed) \
    {                                  \
    }

/**
        \brief
        An utility macro that is expanded to delay(...) only if the define <b>DebugMode</b> is active.
*/
#define DebugDelay(amount) \
    {                      \
    }
#endif

#endif  // _Utils_h_
