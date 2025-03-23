#ifndef _UtilsI2C_h_
#define _UtilsI2C_h_

#include <Arduino.h>
#include <Print.h>
#include <Wire.h>

/**
        \brief
        Scan a <b><code>I2C</code></b> bus for devices.

        This function scans all the addresses (from 1 to 127) of the <b><code>I2C</code></b> bus using the return value
   of the <code>Write.endTransmisstion</code>.

        When it finds a device prints to the default <code>Serial</code> port a message saying the a device was found on
   a specifc address.

        \param bus the I2C bus
*/
void scanBus(TwoWire& bus);

/**
        \brief
        Scan a <b><code>I2C</code></b> bus for devices.

        This function scans all the addresses (from 1 to 127) of the <b><code>I2C</code></b> bus using the return value
   of the <code>Write.endTransmisstion</code>.

        When it finds a device prints to the default <code>Serial</code> port a message saying the a device was found on
   a specifc address.

        This function assumes that the <code>Print</code> argument is associated wit a small display (LCD ou OLED).
        So when a <b><code>I2C</code></b> device is found a small message is also printed in the display saying that a
   new device was found.

        \param bus the I2C bus
        \param pr Arduino <code>Print</code> object that suports the <b><code>print</code></b> and
   <b><code>println</code></b> methods
*/
void scanBus(TwoWire& bus, Print& pr, char* busName = NULL, bool showInSerial = false);

/**
        \brief
        Scan a <b><code>I2C</code></b> bus for devices.

        This function scans all the addresses (from 1 to 127) of the <b><code>I2C</code></b> bus using the return value
   of the <code>Write.endTransmisstion</code>.

        When it finds a device prints to the default <code>Serial</code> port a message saying the a device was found on
   a specifc address.

        This function assumes that the <code>Print</code> argument is associated wit a small display (LCD ou OLED).
        So when a <b><code>I2C</code></b> device is found a small message is also printed in the display saying that a
   new device was found.

        \param bus the I2C bus
        \param pr Arduino <code>Print</code> object that suports the <b><code>print</code></b> and
   <b><code>println</code></b> methods
*/
void scanBus(TwoWire& bus, Print* pr, char* busName = NULL, bool showInSerial = false);

#endif  // _UtilsI2C_h_
