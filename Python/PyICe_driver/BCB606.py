import sys, os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))) + "\\PyICe_driver")
from PyICe.lab_instruments.TMP117 import TMP117
from PyICe.lab_instruments.BR24H64 import BR24H64
from PyICe import lab_interfaces
from PyICe.lab_core import *
import stowe_stowe_i2c

# Client list
PYICE_GUI_ADDRESS           = 1
ENABLE_PIN_ADDRESS          = 2
RST_PIN_ADDRESS             = 3
WDDIS_PIN_ADDRESS           = 4
SMBUS_SVC_ADDRESS           = 5
IDENTIFY_ADDRESS            = 6
WATCHDOG_ADDRESS            = 7
EEPROM_ADDRESS              = 8
TMP117_ADDRESS              = 9

EMPTY                       = ""
SET_EN_CMD                  = "\x01"
GET_EN_CMD                  = "\x02"
STOWE_ON                    = SET_EN_CMD + "\x01"
STOWE_OFF                   = SET_EN_CMD + "\x00"
STOWE_HOOK                  = SET_EN_CMD + "\x02"
SET_WDDIS_STATE             = "\x01"
WDDIS_ON                    = SET_WDDIS_STATE + "\x01"
WDDIS_OFF                   = SET_WDDIS_STATE + "\x00"
WDDIS_GET_STATE             = "\x02"
WD_SET_RESPONSE_TIME_us     = "\x01" # Should be followed by 4 bytes for the micros() 32 bit value.
WD_GET_RESPONSE_TIME_us     = "\x04"
WD_GET_SERVICE_STATE        = "\x05"
WD_ENABLE_SERVICE           = "\x02" # Up to 2^32 µs or 71.58 minutes.
WD_DISABLE_SERVICE          = "\x03"
IDENT_WRITE_SCRATCHPAD      = "\x01"
IDENT_READ_SCRATCHPAD       = "\x02"
IDENT_GET_SERIALNUM         = "\x03"

