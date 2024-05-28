/****************************************************************************
 * BCB606 Stowe Watchdog                                                    *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_WATCHDOG_B
#define BCB606_WATCHDOG_B

#include <avr/pgmspace.h>           // For Watchdog table storage
#include <Arduino.h>                // For micros()
#include <Wire.h>                   // For I²C access
#include "BCB606_postoffice.h"      // Gets Mailbox
#include "BCB606_board.h"           // For RST pin location
#include "BCB606_smbus.h"           // For read byte and write byte commands

// Payload Commands
#define SET_SERVICE_STATE           0
#define GET_SERVICE_STATE           1
#define SET_RESPONSE_TIME_us        2 // Should be followed by 4 bytes for the micros() 32 bit value. Up to 2^32 µs or 71.58 minutes.
#define GET_RESPONSE_TIME_us        3 // Returns 4 bytes
#define SET_ADDR7                   4
#define GET_ADDR7                   5
#define SET_SERVICE_METHOD          6
#define GET_SERVICE_METHOD          7
#define SET_USE_PEC                 8
#define GET_USE_PEC                 9
#define SET_QUESTION_ADDR           10
#define GET_QUESTION_ADDR           11
#define SET_ANSWER_ADDR             12
#define GET_ANSWER_ADDR             13
#define SET_CRC_POLY                14
#define GET_CRC_POLY                15

// Byte location enums
#define COMMAND_BYTE                0
#define START_OF_DATA_OUT_BYTE      0
#define START_OF_DATA_IN_BYTE       1

// Method ENUMS
#define USE_LOOKUP_TABLE            0
#define USE_ALGORITHMIC             1

uint8_t compute_watchdog_answer(uint8_t);
void BCB606_watchdog_services();
void service_watchdog();
void set_response_time_us();
void get_response_time_us();
void set_service_method();
void get_service_method();
void set_service_state();
void get_service_state();
void set_question_addr();
void get_question_addr();
void set_answer_addr();
void get_answer_addr();
void set_crc_poly();
void get_crc_poly();
void set_use_pec();
void get_use_pec();
void set_addr7();
void get_addr7();

#endif 