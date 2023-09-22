/****************************************************************************
 * BCB606 ID String                                                         *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_IDSTRING_H
#define BCB606_IDSTRING_H
#include <avr/pgmspace.h>

const PROGMEM char IDENTIFICATION[] = "BCB606 Stowe Demo Board REV: 0.0";

#define SCRATCHPAD_SIZE         255
#define ID_STRING_MESSAGE_SIZE  sizeof(IDENTIFICATION) - 1 // Omit the string's null character
#define IDENTIFY_PAYLOAD_SIZE   (SCRATCHPAD_SIZE >= ID_STRING_MESSAGE_SIZE) ? SCRATCHPAD_SIZE : ID_STRING_MESSAGE_SIZE

#endif 