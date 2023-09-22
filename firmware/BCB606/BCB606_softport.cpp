/****************************************************************************
 * BCB606 Soft I²C port                                                     *
 * Steve Martin                                                            	*
 * June 1, 2023                                                            	*
 ****************************************************************************/
//  https://www.arduino.cc/reference/en/libraries/softwire/
//  https://github.com/stevemarple/SoftWire/blob/master/examples/SoftWire_MLX90614/SoftWire_MLX90614.ino
//  https://github.com/stevemarple/SoftWire/blob/master/examples/ReadDS1307/ReadDS1307.ino
 
#include "BCB606_softport.h"

SoftWire softport(AUX_SDA, AUX_SCL);
char spTxBuffer[2]; // These buffers must be at least as large as the largest read or write you perform.
char spRxBuffer[1];
AsyncDelay readInterval;

#define BYTE_SIZE 8     // Bits
#define WORD_SIZE 16    // Bits

/****************************************************************************
 * Setup the softwire port                                                  *
 ****************************************************************************/
void setup_softport()
{
  softport.setTxBuffer(spTxBuffer, sizeof(spTxBuffer));
  softport.setRxBuffer(spRxBuffer, sizeof(spRxBuffer));
  softport.setDelay_us(5);
  softport.setTimeout(1000);
  softport.begin();
  readInterval.start(2000, AsyncDelay::MILLIS);
}
/****************************************************************************
 * Write Register, flexible size, 8 or 16 so SMBus Write-Byte or Write-Word *
 ****************************************************************************/
void softport_SMBUS_write_register(uint8_t address, uint8_t command_code, uint8_t size, bool use_pec, uint16_t data)
{
    softport.startWrite(address);
    softport.llWrite(command_code);
    softport.llWrite(lowByte(data));
    if (size == WORD_SIZE) softport.llWrite(highByte(data));
    softport.stop();
}
/****************************************************************************
 * Read Register, flexible size, 8 or 16 so SMBus Read-Byte or Read-Word    *
 ****************************************************************************/
uint16_t softport_SMBUS_read_register(uint8_t address, uint8_t command_code, uint8_t size, bool use_pec)
{
    uint8_t hibyte;
    uint8_t lobyte;
    softport.startWrite(address);
    softport.llWrite(command_code);
    softport.stop();
    softport.startRead(address);
    if (size == WORD_SIZE)
    {
        softport.readThenAck(lobyte);
        softport.readThenNack(hibyte);
    }
    else
        softport.readThenNack(lobyte);
    softport.stop();
    return (hibyte << BYTE_SIZE) | lobyte;
}
/****************************************************************************
 * SMBus Receive Byte                                                       *
 ****************************************************************************/
uint8_t softport_SMBUS_receive_byte(uint8_t address, bool use_pec)
{
    uint8_t data;
    softport.startRead(address);
    softport.readThenNack(data);
    softport.stop();
    return data;
}