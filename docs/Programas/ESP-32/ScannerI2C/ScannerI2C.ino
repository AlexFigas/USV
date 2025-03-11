#include <Wire.h>
#include <Utils.h>
#include <UtilsI2C.h>

const int dataPinSDA = 21;
const int dataPinSCL = 22;

void setup() {
  DebugDelay( 2000 );

  serialBeginNoBlock( 115200 );
  Serial.println( "Serial ready." );

  Serial.println( "I2C Scanner." );

  Wire.begin( dataPinSDA, dataPinSCL );
  Serial.println( "Wire is ready." );
}

void loop() {
  scanBus( Wire );
  
  // Wait 1 seconds for next scan
  delay( 1000 );
}
