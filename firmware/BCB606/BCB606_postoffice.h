/****************************************************************************
 * BCB606 Post Office                                                       *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#ifndef BCB606_POSTOFFICE_H
#define BCB606_POSTOFFICE_H

#include "labcomm.h"                    // Gets incoming_packet and MAX_BUFFER_SIZE_MESSAGE_SIZE
#include "BCB606_idstring.h"            // Gets IDENTIFICATION string
#include "BCB606_smbus_comnd_struct.h"  // Gets start of SMBus register as proxy for command size.

/****************************************************************************
 * Mail Box Addresses                                                       *
 ****************************************************************************/
#define PyICe_GUI                   1
#define ENABLE_PIN_ADDRESS          2
#define RST_PIN_ADDRESS             3
#define WD_DIS_PIN_ADDRESS          4
#define SMBUS_SERVICES_ADDRESS      5
#define IDENTIFY_ADDRESS            6
#define WATCHDOG_ADDRESS            7
#define EEPROM_ADDRESS              8
#define TMP117_ADDRESS              9

/****************************************************************************
 * Mail Box Sizes                                                           *
 ****************************************************************************/
#define LABCOMM_INBOX_SIZE          0
#define LABCOMM_OUTBOX_SIZE         MAX_BUFFER_SIZE_MESSAGE_SIZE

#define ID_INBOX_SIZE               1 + IDENTIFY_PAYLOAD_SIZE
#define ID_OUTBOX_SIZE              IDENTIFY_PAYLOAD_SIZE

#define SMBUS_INBOX_SIZE            START_OF_SMBUS_DATA_IN + 2*256 // Write list will have: [REG|DATA|REG|DATA....]
#define SMBUS_OUTBOX_SIZE           256

#define WDDIS_PIN_INBOX_SIZE        2
#define WDDIS_PIN_OUTBOX_SIZE       1

#define ENABLE_PIN_INBOX_SIZE       2
#define ENABLE_PIN_OUTBOX_SIZE      1

#define RST_PIN_INBOX_SIZE          0
#define RST_PIN_OUTBOX_SIZE         1

#define WATCHDOG_INBOX_SIZE         5
#define WATCHDOG_OUTBOX_SIZE        4

#define EEPROM_INBOX_SIZE           START_OF_SMBUS_DATA_IN + 2 // Must support Write Word
#define EEPROM_OUTBOX_SIZE          1

#define TMP117_INBOX_SIZE           START_OF_SMBUS_DATA_IN + 2 // Must support Write Word
#define TMP117_OUTBOX_SIZE          2

/****************************************************************************
 * Mailbox Externs for the clients to find                                  *
 ****************************************************************************/
extern Mailbox<ID_INBOX_SIZE,           ID_OUTBOX_SIZE>             identify_mailbox;
extern Mailbox<SMBUS_INBOX_SIZE,        SMBUS_OUTBOX_SIZE>          smbus_services_mailbox;
extern Mailbox<WDDIS_PIN_INBOX_SIZE,    WDDIS_PIN_OUTBOX_SIZE>      wd_dis_pin_mailbox;
extern Mailbox<ENABLE_PIN_INBOX_SIZE,   ENABLE_PIN_OUTBOX_SIZE>     enable_pin_mailbox;
extern Mailbox<RST_PIN_INBOX_SIZE,      RST_PIN_OUTBOX_SIZE>        rst_pin_mailbox;
extern Mailbox<WATCHDOG_INBOX_SIZE,     WATCHDOG_OUTBOX_SIZE>       watchdog_mailbox;
extern Mailbox<EEPROM_INBOX_SIZE,       EEPROM_OUTBOX_SIZE>         eeprom_mailbox;
extern Mailbox<TMP117_INBOX_SIZE,       TMP117_OUTBOX_SIZE>         tmp117_mailbox;

void BCB606_process_mail();

#endif