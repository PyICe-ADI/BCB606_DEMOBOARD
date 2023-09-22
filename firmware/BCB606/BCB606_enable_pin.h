/****************************************************************************
 * BCB606 Stowe ENABLE Pin                                                  *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_ENABLE_PIN
#define BCB606_ENABLE_PIN

#include "labcomm.h"            // Gets mailbox ENUMS and types
#include "BCB606_postoffice.h"  // Gets inbox and outbox sizes
#include "BCB606_board.h"

void BCB606_set_enable_pin_state();
void BCB606_get_enable_pin_state();

#endif