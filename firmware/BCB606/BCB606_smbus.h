/****************************************************************************
 * BCB606 SMBus Phy                                                         *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_SMBUS_H
#define BCB606_SMBUS_H

#include <Wire.h>
#include <Arduino.h>    // Needed for delayMicroseconds()
#include "stowe_pec.h"

#define BYTE_SIZE 8     // Bits
#define WORD_SIZE 16    // Bits

typedef struct {
        uint8_t status;
        uint8_t lo_byte;
        uint8_t hi_byte;
}       SMBUS_reply;

SMBUS_reply read_register(uint8_t address, uint8_t command_code, uint8_t use_pec, uint8_t data_size);
SMBUS_reply write_register(uint8_t address, uint8_t command_code, uint8_t use_pec, uint8_t data_size, uint8_t lo_byte, uint8_t hi_byte);

#endif