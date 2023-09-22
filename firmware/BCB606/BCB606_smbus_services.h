/****************************************************************************
 * BCB606 SMBus Services                                                    *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_SMBUS_SERVICES_H
#define BCB606_SMBUS_SERVICES_H

#include "BCB606_postoffice.h"          // Gets Mailbox
#include "BCB606_smbus_comnd_struct.h"  // Defines SMBus payload menu structure
#include "BCB606_smbus.h"

void set_register_list();
void read_register_list();
void enable_stream_mode(); 
void disable_stream_mode();
void write_register_list();
void set_list_and_stream();
void BCB606_service_smbus();
void set_list_and_read_list();  

#endif