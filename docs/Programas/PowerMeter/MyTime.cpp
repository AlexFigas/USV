#include <Utils.h>
#include <UtilsBoards.h>

#include "MyTime.h"

#ifdef ARDUINO_ARCH_ESP8266
  // http://www.weigu.lu/microcontroller/tips_tricks/esp_NTP_tips_tricks/index.html
  // https://werner.rothschopf.net/202011_arduino_esp8266_ntp_en.htm
  
  #include <coredecls.h>
#else
  // https://techtutorialsx.com/2021/09/01/esp32-system-time-and-sntp/
  // https://techtutorialsx.com/2021/09/03/esp32-sntp-additional-features/

  #include "esp_sntp.h"  
#endif

#define offSetGTM   0

// Value in milli seconds
#define offSetUTC   0

// Value in milli seconds
//#define intervalUpdateNTP 60*60*1000  // 1 hour
#define intervalUpdateNTP 15*60*1000     // 15 minutes

#define NTP_ADDRESS "pool.ntp.org"

static char _bufferDateAndTime[32];

static bool _timeUpdated;

/*
 * https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
 * https://www.geeksforgeeks.org/strftime-function-in-c/
 * 
 *  Specifier     Replaced By                                         Example
 *
 *  %a            Abbreviated weekday name                                                  Sun
 *  %A            Full weekday name                                                         Sunday
 *  %b            Abbreviated month name                                                    Mar
 *  %B            Full month name                                                           March
 *  %c            Date and time representation                                              Sun Aug 19 02:56:02 2012
 *  %d            Day of the month (01-31)                                                  19
 *  %H            Hour in 24h format (00-23)                                                14
 *  %I            Hour in 12h format (01-12)                                                05
 *  %j            Day of the year (001-366)                                                 231
 *  %m            Month as a decimal number (01-12)                                         08
 *  %M            Minute (00-59)                                                            55
 *  %p            AM or PM designation                                                      PM
 *  %S            Second (00-61)                                                            02
 *  %U            Week number with the first Sunday as the first day of week one (00-53)    33
 *  %w            Weekday as a decimal number with Sunday as 0 (0-6)                        4
 *  %W            Week number with the first Monday as the first day of week one (00-53)    34
 *  %x            Date representation                                                       08/19/12
 *  %X            Time representation                                                       02:50:06
 *  %y            Year, last two digits (00-99)                                             01
 *  %Y            Year                                                                      2012
 *  %Z            Timezone name or abbreviation                                             CDT
 *  %%            A % sign                                                                  %
 *  
 *  %F            Equivalent to "%Y-%m-%d" (the ISO 8601 date format)
 *  %T            Equivalent to "%H:%M:%S" (the ISO 8601 time format)
 */

const char* _patternDateAndTime  = (const char* )"%Y-%m-%d %H:%M:%S";
const char* _patternDate         = (const char* )"%Y-%m-%d";
const char* _patternTime         = (const char* )"%H:%M:%S";

#ifdef ARDUINO_ARCH_ESP8266
  uint32_t sntp_update_delay_MS_rfc_not_less_than_15000 () {
    return intervalUpdateNTP;
  }
  
  void timeUpdatedCallBack() {
    #ifdef DEBUG_NTP
      DebugMessagePrintf( "timeUpdatedCallBack()\n" );
      DebugMessagePrintf( "Local system time: %s\n", getCurrentDateAndTime().c_str() );
    #endif

    _timeUpdated = true;
  }
#else
  void timeUpdatedCallBack(struct timeval* updateTime) {
    #ifdef DEBUG_NTP
      DebugMessagePrintf( "timeUpdatedCallBack()\n" );

      // Using the argument passed to the call back function
      char* localCalendarTime = ctime( &updateTime->tv_sec );
      localCalendarTime[ strcspn( localCalendarTime, "\n" ) ] = '\0';
      DebugMessagePrintf( "Seconds of elapsed time since the start of the epoch: %d\n", updateTime->tv_sec );
      DebugMessagePrintf( "Local calendar time: %s\n", localCalendarTime );

      // Using the local system time
      DebugMessagePrintf( "Local system time: %s\n", getCurrentDateAndTime().c_str() );
    #endif

    _timeUpdated = true;
  }
#endif

