/****************************************************************************
 * BCB606 Soft I²C port                                                     *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_SOFTPORT_B
#define BCB606_SOFTPORT_B

#include <SoftWire.h>
#include "BCB606_board.h"
#include "BCB606_smbus_comnd_struct.h"  // Defines SMBus payload menu structure

void        setup_softport();
void        softport_SMBUS_write_register(uint8_t address, uint8_t command_code, uint8_t size, bool use_pec, uint16_t data);
uint16_t    softport_SMBUS_read_register(uint8_t address, uint8_t command_code, uint8_t size, bool use_pec);
uint8_t     softport_SMBUS_receive_byte(uint8_t address, bool use_pec);

#endif