class BCB606(instrument):
    ''' Stowe Demo Board, a base board that accepts the Stowe bench evaluation target board but can be given to customers.
        Has PyICe as a depdendency.'''
    def __init__(self, channel_master, comport, verbose=False):
        self._base_name = 'BCB606'
        instrument.__init__(self, f"{self._base_name}")
        self.verbose = verbose
        interface_factory   = lab_interfaces.interface_factory()
        self.ENABLE_port    = interface_factory.get_labcomm_raw_interface(comport_name=comport, src_id=PYICE_GUI_ADDRESS, dest_id=ENABLE_PIN_ADDRESS,  baudrate=115200, timeout=4)
        self.RST_port       = interface_factory.get_labcomm_raw_interface(comport_name=comport, src_id=PYICE_GUI_ADDRESS, dest_id=RST_PIN_ADDRESS,     baudrate=115200, timeout=4)
        self.WDDIS_port     = interface_factory.get_labcomm_raw_interface(comport_name=comport, src_id=PYICE_GUI_ADDRESS, dest_id=WDDIS_PIN_ADDRESS,   baudrate=115200, timeout=4)
        self.IDENT_port     = interface_factory.get_labcomm_raw_interface(comport_name=comport, src_id=PYICE_GUI_ADDRESS, dest_id=IDENTIFY_ADDRESS,    baudrate=115200, timeout=4)
        self.WATCHDOG_port  = interface_factory.get_labcomm_raw_interface(comport_name=comport, src_id=PYICE_GUI_ADDRESS, dest_id=WATCHDOG_ADDRESS,    baudrate=115200, timeout=4)
        self.TMP117_port    = interface_factory.get_labcomm_twi_interface(comport_name=comport, src_id=PYICE_GUI_ADDRESS, dest_id=TMP117_ADDRESS,      baudrate=115200, timeout=4)
        self.EEPROM_port    = interface_factory.get_labcomm_twi_interface(comport_name=comport, src_id=PYICE_GUI_ADDRESS, dest_id=EEPROM_ADDRESS,      baudrate=115200, timeout=4)
        self.STOWE_port     = interface_factory.get_labcomm_twi_interface(comport_name=comport, src_id=PYICE_GUI_ADDRESS, dest_id=SMBUS_SVC_ADDRESS,   baudrate=115200, timeout=4)
        self.TMP117         = TMP117(interface_twi=self.TMP117_port, addr7=0x49)
        self.EEPROM         = BR24H64(interface_twi=self.EEPROM_port, addr7=0x50)
        stowe_i2c           = stowe_stowe_i2c.stowe_i2c(twi_port=self.STOWE_port, addr7=stowe_stowe_i2c.LT3390_ADDR7)
        self.add_all_channels()
        channel_master.add(stowe_i2c)
        channel_master.add(self.TMP117)
        channel_master.add(self.EEPROM)
        self.reset_board()

    def __del__(self):
        '''Close interface (serial) ports on exit???'''
        # self.ENABLE_port.close()
        pass
        
    def _send_payload(self, port, payload):
        if self.verbose:
            print(f"Sending payload: {payload.encode('latin1')}")
        port.send_payload(payload)
        
    def _get_payload(self, port, datatype):
        x = port.receive_packet()
        if self.verbose:
            print(f"Received packet {x}")
        if datatype == "integer":
            return port.parser.payload_buffer_as_integer
        elif datatype == "string":
            return port.parser.payload_buffer_as_string
        else:
            raise(f"BCB606: Don't know data type {datatype}.")

    def add_all_channels(self):
        '''Helper function adds all available channels.'''
        self.add_channel_rstpin(self._base_name + "_rst")
        self.add_channel_wdpin(self._base_name + "_wddis_pin")
        self.add_channel_enablepin(self._base_name + "_enable")
        self.add_channel_identify(self._base_name + "_board_id")
        self.add_channel_TMP117(self._base_name + "_temp_sensor")
        self.add_channel_scratchpad(self._base_name + "_scratchpad")
        self.add_channel_serialnum(self._base_name + "_baseboard_serialnum")
        self.add_channel_wd_enable(self._base_name + "_wd_svc_enable")
        self.add_channel_EEPROM(self._base_name + "_targetboard_serialnum")
        self.add_channel_wd_response_time(self._base_name + "_wd_svc_time")

    def add_channel_enablepin(self, channel_name):
        def _set_enable_pin(value):
            if value in [1, True, "ON"]:
                payload = STOWE_ON
            elif value in [0, False, "OFF"]:
                payload = STOWE_OFF
            elif value in [2, "HOOK"]:
                payload = STOWE_HOOK
            else:
                raise ValueError((f'\n\nBCB606: Sorry don\'t know how to set Stowe\'s Enable pin to "{value}".'))
            self._send_payload(port=self.ENABLE_port, payload=payload)
        def _get_enable_pin():
            self._send_payload(port=self.ENABLE_port, payload=GET_EN_CMD)
            return self._get_payload(port=self.ENABLE_port, datatype="integer")
        self.enablepin_channel = channel(channel_name, write_function=lambda value: _set_enable_pin(value))
        self.enablepin_channel.add_preset("ON", "Turn on Stowe")
        self.enablepin_channel.add_preset("OFF", "Turn off Stowe")
        self.enablepin_channel.add_preset("HOOK", "Test hook Stowe")
        self.enablepin_channel._read = _get_enable_pin
        return self._add_channel(self.enablepin_channel)
        
    def add_channel_rstpin(self, channel_name):
        def _get_rst_pin():
            self._send_payload(port=self.RST_port, payload=EMPTY)
            return self._get_payload(port=self.RST_port, datatype="integer")
        new_channel = channel(channel_name, read_function=_get_rst_pin)
        return self._add_channel(new_channel)
        
    def add_channel_wdpin(self, channel_name):
        def _set_wddis_pin(set_hi):
            self._send_payload(port=self.WDDIS_port, payload=WDDIS_ON if set_hi else WDDIS_OFF)
        def _get_wdpin_state():
            self._send_payload(port=self.WDDIS_port, payload=WDDIS_GET_STATE)
            return self._get_payload(port=self.WDDIS_port, datatype="integer")
        self.wddpin_channel = integer_channel(name=channel_name, size=1, write_function=_set_wddis_pin)
        self.wddpin_channel._read = _get_wdpin_state
        return self._add_channel(self.wddpin_channel)

    def add_channel_identify(self, channel_name):
        def _get_id():
            self._send_payload(port=self.IDENT_port, payload="?")
            return self._get_payload(self.IDENT_port, datatype="string")
        new_channel = channel(channel_name, read_function=_get_id)
        return self._add_channel(new_channel)

    def add_channel_scratchpad(self, channel_name):
        def _write_scratchpad(scratchpad):
            self._send_payload(port=self.IDENT_port, payload=IDENT_WRITE_SCRATCHPAD + str(scratchpad))
        def _read_scratchpad():
            self._send_payload(port=self.IDENT_port, payload=IDENT_READ_SCRATCHPAD)
            return self._get_payload(self.IDENT_port, datatype="string")
        new_channel = channel(channel_name, write_function=lambda scratchpad: _write_scratchpad(scratchpad))
        new_channel._read = _read_scratchpad
        return self._add_channel(new_channel)

    def add_channel_serialnum(self, channel_name):
        def _get_serial_number():
            self._send_payload(port=self.IDENT_port, payload=IDENT_GET_SERIALNUM)
            return str(hex(self._get_payload(self.IDENT_port, datatype="integer")))
        new_channel = channel(channel_name, read_function=_get_serial_number)
        new_channel._read = _get_serial_number
        return self._add_channel(new_channel)

    def add_channel_wd_response_time(self, channel_name):
        def _set_wd_response_time(delay_us):
            payload = WD_SET_RESPONSE_TIME_us + int.to_bytes(delay_us, length=4, byteorder="big").decode("latin-1")
            self._send_payload(port=self.WATCHDOG_port, payload=payload)
        def _get_channel_wd_response_time():
            self._send_payload(port=self.WATCHDOG_port, payload=WD_GET_RESPONSE_TIME_us)
            return self._get_payload(port=self.WATCHDOG_port, datatype="integer")
        self.wd_service_time_channel = integer_channel(channel_name, size=32, write_function=lambda delay_us: _set_wd_response_time(delay_us))
        self.wd_service_time_channel._read = _get_channel_wd_response_time
        self.wd_service_time_channel.add_format(format_name="wd_us2ms_fmt", format_function=lambda x: round(x/1000), unformat_function=lambda x: int(x)*1000, signed=False, units='ms')
        self.wd_service_time_channel.set_display_format_str('5.3f')
        return self._add_channel(self.wd_service_time_channel)

    def add_channel_wd_enable(self, channel_name):
        def _wd_enable(wddis_enable):
            self._send_payload(port=self.WATCHDOG_port, payload=WD_ENABLE_SERVICE if wddis_enable else WD_DISABLE_SERVICE)
        def _get_wd_service_state():
            self._send_payload(port=self.WATCHDOG_port, payload=WD_GET_SERVICE_STATE)
            return self._get_payload(self.WATCHDOG_port, datatype="integer")
        self.wd_service_enable_channel = integer_channel(name=channel_name, size=1, write_function=_wd_enable)
        self.wd_service_enable_channel._read = _get_wd_service_state
        return self._add_channel(self.wd_service_enable_channel)
        
    def add_channel_EEPROM(self, channel_name):
        def get_board_id():
            eeprom_records = self.EEPROM.read_dictionary(verbose=self.verbose)
            return eeprom_records["S/N"]
        new_channel = channel(channel_name, read_function=get_board_id)
        return self._add_channel(new_channel)
        
    def add_channel_TMP117(self, channel_name):
        new_channel = channel(channel_name, read_function=self.TMP117.read_temp)
        return self._add_channel(new_channel)
        
    def reset_board(self):
        self.enablepin_channel.write("OFF")
        self.wddpin_channel.write(True)
        self.wd_service_time_channel.write(48000)
        self.wd_service_enable_channel.write(True)
        self.enablepin_channel.write("ON")
        