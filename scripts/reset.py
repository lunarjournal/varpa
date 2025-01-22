# VARPA MSc Project
# 
# Author: Dylan Muller
# Copyright (c) 2025 
# All rights reserved.
# 
# - Commercial/IP use prohibited.
# - Attribution required.
# See License.txt

import serial
import socket
import time
import math
import sys
import os

from setup import *

RESET_DELAY = 2
EXIT_ERROR = 1

def serial_send(serial, data):
    serial.write(data.encode('utf-8'))

def serial_receive_line(serial):
    serial_data = serial.readline().decode('utf-8').rstrip() 
    return serial_data

def sys_reset(serial):
    serial_send(serial, SETUP_CMD_RESET)
    time.sleep(SETUP_CMD_TIMEOUT)
    response = serial_receive_line(serial)
    return response

sys_serial = serial.Serial(port=SETUP_SERIAL_PORT, baudrate=SETUP_SERIAL_BAUD, timeout=SETUP_SERIAL_TIMEOUT)

print("PRECOMPILE CONDITIONING")
print("EXEC SOFT RESET")
response = ""

try:
    response = sys_reset(sys_serial)
except:
    print("CRITICAL ERROR")

if ("RESET" in response):
    print("OK")
    time.sleep(RESET_DELAY)
    sys.exit(os.EX_OK)
else:
    sys.exit(EXIT_ERROR)
