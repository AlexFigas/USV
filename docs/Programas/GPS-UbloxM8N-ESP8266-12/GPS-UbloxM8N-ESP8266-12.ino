#include <Utils.h>
#include <UtilsI2C.h>
#include <DisplaySWIMU.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

static const int pinRXs1 = 14;  // D5
static const int pinTXs1 = 12;  // D6

static const int pinSDA = 5;    // D1
static const int pinSCL = 4;    // D2

static const uint32_t CompassID = 12345;

static const int baudRateGPS = 9600;

static const uint32_t MessageDelay = 5000;

SoftwareSerial portGPS( pinRXs1, pinTXs1 );

TinyGPSPlus gps;

// Assign a Unique ID to the HMC5883 Compass Sensor
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified( CompassID );

void displayCompassSensorDetails() {
    sensor_t sensor;
    mag.getSensor( &sensor );
    
    DebugMessagePrintf( "Sensor:          %s\n", sensor.name );
    DebugMessagePrintf( "Driver Ver:      %d\n", sensor.version );
    DebugMessagePrintf( "Unique ID:       %d\n", sensor.sensor_id );
    DebugMessagePrintf( "Max Value (uT):  %6.3f\n", (float)sensor.max_value );
    DebugMessagePrintf( "Min Value (uT):  %6.3f\n", (float)sensor.min_value );
    DebugMessagePrintf( "Resolution (uT): %6.3f\n\n", (float)sensor.resolution );

    // clearDisplay();
    // showMessageAtLine( POSITION_LINE_1, false, "Sensor %s, Ver %d", sensor.name, sensor.version );
    // showMessageAtLine( POSITION_LINE_2, false, "ID %d", sensor.sensor_id );
    // showMessageAtLine( POSITION_LINE_3, false, "Max %6.2f Min %6.2f Re %6.2f", (float)sensor.max_value, (float)sensor.min_value );
    // showMessageAtLine( POSITION_LINE_4, true, "Res %6.2f", (float)sensor.resolution );

    // delay( MessageDelay );
}

void displayCompassInfo(boolean showOnConsole=false) {
  // Get a new sensor event 
  sensors_event_t event; 
  mag.getEvent( &event );

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2( event.magnetic.y, event.magnetic.x );
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  
  float declinationAngle = 0.02647;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if( heading < 0 ) {
    heading += 2*PI;
  }
    
  // Check for wrap due to addition of declination.
  if( heading > 2*PI ) {
    heading -= 2*PI;
  }
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI;

  // Display the results (magnetic vector values are in micro-Tesla (uT))
  // if ( showOnConsole==true ) {
    DebugMessagePrintf( "X(uT): %6.3f, Y(uT): %6.3f, Z(uT): %6.3f, Heading(degrees): %6.3f\n", (float)event.magnetic.x, (float)event.magnetic.y, (float)event.magnetic.z, headingDegrees);
  // }

  // showMessageAtLine( POSITION_LINE_4, true, "Heading %5.2f", headingDegrees);
}

void displayGpsInfo(boolean showOnConsole=false) {
  // Prints the location if lat-lng information was recieved
  // Prints "INVALID" if no information was recieved in regards to location.

  // clearDisplay();
  
  if ( gps.location.isValid() ) {
    // if ( showOnConsole==true ) {
      DebugMessagePrintf( "Location: %f, %f\n", gps.location.lat(), gps.location.lng() );
    // }
    
    // showMessageAtLine( POSITION_LINE_1, false, "%3.7f, %3.7f", gps.location.lat(), gps.location.lng() );
  }
  else {
    // if ( showOnConsole==true ) {
      DebugMessagePrintf( "Location: INVALID\n" );
    // }

    // showMessageAtLine( POSITION_LINE_1, false, "Local: INVALID" );
  }

  // Prints the recieved GPS module date if it was decoded in a valid response.
  // Prints invalid otherwise.
  if ( gps.date.isValid() ) {
    // if ( showOnConsole==true ) {
      DebugMessagePrintf( "Date (dd/mm/yyyy): %02d/%02d/%04d\n", gps.date.day(), gps.date.month(), gps.date.year() );
    // }

    // showMessageAtLine( POSITION_LINE_2, false, "Date %02d/%02d/%04d", gps.date.day(), gps.date.month(), gps.date.year() );
  }
  else {
    // if ( showOnConsole==true ) {
      DebugMessagePrintf( "Date: INVALID\n" );
    // }

    // showMessageAtLine( POSITION_LINE_2, false, "Date: INVALID" );
  }

  // Prints the recieved GPS module time if it was decoded in a valid response.  
  // Print invalid otherwise.
  if ( gps.time.isValid() ) {
    // if ( showOnConsole==true ) {
      DebugMessagePrintf( "Time (HH:MM:SS.ss): %02d:%02d:%02d\n", gps.time.hour(), gps.time.minute(), gps.time.second() );
    // }

    // showMessageAtLine( POSITION_LINE_3, false, "Time: %02d:%02d:%02d\n", gps.time.hour(), gps.time.minute(), gps.time.second() );
  }
  else {
    // if ( showOnConsole==true ) {
      DebugMessagePrintf( "Time: INVALID\n" );
    // }

    // showMessageAtLine( POSITION_LINE_3, false, "Time: INVALID" );
  }
}

