#ifndef MyPZEM_h
#define MyPZEM_h

#include <SoftwareSerial.h>
#include <stdio.h>
#include <Utils.h>

typedef struct {
  float voltage;
  float current;
  float power;
  float energy;
  float frequency;
  float powerFactor;
  uint16_t alarms;
} ValuesPZM, * PtrValuesPZM;

class PZEM {
  private:
    // Addresses of PZEM registers
    const uint16_t REG_VOLTAGE   = 0x0000;
    const uint16_t REG_CURRENT_L = 0x0001;
    const uint16_t REG_CURRENT_H = 0X0002;
    const uint16_t REG_POWER_L   = 0x0003;
    const uint16_t REG_POWER_H   = 0x0004;
    const uint16_t REG_ENERGY_L  = 0x0005;
    const uint16_t REG_ENERGY_H  = 0x0006;
    const uint16_t REG_FREQUENCY = 0x0007;
    const uint16_t REG_PF        = 0x0008;
    const uint16_t REG_ALARM     = 0x0009;

    // Commands that can be send to the PZEM
    const uint8_t CMD_RHR  = 0x03;
    const uint8_t CMD_RIR  = 0X04;
    const uint8_t CMD_WSR  = 0x06;
    const uint8_t CMD_CAL  = 0x41;
    const uint8_t CMD_REST = 0x42;

    // Power alarms
    const uint16_t WREG_ALARM_THR = 0x0001;
    const uint16_t WREG_ADDR      = 0x0002;

    // Communication parameters
    const int RESPONSE_SIZE = 32;
    const int READ_TIMEOUT  = 100;

    const int DefaultUpdateTime = 200;
    int updateTime;

    // Measured values
    ValuesPZM currentValues;

    // Invalid values;
    ValuesPZM invalidValues;

    // Serial interface
    Stream* serial;

    // Is serial interface software
    bool isSerialSoftware;

    // Device address
    uint8_t address;

    // Last time values were updated
    uint64_t timeForLastRead;

    // Software serial port 
    SoftwareSerial* port;

    /*
     * PZEM::sendCmd8
     * 
     * Prepares the 8 byte command buffer and sends
     * 
     * @param[in] cmd - Command to send (position 1)
     * @param[in] rAddr - Register address (postion 2-3)
     * @param[in] val - Register value to write (positon 4-5)
     * @param[in] check - perform a simple read check after write
     * 
     * @return success
     */
    bool sendCmd8(uint8_t cmd, uint16_t rAddr, uint16_t val, bool check, uint16_t slave_addr=0);

    /*
     * PZEM::init
     * 
     * Initialization common to all consturctors
     * 
     * @param[in] addr - device address
     *
     * @return success
     */
    void init(uint8_t addr);

    /*
     * PZEM::receive
     * 
     * Receive data from serial with buffer limit and timeout
     * 
     * @param[out] resp Memory buffer to hold response. Must be at least `len` long
     * @param[in] len Max number of bytes to read
     *
     * @return number of bytes read
     */
    uint16_t receive(uint8_t *resp, uint16_t len);

    /*
     * PZEM::checkCRC
     * 
     * Performs CRC check of the buffer up to len-2 and compares check sum to last two bytes
     * 
     * @param[in] data Memory buffer containing the frame to check
     * @param[in] len  Length of the respBuffer including 2 bytes for CRC
     * 
     * @return is the buffer check sum valid
     */
    bool checkCRC(const uint8_t *buf, uint16_t len);

    /*
     * PZEM::setCRC
     * 
     * Set last two bytes of buffer to CRC16 of the buffer up to byte len-2
     * Buffer must be able to hold at least 3 bytes
     * 
     * @param[out] data Memory buffer containing the frame to checksum and write CRC to
     * @param[in] len  Length of the respBuffer including 2 bytes for CRC
     *
     */
    void setCRC(uint8_t *buf, uint16_t len);

