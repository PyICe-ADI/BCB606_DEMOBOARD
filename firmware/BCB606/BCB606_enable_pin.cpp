/****************************************************************************
 * BCB606 Stowe ENABLE Pin                                                  *
 * Steve Martin                                                            	*
 * June 1, 2023                                                            	*
 ****************************************************************************/
#include "BCB606_enable_pin.h"
// ┌──────────┬─────────┬──────────┬┬──────────┐
// │   TH     │  ~EN    │ ENABLE   ││ COMMAND  │
// └──────────┴─────────┴──────────┴┴──────────┘
// │    LOW   │   LOW   │   ON     ││    1     │
// └──────────┴─────────┴──────────┴┴──────────┘
// │    LOW   │   HIGH  │   OFF    ││    0     │
// └──────────┴─────────┴──────────┴┴──────────┘
// │    HIGH  │   LOW   │   HOOK   ││    2     │
// └──────────┴─────────┴──────────┴┴──────────┘
// │    HIGH  │   HIGH  │   OFF    ││   N/A    │
// └──────────┴─────────┴──────────┴┴──────────┘

#define COMMAND_BYTE        0
#define SET_STATE           1
#define GET_STATE           2
#define MSG_DATA_BYTE       1
#define REPLY_DATA_BYTE     0
#define OFF                 0
#define ON                  1
#define HOOK                2

/****************************************************************************
 * Set the pin state                                                        *
 ****************************************************************************/
void BCB606_set_enable_pin_state()
{
	if (enable_pin_mailbox.inbox_status == PACKET_PRESENT)
    {
        switch(enable_pin_mailbox.inbox[COMMAND_BYTE])
        {
            case SET_STATE:
            {
                switch(enable_pin_mailbox.inbox[MSG_DATA_BYTE])
                {
                    case OFF:   digitalWrite(TESTHOOKPIN, LOW);    digitalWrite(ENABLEPIN_b, HIGH);    break;
                    case ON:    digitalWrite(TESTHOOKPIN, LOW);    digitalWrite(ENABLEPIN_b, LOW);     break;
                    case HOOK:  digitalWrite(TESTHOOKPIN, HIGH);   digitalWrite(ENABLEPIN_b, LOW);     break;
                }
            }break;
            case GET_STATE: BCB606_get_enable_pin_state(); break;
        }
        enable_pin_mailbox.inbox_status = PACKET_ABSENT;
    }
}
/****************************************************************************
 * Get the pin state                                                        *
 ****************************************************************************/
void BCB606_get_enable_pin_state()
{
    enable_pin_mailbox.to_id = enable_pin_mailbox.from_id;
    enable_pin_mailbox.outbox_msg_size = ENABLE_PIN_OUTBOX_SIZE;
    enable_pin_mailbox.outbox[REPLY_DATA_BYTE] = digitalRead(TESTHOOKPIN) + !digitalRead(ENABLEPIN_b);
    enable_pin_mailbox.outbox_status = PACKET_PRESENT;
}