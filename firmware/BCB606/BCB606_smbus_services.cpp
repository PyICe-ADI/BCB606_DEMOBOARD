/****************************************************************************
 * BCB606 SMBus Services                                                    *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
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
            case SET_REGISTER_LIST:             set_register_list();            break;
            case READ_REGISTER_LIST:            read_register_list();           break;
            case ENABLE_STREAM_MODE:            enable_stream_mode();           break;
            case SMBUS_READ_REGISTER:           smbus_read_register();          break;
            case DISABLE_STREAM_MODE:           disable_stream_mode();          break;
            case WRITE_REGISTER_LIST:           write_register_list();          break;
            case SMBUS_WRITE_REGISTER:          smbus_write_register();         break;
            case SET_REG_LIST_AND_STREAM:       set_reg_list_and_stream();      break;
            case SET_REG_LIST_AND_READ_LIST:    set_reg_list_and_read_list();   break;
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
 * Retrieve and Send out the registers                                      *
 ****************************************************************************/
void read_register_list()
{
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};
    for (uint16_t cmnd_code=0; cmnd_code < SMBUS_read_list_size; cmnd_code++)   // needs to get as high as 256
    {
        reply = read_register(  smbus_services_mailbox.inbox[ADDR7],
                                SMBUS_cmnd_code_list[(uint8_t)cmnd_code],
                                smbus_services_mailbox.inbox[USE_PEC],
                                smbus_services_mailbox.inbox[DATA_SIZE]);
        smbus_services_mailbox.outbox[(uint8_t)cmnd_code + START_OF_DATA_OUT] = reply.lo_byte;  // TODO make support Word parts too?
                                                                                                // TODO Make a new reply mailbox and send the reply.status to it?
    }
    smbus_services_mailbox.to_id = smbus_services_mailbox.from_id;
    smbus_services_mailbox.outbox_msg_size = SMBUS_read_list_size;
    smbus_services_mailbox.outbox_status = PACKET_PRESENT;
}
/****************************************************************************
 * SMBus Retrieve and Send out a single register                            *
 ****************************************************************************/
void smbus_read_register()
{
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};
    reply = read_register(  smbus_services_mailbox.inbox[ADDR7],
                            smbus_services_mailbox.inbox[COMMAND_CODE],
                            smbus_services_mailbox.inbox[USE_PEC],
                            smbus_services_mailbox.inbox[DATA_SIZE]);
    smbus_services_mailbox.outbox[START_OF_DATA_OUT] = reply.lo_byte;
    if (smbus_services_mailbox.inbox[DATA_SIZE]==WORD_SIZE)
        smbus_services_mailbox.outbox[START_OF_DATA_OUT + 1] = reply.hi_byte;
    smbus_services_mailbox.to_id = smbus_services_mailbox.from_id;
    smbus_services_mailbox.outbox_msg_size = smbus_services_mailbox.inbox[DATA_SIZE]/BYTE_SIZE;
    smbus_services_mailbox.outbox_status = PACKET_PRESENT;
}
/****************************************************************************
 * Write a single register of the part                                      *
 ****************************************************************************/
void smbus_write_register()
{
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};
    reply = write_register( smbus_services_mailbox.inbox[ADDR7],
                            smbus_services_mailbox.inbox[COMMAND_CODE],
                            smbus_services_mailbox.inbox[USE_PEC],
                            smbus_services_mailbox.inbox[DATA_SIZE],
                            smbus_services_mailbox.inbox[START_OF_SMBUS_DATA_IN],
                            smbus_services_mailbox.inbox[DATA_SIZE]==BYTE_SIZE ? 0 : smbus_services_mailbox.inbox[START_OF_SMBUS_DATA_IN + 1]);
}
/****************************************************************************
 * Write a bunch of registers to the part                                   *
 ****************************************************************************/
void write_register_list()
{
    SMBUS_reply reply={.status=0, .lo_byte=0, .hi_byte=0};
    // Message is address/data pairs: [ A,D | A,D | A,D | A,D | A,D | A,D | A,D ]
    if (!(smbus_services_mailbox.inbox_msg_size % 2)) // Odd number of address/data pairs? No soup for you!
    {
        for (uint16_t cmnd_code=0; cmnd_code < (smbus_services_mailbox.inbox_msg_size-START_OF_SMBUS_DATA_IN)/2; cmnd_code+=2)
            reply = write_register( smbus_services_mailbox.inbox[ADDR7],
                                    smbus_services_mailbox.inbox[START_OF_SMBUS_DATA_IN + cmnd_code],
                                    smbus_services_mailbox.inbox[USE_PEC],
                                    smbus_services_mailbox.inbox[DATA_SIZE],
                                    smbus_services_mailbox.inbox[START_OF_SMBUS_DATA_IN + cmnd_code + 1],
                                    smbus_services_mailbox.inbox[DATA_SIZE]==BYTE_SIZE ? 0 : smbus_services_mailbox.inbox[START_OF_SMBUS_DATA_IN + cmnd_code + 2]);
    }
}
/****************************************************************************
 * Populate the read list and retreive one set                              *
 ****************************************************************************/
void set_reg_list_and_read_list()
{
    set_register_list();
    read_register_list();
}
/****************************************************************************
 * Populate the read list and stream continuously                           *
 ****************************************************************************/
void set_reg_list_and_stream()
{
    set_register_list();
    SMBUS_stream_mode = true;
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