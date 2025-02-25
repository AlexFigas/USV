# UtilsBoards

### Carlos Gonçalves <cajocasa@gmail.com>
#### Version 1.0.2
<br/>
  
This library provides utility functions and macros that aim to uniform the usage of the WiFi and I2C across different boards.

Despite the ESP32 and ESP8266 boards have support for WiFi, the names of the functions used are not equal across different boards. So this library offers a small set of functions, and some utility macros, that can be used to initialize the WiFi using the same code in different boards.

The library also offers a function to scan the I2C bus for devices.