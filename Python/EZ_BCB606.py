import sys, os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from PyICe_driver.BCB606 import BCB606
from PyICe import lab_core
from PyICe.lab_utils.banners import print_banner

print_banner("","Please enter the COM port number for your BCB606.", "It can be found obtained from the Windows Device Manager.","")
comport = input("Enter COM port number, COM")

channel_master = lab_core.channel_master()
BCB606_board = BCB606(channel_master, comport=f"COM{comport}", verbose=False)
channel_master.add(BCB606_board)

channel_master.write("BCB606_enable", "OFF") # Board is stateful
channel_master.write("BCB606_wd_svc_time", 16000)
channel_master.write("BCB606_wd_svc_enable", True)
channel_master.write("BCB606_wddis_pin", False)
channel_master.write("BCB606_enable", "ON")

channel_master.gui()