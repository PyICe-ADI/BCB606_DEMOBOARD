/****************************************************************************
 * BCB606 SMBus Phy                                                         *
 * Steve Martin (From D. Simmons)                                           *
 * June 1, 2023                                                             *
 ****************************************************************************/
#include "BCB606_smbus.h"

//                    TODO   !!!    Convert these to read_register and write_register to support 8 and 16 bit
//                    TODO   !!!    Convert these to read_register and write_register to support 8 and 16 bit
//                    TODO   !!!    Convert these to read_register and write_register to support 8 and 16 bit
//                    TODO   !!!    Convert these to read_register and write_register to support 8 and 16 bit
//                    TODO   !!!    Convert these to read_register and write_register to support 8 and 16 bit
//                    TODO   !!!    Convert these to read_register and write_register to support 8 and 16 bit
//                    TODO   !!!    Convert these to read_register and write_register to support 8 and 16 bit

/****************************************************************************
 * Read Byte															    *
 ****************************************************************************/
uint8_t read_byte(uint8_t address, uint8_t command_code, uint8_t use_pec, uint8_t retry_count)
{
    Wire.beginTransmission(address);
    Wire.write(command_code);
    Wire.endTransmission(false); //keep bus active for restart to avoid clearing DUT PEC buffer
    // uint8_t byte_count = Wire.requestFrom(address, 2);
    Wire.requestFrom(address, 2);
    //if (command_buffer[2] < current_i2c_port->available()) // check to be sure correct number of bytes were returned by slave
    uint8_t end_return = Wire.endTransmission(true);
    uint8_t data = Wire.read();
    uint8_t pec = Wire.read();

    delayMicroseconds(50); // Put a modicum air between repeated transactions for scope debug

    if (end_return == 0) {
    if (pec_read_byte_test(address, command_code, data, pec) == 0) {
      return data;
    } else {
      if (retry_count) {
        return read_byte(address, command_code, use_pec, retry_count - 1);
      } else return -1;
    }
    } else {
    if (retry_count) {
      return read_byte(address, command_code, use_pec, retry_count - 1);
    } else return -1;
    }
}
/****************************************************************************
 * Write Byte															    *
 ****************************************************************************/
uint8_t write_byte(uint8_t address, uint8_t command_code, uint8_t use_pec, uint8_t retry_count, uint8_t data)
{
    Wire.beginTransmission(address);
    Wire.write(command_code);
    Wire.write(data);
    Wire.write(pec_write_byte(address, command_code, data));
    uint8_t end_return = Wire.endTransmission(true);

    if (end_return == 0) {
    return 0;
    } else if (retry_count) {
    return write_byte(address, command_code, use_pec, retry_count - 1, data);
    } else return -1;
}