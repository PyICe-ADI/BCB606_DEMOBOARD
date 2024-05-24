/****************************************************************************
 * BCB606 Soft I²C port                                                     *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
//  https://www.arduino.cc/reference/en/libraries/softwire/
//  https://github.com/stevemarple/SoftWire/blob/master/examples/SoftWire_MLX90614/SoftWire_MLX90614.ino
//  https://github.com/stevemarple/SoftWire/blob/master/examples/ReadDS1307/ReadDS1307.ino
 
#include "BCB606_softport.h"

SoftWire softport(AUX_SDA, AUX_SCL);
char spTxBuffer[2]; // These buffers must be at least as large as the largest read or write you perform.
char spRxBuffer[1];
AsyncDelay readInterval;

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
SMBUS_reply softport_SMBUS_write_register(uint8_t address, uint8_t command_code, bool use_pec, uint8_t data_size, uint8_t lo_byte, uint8_t hi_byte)
{
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};

    softport.startWrite(address);
    softport.llWrite(command_code);
    softport.llWrite(lo_byte);
    if (data_size == WORD_SIZE)
        softport.llWrite(hi_byte);
    softport.stop();
    return reply;
}
/****************************************************************************
 * Read Register, flexible size, 8 or 16 so SMBus Read-Byte or Read-Word    *
 ****************************************************************************/
SMBUS_reply softport_SMBUS_read_register(uint8_t address, uint8_t command_code, bool use_pec, uint8_t data_size)
{
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};

    softport.startWrite(address);
    softport.llWrite(command_code);
    softport.stop();
    softport.startRead(address);
    if (data_size == WORD_SIZE)
    {
        softport.readThenAck(reply.lo_byte);
        softport.readThenNack(reply.hi_byte);
    }
    else
        softport.readThenNack(reply.lo_byte);
    softport.stop();
    return reply;
}
/****************************************************************************
 * SMBus Receive Byte                                                       *
 ****************************************************************************/
SMBUS_reply softport_SMBUS_receive_byte(uint8_t address, bool use_pec)
{
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};

    softport.startRead(address);
    softport.readThenNack(reply.lo_byte);
    softport.stop();
    return reply;
}