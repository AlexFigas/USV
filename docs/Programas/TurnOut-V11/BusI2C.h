#ifndef _BusI2C_h_
#define _BusI2C_h_

// SDA and SCL pins
const int dataPinSDA = 21;  // GPIO21
const int dataPinSCL = 22;  // GPIO22

/**
 * Initialization of the I2C bus
 */
void initializeWire(int dataPin = dataPinSDA, int clockPin = dataPinSCL);

#endif  // _BusI2C_h_
