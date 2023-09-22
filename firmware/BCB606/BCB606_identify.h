/****************************************************************************
 * BCB606 Identify Onseself                                                 *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_IDENTIFY_B
#define BCB606_IDENTIFY_B

#include "BCB606_postoffice.h"  // Gets Mailbox
#include "BCB606_idstring.h"    // Gets Identify message and sizes

void BCB606_identify();
void send_identity();
void write_scratchpad();
void read_scratchpad();
void get_serialnum();

#endif