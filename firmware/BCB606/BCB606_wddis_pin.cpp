/****************************************************************************
 * BCB606 Stowe WD_DISABLE Pin                                              *
 * Steve Martin                                                            	*
 * June 1, 2023                                                            	*
 ****************************************************************************/
#include "BCB606_wddis_pin.h"

#define COMMAND_BYTE    0
#define DATA_BYTE_OUT   0
#define DATA_BYTE_IN    1
#define SET_STATE       1
#define GET_STATE       2
#define OFF             0
#define ON              1

/****************************************************************************
 * Set the pin state                                                        *
 ****************************************************************************/
void BCB606_set_wd_dis_pin_state()
{
	if (wd_dis_pin_mailbox.inbox_status == PACKET_PRESENT)
    {        
        switch(wd_dis_pin_mailbox.inbox[COMMAND_BYTE])
        {
            case SET_STATE:
            {
                switch(wd_dis_pin_mailbox.inbox[DATA_BYTE_IN])
                {
                    case OFF:   digitalWrite(WD_DISABLEPIN, LOW);   break;
                    case ON:    digitalWrite(WD_DISABLEPIN, HIGH);  break;
                }
            }break;
            case GET_STATE: BCB606_get_wd_dis_pin_state(); break;
        }
        wd_dis_pin_mailbox.inbox_status = PACKET_ABSENT;
    }
}
/****************************************************************************
 * Get the pin state                                                        *
 ****************************************************************************/
void BCB606_get_wd_dis_pin_state()
{
    wd_dis_pin_mailbox.to_id = wd_dis_pin_mailbox.from_id;
    wd_dis_pin_mailbox.outbox_msg_size = WDDIS_PIN_OUTBOX_SIZE;
    wd_dis_pin_mailbox.outbox[DATA_BYTE_OUT] = digitalRead(WD_DISABLEPIN);
    wd_dis_pin_mailbox.outbox_status = PACKET_PRESENT;
}