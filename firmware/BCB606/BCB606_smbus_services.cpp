/****************************************************************************
 * BCB606 SMBus Services                                                    *
 * Steve Martin                                                            	*
 * June 1, 2023                                                            	*
 ****************************************************************************/
#include "BCB606_smbus_services.h"

static uint16_t SMBUS_read_list_size = 0; // Can be up to 256
static bool     SMBUS_stream_mode = false;
static uint8_t  SMBUS_cmnd_code_list[256] = {0};
/****************************************************************************
 *                                                                          *
 ****************************************************************************/
void BCB606_service_smbus()
{
	if (smbus_services_mailbox.inbox_status == PACKET_PRESENT)
    {
        switch(smbus_services_mailbox.inbox[TRANSACTION_TYPE])
        {
            case SET_REGISTER_LIST:             set_register_list();        break;
            case READ_REGISTER_LIST:            read_register_list();       break;
            case ENABLE_STREAM_MODE:            enable_stream_mode();       break;
            case DISABLE_STREAM_MODE:           disable_stream_mode();      break;
            case WRITE_REGISTER_LIST:           write_register_list();      break;
            case SET_REG_LIST_AND_STREAM:       set_list_and_stream();      break;
            case SET_REG_LIST_AND_READ_LIST:    set_list_and_read_list();   break;
        }
        smbus_services_mailbox.inbox_status = PACKET_ABSENT;
    }
    if (SMBUS_stream_mode)
        SerialUSB.print("Hello     ");
        // read_register_list();
}
/****************************************************************************
 * Populate the Read list                                                   *
 ****************************************************************************/
void set_register_list()
{
    SMBUS_read_list_size = smbus_services_mailbox.inbox_msg_size - START_OF_SMBUS_DATA_IN;
    for (uint16_t cmnd_code=0; cmnd_code < SMBUS_read_list_size; cmnd_code++)   // needs to get as high as 256
        SMBUS_cmnd_code_list[(uint8_t)cmnd_code] = smbus_services_mailbox.inbox[START_OF_SMBUS_DATA_IN + (uint8_t)cmnd_code];
    // SMBUS_addr7 = smbus_services_mailbox.inbox[ADDR7];
}
/****************************************************************************
 * Send out the registers                                                   *
 ****************************************************************************/
void read_register_list()
{
    for (uint16_t cmnd_code=0; cmnd_code < SMBUS_read_list_size; cmnd_code++)   // needs to get as high as 256
        smbus_services_mailbox.outbox[(uint8_t)cmnd_code + START_OF_DATA_OUT] = read_byte(  smbus_services_mailbox.inbox[ADDR7],
                                                                                            SMBUS_cmnd_code_list[(uint8_t)cmnd_code],
                                                                                            smbus_services_mailbox.inbox[USE_PEC],
                                                                                            RETRY_COUNT);
    smbus_services_mailbox.to_id = smbus_services_mailbox.from_id;
    smbus_services_mailbox.outbox_msg_size = SMBUS_read_list_size;
    smbus_services_mailbox.outbox_status = PACKET_PRESENT;
}
/****************************************************************************
 * Toggle stream mode on                                                    *
 ****************************************************************************/
void enable_stream_mode()
{
    SMBUS_stream_mode = true;
}
/****************************************************************************
 * Toggle stream mode off                                                   *
 ****************************************************************************/
void disable_stream_mode()
{
    SMBUS_stream_mode = false;
}
/****************************************************************************
 * Write a bunch of registers to the part                                   *
 ****************************************************************************/
void write_register_list()
{
    // Message is address/data pairs: [ A,D | A,D | A,D | A,D | A,D | A,D | A,D ]
    if (!(smbus_services_mailbox.inbox_msg_size % 2)) // Odd number of address/data pairs? No soup for you!
    {
        for (uint16_t cmnd_code=0; cmnd_code < (smbus_services_mailbox.inbox_msg_size-START_OF_SMBUS_DATA_IN)/2; cmnd_code+=2)
            write_byte( smbus_services_mailbox.inbox[ADDR7],
                        smbus_services_mailbox.inbox[START_OF_SMBUS_DATA_IN + cmnd_code],
                        smbus_services_mailbox.inbox[USE_PEC],
                        RETRY_COUNT,
                        smbus_services_mailbox.inbox[START_OF_SMBUS_DATA_IN + cmnd_code + 1]);
    }
}
/****************************************************************************
 * Populate the read list and retreive one set                              *
 ****************************************************************************/
void set_list_and_read_list()
{
    set_register_list();
    read_register_list();
}
/****************************************************************************
 * Populate the read list and stream continuously                           *
 ****************************************************************************/
void set_list_and_stream()
{
    set_register_list();
    SMBUS_stream_mode = true;
}