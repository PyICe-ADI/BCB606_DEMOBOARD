/****************************************************************************
 * BCB606 Stowe Temperature Sensor                                          *
 * Steve Martin                                                            	*
 * June 1, 2023                                                            	*
 ****************************************************************************/
#include "BCB606_temp_sensor.h"

/****************************************************************************
 * Set the pin state                                                        *
 ****************************************************************************/
void BCB606_get_temperature_sensor()
{
    uint16_t data;
	if (tmp117_mailbox.inbox_status == PACKET_PRESENT)
    {
        switch(tmp117_mailbox.inbox[TRANSACTION_TYPE])
        {
            case SMBUS_WRITE_REGISTER:
                softport_SMBUS_write_register(  tmp117_mailbox.inbox[ADDR7],
                                                tmp117_mailbox.inbox[COMMAND_CODE],
                                                tmp117_mailbox.inbox[DATA_SIZE],
                                                tmp117_mailbox.inbox[USE_PEC],
                                                tmp117_mailbox.inbox[START_OF_SMBUS_DATA_IN] << 8 |
                                                tmp117_mailbox.inbox[START_OF_SMBUS_DATA_IN+1]);
                break;
            case SMBUS_READ_REGISTER:
                data = softport_SMBUS_read_register(tmp117_mailbox.inbox[ADDR7],
                                                    tmp117_mailbox.inbox[COMMAND_CODE],
                                                    tmp117_mailbox.inbox[DATA_SIZE],
                                                    tmp117_mailbox.inbox[USE_PEC]);
                tmp117_mailbox.to_id = tmp117_mailbox.from_id;
                tmp117_mailbox.outbox_msg_size = TMP117_OUTBOX_SIZE;
                tmp117_mailbox.outbox[START_OF_DATA_OUT + 1] = data >> 8;
                tmp117_mailbox.outbox[START_OF_DATA_OUT]     = data & 0xff;
                tmp117_mailbox.outbox_status = PACKET_PRESENT;
                break;
        }
        tmp117_mailbox.inbox_status = PACKET_ABSENT;
    }
}