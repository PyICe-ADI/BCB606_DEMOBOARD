/****************************************************************************
 * BCB606 Post Office                                                       *
 * Steve Martin                                                             *
 * June 1, 2023                                                             *
 ****************************************************************************/
#include "BCB606_postoffice.h"

/****************************************************************************
 * Create Mailboxes                                                         *
 ****************************************************************************/
Mailbox<ID_INBOX_SIZE, ID_OUTBOX_SIZE>                  identify_mailbox        = {.my_id=IDENTIFY_ADDRESS,         .to_id=0, .from_id=0, .inbox_msg_size=0, .outbox_msg_size=0, .inbox_status=PACKET_ABSENT, .outbox_status=PACKET_ABSENT, .inbox_overflow=false, .inbox={0}, .outbox={0}};
Mailbox<SMBUS_INBOX_SIZE, SMBUS_OUTBOX_SIZE>            smbus_services_mailbox  = {.my_id=SMBUS_SERVICES_ADDRESS,   .to_id=0, .from_id=0, .inbox_msg_size=0, .outbox_msg_size=0, .inbox_status=PACKET_ABSENT, .outbox_status=PACKET_ABSENT, .inbox_overflow=false, .inbox={0}, .outbox={0}};
Mailbox<WDDIS_PIN_INBOX_SIZE, WDDIS_PIN_OUTBOX_SIZE>    wd_dis_pin_mailbox      = {.my_id=WD_DIS_PIN_ADDRESS,       .to_id=0, .from_id=0, .inbox_msg_size=0, .outbox_msg_size=0, .inbox_status=PACKET_ABSENT, .outbox_status=PACKET_ABSENT, .inbox_overflow=false, .inbox={0}, .outbox={0}};
Mailbox<ENABLE_PIN_INBOX_SIZE, ENABLE_PIN_OUTBOX_SIZE>  enable_pin_mailbox      = {.my_id=ENABLE_PIN_ADDRESS,       .to_id=0, .from_id=0, .inbox_msg_size=0, .outbox_msg_size=0, .inbox_status=PACKET_ABSENT, .outbox_status=PACKET_ABSENT, .inbox_overflow=false, .inbox={0}, .outbox={0}};
Mailbox<RST_PIN_INBOX_SIZE, RST_PIN_OUTBOX_SIZE>        rst_pin_mailbox         = {.my_id=RST_PIN_ADDRESS,          .to_id=0, .from_id=0, .inbox_msg_size=0, .outbox_msg_size=0, .inbox_status=PACKET_ABSENT, .outbox_status=PACKET_ABSENT, .inbox_overflow=false                         };
Mailbox<WATCHDOG_INBOX_SIZE, WATCHDOG_OUTBOX_SIZE>      watchdog_mailbox        = {.my_id=WATCHDOG_ADDRESS,         .to_id=0, .from_id=0, .inbox_msg_size=0, .outbox_msg_size=0, .inbox_status=PACKET_ABSENT, .outbox_status=PACKET_ABSENT, .inbox_overflow=false, .inbox={0}, .outbox={0}};
Mailbox<EEPROM_INBOX_SIZE, EEPROM_OUTBOX_SIZE>          eeprom_mailbox          = {.my_id=EEPROM_ADDRESS,           .to_id=0, .from_id=0, .inbox_msg_size=0, .outbox_msg_size=0, .inbox_status=PACKET_ABSENT, .outbox_status=PACKET_ABSENT, .inbox_overflow=false, .inbox={0}, .outbox={0}};
Mailbox<TMP117_INBOX_SIZE, TMP117_OUTBOX_SIZE>          tmp117_mailbox          = {.my_id=TMP117_ADDRESS,           .to_id=0, .from_id=0, .inbox_msg_size=0, .outbox_msg_size=0, .inbox_status=PACKET_ABSENT, .outbox_status=PACKET_ABSENT, .inbox_overflow=false, .inbox={0}, .outbox={0}};
/****************************************************************************
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 * Process incoming and outgoing mail                                       *
 ****************************************************************************/
void BCB606_process_mail()
{
   /********************************
    * Incoming Mail                *
    ********************************/
    if(labcomm_packet.available)
        switch(labcomm_packet.destination_id)
        {
            case SMBUS_SERVICES_ADDRESS:    smbus_services_mailbox.get_packet(&labcomm_packet); break;
            case WD_DIS_PIN_ADDRESS:        wd_dis_pin_mailbox.get_packet(&labcomm_packet);     break;
            case ENABLE_PIN_ADDRESS:        enable_pin_mailbox.get_packet(&labcomm_packet);     break;
            case IDENTIFY_ADDRESS:          identify_mailbox.get_packet(&labcomm_packet);       break;
            case WATCHDOG_ADDRESS:          watchdog_mailbox.get_packet(&labcomm_packet);       break;
            case RST_PIN_ADDRESS:           rst_pin_mailbox.get_packet(&labcomm_packet);        break;
            case EEPROM_ADDRESS:            eeprom_mailbox.get_packet(&labcomm_packet);         break;
            case TMP117_ADDRESS:            tmp117_mailbox.get_packet(&labcomm_packet);         break;
        }
   /********************************
    * Outgoing Mail                *
    ********************************/
    if(tmp117_mailbox.outbox_status == PACKET_PRESENT)          tmp117_mailbox.send_packet();
    if(eeprom_mailbox.outbox_status == PACKET_PRESENT)          eeprom_mailbox.send_packet();
    if(rst_pin_mailbox.outbox_status == PACKET_PRESENT)         rst_pin_mailbox.send_packet();
    if(identify_mailbox.outbox_status == PACKET_PRESENT)        identify_mailbox.send_packet();
    if(watchdog_mailbox.outbox_status == PACKET_PRESENT)        watchdog_mailbox.send_packet();
    if(enable_pin_mailbox.outbox_status == PACKET_PRESENT)      enable_pin_mailbox.send_packet();
    if(wd_dis_pin_mailbox.outbox_status == PACKET_PRESENT)      wd_dis_pin_mailbox.send_packet();
    if(smbus_services_mailbox.outbox_status == PACKET_PRESENT)  smbus_services_mailbox.send_packet();
    if(labcomm_mailbox.outbox_status == PACKET_PRESENT)         labcomm_mailbox.send_packet();
}