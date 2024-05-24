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
#define SET_WD_RESPONSE_TIME_us     1 // Should be followed by 4 bytes for the micros() 32 bit value.
#define ENABLE_WD_SERVICE           2 // Up to 2^32 µs or 71.58 minutes.
#define DISABLE_WD_SERVICE          3
#define GET_WD_RESPONSE_TIME_us     4
#define GET_WD_SERVICE_STATE        5
#define SET_ADDR7                   6
#define SET_METHOD_LOOKUP_TABLE     7
#define SET_METHOD_ALGORITHMIC      8
#define GET_SERVICE_METHOD          9
#define SET_USE_PEC                 10
#define SET_QUESTION_ADDR           11
#define SET_ANSWER_ADDR             12
#define SET_CRC_POLY                13

// Byte location enums
#define COMMAND_BYTE                0
#define START_OF_DATA_OUT_BYTE      0
#define START_OF_DATA_IN_BYTE       1

// Others
#define USE_LOOKUP_TABLE            0
#define USE_ALGORITHMIC             1
#define CRC_POLY                    0b100000111 // TODO should be programmable

uint8_t compute_watchdog_answer(uint8_t);
void BCB606_watchdog_services();
void set_wd_response_time_us();
void get_wd_response_time_us();
void set_method_lookup_table();
void set_method_algorithmic();
void get_wd_service_state();
void get_service_method();
void disable_wd_service();
void set_question_addr();
void enable_wd_service();
void service_watchdog();
void set_answer_addr();
void set_crc_poly();
void set_use_pec();
void set_addr7();

#endif 