void readRaw_while() {
  while ( portGPS.available() ) {
    DebugMessagePrintf( "%c", (char)( portGPS.read() ) );
  }
}

void readRaw_if() {
  if ( portGPS.available() ) {
    DebugMessagePrintf( "%c", (char)( portGPS.read() ) );
  }
}

void readLib_while(boolean showRaw=true, boolean showOnConsole=false) {
  while ( portGPS.available() > 0 ) {
    int aux = portGPS.read();

    if ( showRaw==true ) {
      DebugMessagePrintf( "%c", (char)aux );
    }
    
    if ( gps.encode( aux ) ) {
      displayGpsInfo( showOnConsole );

      displayCompassInfo( showOnConsole );
    }
  }
}

void readLib_if(boolean showRaw=true, boolean showOnConsole=false) {
  if ( portGPS.available() > 0 ) {
    int aux = portGPS.read();

    if ( showRaw==true ) {
      DebugMessagePrintf( "%c", (char)aux );
    }
    
    if ( gps.encode( aux ) ) {
      displayGpsInfo( showOnConsole );

      displayCompassInfo( showOnConsole );
    }

    // if ( showRaw==true ) {
      // DebugMessagePrintf( "\n" );
    // }
  }
}

void setup() {
  DebugDelay( 3000 );

  Wire.begin( pinSDA, pinSCL );
  // initializeDisplay();
  // delay( MessageDelay );
  // clearDisplay( true );

  DebugSerialBeginNoBlock( 115200 );
  DebugMessagePrintf( "Serial is ready.\n" );

  DebugMessagePrintf( "Scanning I2C...\n" );
  scanBus( Wire /*, (*getDisplay())*/ );
  // updateDisplay( MessageDelay );
  
  DebugMessagePrintf( "Configuring GPS serial port...\n" );
  portGPS.begin( baudRateGPS );
  while( !portGPS )
    ;
  DebugMessagePrintf( "GPS serial port is ready.\n" );
  
  // clearDisplay();
  // showMessage( false, NULL, "GPS port ready.\n" );

  DebugMessagePrintf( "GPS serial TX is on pin: %d\n", pinTXs1 );
  DebugMessagePrintf( "GPS serial RX is on pin: %d\n", pinRXs1 );

  // showMessage( true, NULL, "TX=%d, RX=%d\n", pinTXs1, pinRXs1 );
  // updateDisplay( MessageDelay );

  displayCompassSensorDetails();
  // updateDisplay( MessageDelay );

  DebugMessagePrintf( "Simple Test with TinyGPS++ and attached NEO-M8N-0-01 GPS module.\n" );
  DebugMessagePrintf( "Testing TinyGPS++ library version %s\n", TinyGPSPlus::libraryVersion() );

  // showMessage( false, NULL, "TinyGPS++ ver %s\n", TinyGPSPlus::libraryVersion() );

  DebugMessagePrintf( "Setup is ready.\n" );

  // clearDisplay();
  // showMessage( true, NULL, "Setup ready.\n" );
  // delay( MessageDelay );
  // clearDisplay( true );
}

boolean showRaw = true;
boolean showOnConsole = true;

void loop() {
  //readRaw_while();

  //readRaw_if();

  //readLib_while( showRaw, showOnConsole);

  readLib_if( showRaw, showOnConsole);
}

// Lat e Lon não vêm em decimal: Latitude: DDMM.MMMM Longitude DDDMM.MMMM
// N & E são + , S & W são -
