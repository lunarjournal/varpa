# VARPA MSc Project
# 
# Author: Dylan Muller
# Copyright (c) 2025 
# All rights reserved.
# 
# - Commercial/IP use prohibited.
# - Attribution required.
# See License.txt

# Script for RMS power calibration
# using the DSA815. 

import serial
import socket
import time
import math

from setup import *
from device import *

rms_power_count = []
rms_power_dbm = []

rms_power_gradient_count = []
rms_power_threshold = []
rms_power_intercept = []

drain_increment = 0
drain_counter = 0

steps = int(2*SETUP_CAL_POINTS)

def dsa815_get_power(sa_socket):
    sa_socket.send(':CALCulate:MARKer1:Y?\r\n'.encode('utf-8'))
    power_dbm = float(sa_socket.recv(1024).strip().decode('utf-8'))
    return power_dbm

def print_response(data):
    for i in data:
        print(i)

# Initialize connections
sa_socket = socket.create_connection((SETUP_DSA815_IP, SETUP_DSA815_PORT))
sys_serial = serial.Serial(port=SETUP_SERIAL_PORT, baudrate=SETUP_SERIAL_BAUD, timeout=SETUP_SERIAL_TIMEOUT)

print("BATCH START")

print ("EXEC SYSTEM RESET")
response = sys_reset(sys_serial)
print(response)
time.sleep(SETUP_RESET_TIMEOUT)
sys_serial.flushInput()

print("EXEC GATE PROBE")
response = sys_gate_probe(sys_serial)
print_response(response)

print("EXEC TX MODE")
response = sys_si4468_tx(sys_serial)
print(response)

print("DATA ACQUISITION START")

for i in range(steps):

    if (i > SETUP_CAL_POINTS ):
        drain_increment = SETUP_DRAIN_INC_CORASE
    else:
        drain_increment = SETUP_DRAIN_INC_FINE

    sys_drain_set(drain_counter)
    fw_power_count = sys_read_power(sys_serial)

    time.sleep(SETUP_LOOP_DELAY)

    fw_power_dbm = dsa815_get_power(sa_socket)
    print(drain_counter, fw_power_count, fw_power_dbm)

    rms_power_count.append(float(fw_power_count))
    rms_power_dbm.append(float(fw_power_dbm))

    drain_counter += drain_increment

print("EXEC RX MODE")
response = sys_si4468_rx(sys_serial)
print(response)

print("DATA ACQUISITION END")

for i in range(0, steps, 2):
    num_power_gradient = float(rms_power_count[i + 1] - rms_power_count[i])
    den_power_gradient = float(rms_power_dbm[i + 1] - rms_power_dbm[i])

    power_gradient = num_power_gradient / den_power_gradient
    power_gradient_count = int(math.ceil((power_gradient * 10)))

    power_intercept = float(rms_power_dbm[i + 1] - (float(1/power_gradient)*rms_power_count[i + 1]))

    power_intercept = int(float(round(power_intercept, 2) * 100))

    rms_power_gradient_count.append(power_gradient_count)
    rms_power_intercept.append(power_intercept)
    rms_power_threshold.append(int(rms_power_count[i + 1]))


c_power_gradient = "{ " + ", ".join(map(str, rms_power_gradient_count)) + " };"
c_power_intercept = "{ " + ", ".join(map(str, rms_power_intercept)) + " };"
c_power_threshold = "{ " + ", ".join(map(str, rms_power_threshold)) + " };"

print("C DUMP START")

print("const uint16_t fw_pwr_gradient[] = " + c_power_gradient)
print("const int16_t fw_pwr_intercept[] = " + c_power_intercept)
print("const uint16_t fw_pwr_threshold[] = " + c_power_threshold)

print("C DUMP END")

print("BATCH END")
