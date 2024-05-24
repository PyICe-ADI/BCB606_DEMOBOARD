/****************************************************************************
 * BCB606 SMBus Phy                                                         *
 * Steve Martin (From D. Simmons)                                           *
 * June 1, 2023                                                             *
 ****************************************************************************/
 #include "BCB606_smbus.h"
 // Uses the Wire library from Arduino
 // https://www.arduino.cc/reference/en/language/functions/communication/wire/

/****************************************************************************
 * SMBus Read Byte/Word                                                     *
 ****************************************************************************/
SMBUS_reply read_register(uint8_t address, uint8_t command_code, uint8_t use_pec, uint8_t data_size)
{
    uint8_t pec;
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};

    Wire.beginTransmission(address);                                                            // START followed by chip ADDRESS.
    Wire.write(command_code);                                                                   // Clock in the command code.
    Wire.endTransmission(false);                                                                // "false" argument causes a RESTART. Keeps bus active for restart to avoid clearing DUT PEC buffer.
    Wire.requestFrom(address, use_pec ? data_size==WORD_SIZE ? 3 : 2 : data_size==WORD_SIZE ? 2 : 1, true);   // Clocks 2 bytes/words worth of data plus optional PEC byte from the target device. "true" generates a STOP.
    reply.lo_byte = Wire.read();                                                                // Pulls a byte out of the buffer (?)
    if (data_size==WORD_SIZE) reply.hi_byte = Wire.read();                                      // Pulls a byte out of the buffer (?)
    if (use_pec) pec = Wire.read();                                                             // Pulls another byte out of the buffer (?)
    delayMicroseconds(50);                                                                      // Put a modicum of air between repeated transactions for scope debug.
    // if (use_pec && pec_read_byte_test(address, command_code, reply.LSB, pec) == 0) TODO TEST PEC BYTE FOR CORRECTNESS
    return reply;
}
/****************************************************************************
 * SMBus Write Byte/Word                                                    *
 ****************************************************************************/
SMBUS_reply write_register(uint8_t address, uint8_t command_code, uint8_t use_pec, uint8_t data_size, uint8_t lo_byte, uint8_t hi_byte)
{
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};

    Wire.beginTransmission(address);
    Wire.write(command_code);
    Wire.write(lo_byte);
    if (data_size==WORD_SIZE) Wire.write(hi_byte);
    if (use_pec) Wire.write(pec_write_byte(address, command_code, lo_byte)); // TODO make support PEC for Write Word too !!!!
    Wire.endTransmission(true);
    return reply;
}