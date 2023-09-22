/****************************************************************************
 * BCB606 Stowe RST Pin                                                     *
 * Steve Martin                                                            	*
 * June 1, 2023                                                            	*
 ****************************************************************************/
#include "BCB606_rst_pin.h"

/****************************************************************************
 * Get the RST pin state                                                    *
 ****************************************************************************/
void BCB606_get_rst_pin()
{
    if (rst_pin_mailbox.inbox_status == PACKET_PRESENT)
    {
        rst_pin_mailbox.to_id = rst_pin_mailbox.from_id;
        rst_pin_mailbox.outbox[0] = digitalRead(RSTPIN);
        rst_pin_mailbox.outbox_msg_size = RST_PIN_OUTBOX_SIZE;
        rst_pin_mailbox.inbox_status = PACKET_ABSENT;
        rst_pin_mailbox.outbox_status = PACKET_PRESENT;
    }
}