    /*
     * PZEM::CRC16
     * 
     * Calculate the CRC16-Modbus for a buffer
     * 
     * Based on https://www.modbustools.com/modbus_crc16.html
     * 
     * @param[in] data Memory buffer containing the data to checksum
     * @param[in] len  Length of the respBuffer
     * 
     * @return Calculated CRC
     */
    uint16_t CRC16(const uint8_t *data, uint16_t len);

  public:
    static const int BaudRatePZEM = 9600;
    
    static const uint8_t DefaultAddressPZEM = 0xF8;

    /*
     * PZEM::PZEM
     * 
     * Constructor using a Software Serial Port
     * 
     * @param receivePin RX pin
     * @param transmitPin TX pin
     * @param addr Slave address of device
     */
    PZEM(uint8_t receivePin, uint8_t transmitPin, uint8_t addr=DefaultAddressPZEM);

    /*
     * PZEM::PZEM
     * 
     * Constructor using a Hardware Serial Port
     * @param port Hardware serial to use
     * @param receivePin RX pin
     * @param transmitPin TX pin
     * @param addr Slave address of device
     */
    PZEM(HardwareSerial* port, uint8_t receivePin, uint8_t transmitPin, uint8_t addr=DefaultAddressPZEM);

    /*
     * PZEM::~PZEM 
     *
     * Destructor deleting software serial
     */
    ~PZEM();

    /*
     * PZEM::updateValues
     * 
     * Read all registers of device and update the local values
     * 
     * @return success
     */
    bool updateValues();

    /*
     * PZEM::voltage
     * 
     * Get line voltage in V
     * 
     * @return current L-N voltage
     */
    float voltage();

    /*
     * PZEM::current
     * 
     * Get line current in A
     * 
     * @return current L-N current
     */
    float current();

    /*
     * PZEM::power
     * 
     * Get active power in W
     * 
     * @return active power
     */
    float power();

    /*
     * PZEM::energy
     * 
     * Get Active energy in kWh since last reset
     * 
     * @return active energy in kWh
     */
    float energy();

    /*
     * PZEM::frequency
     * 
     * Get current line frequency in Hz
     * 
     * @return line frequency in Hz
     */
    float frequency();

    /*
     * PZEM::powerFactor
     * 
     * Get power factor of the load
     * 
     * @return load power factor
     */
    float powerFactor();

    /*
     * PZEM::values
     * 
     * Get all the values
     * 
     * @return all the values related to the PZEM
     */
    PtrValuesPZM getValues();

    /*
     * PZEM::setPowerAlarm
     * 
     * Set power alarm threshold in watts
     * 
     * @param[in] watts Alamr theshold
     * 
     * @return success
     */
    bool setPowerAlarm(uint16_t watts);

    /*
     * PZEM::istPowerAlarm
     * 
     * Is the power alarm set
     * 
     * @return alarm triggerd
     */
    bool isPowerAlarm();

    /*
     * PZEM::setAddress
     * 
     * Set a new device address and update the device
     * 
     * WARNING - should be used to set up devices once.
     *  
     * Code initializtion will still have old address on next run!
     * 
     * @param[in] addr New device address 0x01-0xF7
     * 
     * @return success
     */
    bool setAddress(uint8_t addr);

    /*
     * PZEM::getAddress
     * 
     * Get the current device address
     * 
     * @return address
     */
    uint8_t getAddress();

    /*
     * PZEM::setUpdateTime
     * 
     * Set a the time inteval for updating the readings from the PZEM
     * 
     * @param[in] newUpdateTime The new value to be used as the update time
     */
    void setUpdateTime(int newUpdateTime);

    /*
     * PZEM::getUpdateTime
     * 
     * Get the current update time
     * 
     * @return updateTime
     */
    int getUpdateTime();

    /*
     * PZEM::resetEnergy
     * 
     * Reset the Energy counter on the device
     * 
     * @return success
     */
    bool resetEnergy();

    /*
     * PZEM::search
     * 
     * Search for available devices. This should be used only for debugging!
     * 
     * Prints any found device addresses on the bus.
     *  
     */
    void search();
};

typedef PZEM* PtrPZEM;

#endif	// MyPZEM_h
