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

uint8_t compute_watchdog_answer(uint8_t);
void BCB606_watchdog_services();
void set_wd_response_time_us();
void get_wd_response_time_us();
void set_method_lookup_table();
void set_method_algorithmic();
void get_wd_service_state();
void get_service_method();
void disable_wd_service();
void enable_wd_service();
void service_watchdog();
void set_addr7();

#endif 