/****************************************************************************
 * Demo Board for the Stowe Family of Products                             	*
 * Steve Martin                                                            	*
 * June 1, 2023                                                            	*
 ****************************************************************************/
#include "BCB606.h"

/****************************************************************************
 * Setup                                                                    *
 ****************************************************************************/
void setup()
{
    // TwoWire aux_i2C(&sercom1, 11, 13); // started adding second I2C port
    Wire.begin();
    SerialUSB.begin(115200);
    pinMode(HEARTBEAT_LED, OUTPUT);
    pinMode(WD_DISABLEPIN, OUTPUT);
    pinMode(ENABLEPIN_b, OUTPUT);
    pinMode(TESTHOOKPIN, OUTPUT);
    pinMode(RSTPIN, INPUT);
    /**********************************************
    * Have D.U.T. default to off                  *
    ***********************************************/
    digitalWrite(WD_DISABLEPIN, HIGH);
    digitalWrite(ENABLEPIN_b, HIGH);

    setup_softport();
}
/****************************************************************************
 * Loop                                                                     *
 ****************************************************************************/
void loop() // Simple and fast round robin operating system
{
    process_serial();
    BCB606_identify();
    BCB606_set_leds();
    BCB606_heartbeat();
    BCB606_get_rst_pin();
    BCB606_process_mail();
    BCB606_service_smbus();
    BCB606_get_push_button();
    BCB606_eeprom_services();
    BCB606_watchdog_services();
    BCB606_set_wd_dis_pin_state();
    BCB606_set_enable_pin_state();
    BCB606_get_temperature_sensor();
}
