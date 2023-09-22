from PyICe import twi_instrument, lab_core
import os
abspath = os.path.dirname(os.path.dirname(os.path.abspath(__file__))) + "\\PyICe_driver"

json_public_file    = f"{abspath}/stowe_pyice_public.json"
json_fuse_file      = f"{abspath}/stowe_pyice_fuse.json"
json_file           = f"{abspath}/stowe_pyice.json"

if not os.path.isfile(json_fuse_file):
    json_fuse_file = None

if not os.path.isfile(json_file):
    json_file = None

LT3390_ADDR7 = 0x69
LT339x_ADDR7 = 0x6A

def stowe_i2c(twi_port, addr7):
    def dac_update(code, msByteCh, lsByteCh, strobeCh):
        strobeCh.write(False) # Probably don't need both of these
        msByteCh.write((code >> 8) & 0xFF)
        lsByteCh.write(code & 0xFF)
        strobeCh.write(True)
        strobeCh.write(False) # Probably don't need both of these
    def dac_read(msByteCh, lsByteCh):
        return (msByteCh.read() << 8) | lsByteCh.read() # Virtual caching efficiency improvement someday?
    def add_dac_wrappers(twi_inst_inst):
        dac_channels = ['f_dac_mon_ch0',
                        'f_dac_mon_ch1',
                        'f_dac_mon_ch2',
                        'f_dac_mon_ch3',
                        'f_dac_mon_ch4',
                        'f_dac_mon_vin',
                        'f_dac_reg_ch0',
                        'f_dac_reg_ch1',
                        'f_dac_reg_ch2',
                        'f_dac_reg_ch3',
                        'f_dac_reg_ch4',
                       ]
        for dac in dac_channels:
            msByteCh = twi_inst_inst[f'{dac}_11_8_']
            lsByteCh = twi_inst_inst[f'{dac}_7_0_']
            new_ch = lab_core.register(name=dac,
                                       size=12,
                                       read_function  = lambda msByteCh=msByteCh, lsByteCh=lsByteCh: dac_read(msByteCh, lsByteCh),
                                       write_function = lambda code, msByteCh=msByteCh, lsByteCh=lsByteCh, strobeCh=twi_inst_inst['tm_dacs']: dac_update(code, msByteCh, lsByteCh, strobeCh)
                                      )
            new_ch.set_category('stowe_dac_wrapper')
            # new_ch.set_attribute("default", (msByteCh.get_attribute('default') << 8) | lsByteCh.get_attribute('default'))         ### Does not have a default setting
            vbg = 1.209 # todo look up from model!
            new_ch.add_format(format_name = 'dac_scale',
                              format_function = lambda code: vbg * code / 2**12,
                              unformat_function = lambda v: v / vbg * 2**12,
                              signed = False,
                              units = 'V',
                              xypoints = [(0,0),(4095,vbg)]
                             )
            # TODO add output voltage referred formats (include fb div)???
            # TODO remove raw bitfield channels??
            twi_inst_inst._add_channel(new_ch)
    stowe = twi_instrument.twi_instrument(twi_port, PEC = True, except_on_i2cCommError = True, retry_count = 1)
    if json_file is not None:
        stowe.populate_from_yoda_json_bridge(json_file, i2c_addr7 = addr7)
    else:
        stowe.populate_from_yoda_json_bridge(json_public_file, i2c_addr7 = addr7)
    if json_fuse_file is not None:
        stowe.populate_from_yoda_json_bridge(json_fuse_file, i2c_addr7 = addr7)
        add_dac_wrappers(stowe)
    return stowe