void initTime() {
  
  #ifdef DEBUG_NTP
    DebugMessagePrintf( "initTime(%s)\n", _defaultTelegramUser.c_str() );
    DebugMessagePrintf( "Setting NTP interval update time to %d seconds...\n", intervalUpdateNTP/1000 );
  #endif

  _timeUpdated = false;
  
  #ifdef ARDUINO_ARCH_ESP8266
    settimeofday_cb( timeUpdatedCallBack );
  #else
    sntp_set_time_sync_notification_cb( timeUpdatedCallBack );

    sntp_set_sync_interval( intervalUpdateNTP );
  #endif

  // Get UTC time via NTP
  configTime(offSetGTM, offSetUTC, NTP_ADDRESS);

  // Here we can assume that WiFi is turned On.
  // Sooner or later we will have a connection with the NTP Server.
  // If we can not setup the correct time from a NTP server we can not use Telegram
  // If the connection time is greater than numberOfSeconds we reboot the ESP
  
  int numberOfRetries = 0;
  int delayTime = 500;
  int numberOfSeconds = 120;
  int stepsPerSecond = 1000 / delayTime;

  while ( _timeUpdated==false ) {
    #ifdef DEBUG_NTP
      DebugMessagePrintf( "." );
    #endif

    if ( numberOfRetries++ > stepsPerSecond*numberOfSeconds ) {
      #ifdef DEBUG_NTP
        DebugMessagePrintf( "After waiting %d seconds NTP did not respond. Going to reset...\n", numberOfSeconds );
      #endif

      ESP.restart();
    }
    
    delay( delayTime );
  }

  #ifdef DEBUG_NTP
    DebugMessagePrintf( "Board local date and time: %s\n", getCurrentDateAndTime().c_str() );

    time_t timeStamp = time( nullptr );
    DebugMessagePrintf( "Board current hours: %d\n", getCurrentHours( timeStamp ) );
    DebugMessagePrintf( "Board current minutes: %d\n", getCurrentMinutes( timeStamp ) );
    DebugMessagePrintf( "Board current seconds: %d\n", getCurrentSeconds( timeStamp ) );

  #endif  // DEBUG_NTP
}

String getCurrentDateAndTime(char request, time_t timeStamp) {
  #if defined(DEBUG_NTP) && defined(DEBUG_NTP_EXTRA)
    DebugMessagePrintf( "getCurrentDateAndTime(%c, %ld)\n", request, timeStamp); 
  #endif
  
  const char* pattern;

  switch ( request ) {
    case 'd':
    case 'D':
      pattern = _patternDate;
      break;

    case 't':
    case 'T':
      pattern = _patternTime;
      break;

    default:
      pattern = _patternDateAndTime;
      break;
  }

  strftime( _bufferDateAndTime, sizeof(_bufferDateAndTime), pattern, localtime( &timeStamp ) );
  return String( _bufferDateAndTime );
}

int getCurrentHours(time_t timeStamp) {
  #if defined(DEBUG_NTP) && defined(DEBUG_NTP_EXTRA)
    DebugMessagePrintf( "getCurrentHours(%ld)\n", timeStamp); 
  #endif

  return localtime( &timeStamp )->tm_hour;
}

int getCurrentMinutes(time_t timeStamp) {
  #if defined(DEBUG_NTP) && defined(DEBUG_NTP_EXTRA)
    DebugMessagePrintf( "getCurrentMinutes(%ld)\n", timeStamp); 
  #endif

  return localtime( &timeStamp )->tm_min;
}

int getCurrentSeconds(time_t timeStamp) {
  #if defined(DEBUG_NTP) && defined(DEBUG_NTP_EXTRA)
    DebugMessagePrintf( "getCurrentSeconds(%ld)\n", timeStamp); 
  #endif

  return localtime( &timeStamp )->tm_sec;
}

void loopTime(void) {
  if ( _timeUpdated==true ) {
    _timeUpdated =  false;

    #ifdef DEBUG_NTP_EXTRA
      DebugMessagePrintf( "loopTime()\n" );

      #ifdef  USER_WANTS_TELEGRAM
        sendMessageToTelegram( _defaultTelegramUser, "Local clock updated on %s to: %s\nFree heap memory: %d", BOARD_NAME, getCurrentDateAndTime().c_str(), ESP.getFreeHeap() );
      #endif
    #endif
  }
}
