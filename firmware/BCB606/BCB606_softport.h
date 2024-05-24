/****************************************************************************
 * BCB606 Soft I²C port                                                     *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_SOFTPORT_B
#define BCB606_SOFTPORT_B

#include <SoftWire.h>
#include "BCB606_board.h"
#include "BCB606_smbus.h"               // Gets the return data structure

void setup_softport();
SMBUS_reply softport_SMBUS_write_register(uint8_t address, uint8_t command_code, bool use_pec, uint8_t data_size, uint8_t lo_byte, uint8_t hi_byte);
SMBUS_reply softport_SMBUS_read_register(uint8_t address, uint8_t command_code, bool use_pec, uint8_t data_size);
SMBUS_reply softport_SMBUS_receive_byte(uint8_t address, bool use_pec);

#endif