/****************************************************************************
 * BCB606 Stowe EEPROM                                                      *
 * Steve Martin                                                            	*
 * June 1, 2023                                                            	*
 ****************************************************************************/
#include "BCB606_eeprom.h"

/****************************************************************************
 * Set the pin state                                                        *
 ****************************************************************************/
void BCB606_eeprom_services()
{
	if (eeprom_mailbox.inbox_status == PACKET_PRESENT)
    {
        switch(eeprom_mailbox.inbox[TRANSACTION_TYPE])
        {
            case SMBUS_WRITE_REGISTER:
                 softport_SMBUS_write_register( eeprom_mailbox.inbox[ADDR7],
                                                eeprom_mailbox.inbox[COMMAND_CODE],
                                                eeprom_mailbox.inbox[DATA_SIZE],
                                                eeprom_mailbox.inbox[USE_PEC],
                                                eeprom_mailbox.inbox[START_OF_SMBUS_DATA_IN] << 8 | eeprom_mailbox.inbox[START_OF_SMBUS_DATA_IN + 1]);
                 break;
            case SMBUS_RECEIVE_BYTE:
                 eeprom_mailbox.to_id = eeprom_mailbox.from_id;
                 eeprom_mailbox.outbox_msg_size = EEPROM_OUTBOX_SIZE;
                 eeprom_mailbox.outbox[START_OF_DATA_OUT] = softport_SMBUS_receive_byte(eeprom_mailbox.inbox[ADDR7],
                                                                                        eeprom_mailbox.inbox[USE_PEC]);
                 eeprom_mailbox.outbox_status = PACKET_PRESENT;
                 break;
        }
        eeprom_mailbox.inbox_status = PACKET_ABSENT;
    }
}