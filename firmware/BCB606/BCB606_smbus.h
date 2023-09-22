/****************************************************************************
 * BCB606 SMBus Phy                                                         *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_SMBUS_H
#define BCB606_SMBUS_H

#include <Wire.h>
#include <Arduino.h>        // Needed for delayMicroseconds()
#include "stowe_pec.h"

uint8_t read_byte(uint8_t address, uint8_t command_code, uint8_t use_pec, uint8_t retry_count);
uint8_t write_byte(uint8_t address, uint8_t command_code, uint8_t use_pec, uint8_t retry_count, uint8_t data);

#define READ_BF_8BIT_REG(command_code, offset, mask) ((read_byte_with_pec(LT3390_ADDR, command_code, 2) & mask) >> offset)
#define WRITE_BF_8BIT_REG(command_code, offset, mask, value) ((mask == 0xFF) ? (write_byte_with_pec(LT3390_ADDR, command_code, value, 2)) : (write_byte_with_pec(LT3390_ADDR, command_code, ((read_byte_with_pec(LT3390_ADDR, command_code, 2) & ~mask) | ((value << offset) & mask)), 2)))

#endif