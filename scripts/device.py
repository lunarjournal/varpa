# VARPA MSc Project
# 
# Author: Dylan Muller
# Copyright (c) 2025 
# All rights reserved.
# 
# - Commercial/IP use prohibited.
# - Attribution required.
# See License.txt

from setup import *
import time

def serial_send(serial, data):
    try:
        if (serial and serial.is_open):
            serial.write(data.encode('utf-8'))
    except:
        return 0

def serial_receive_line(serial):
    try:
        if (serial and serial.is_open):
            serial_data = serial.readline().decode('utf-8').rstrip()
        else:
            return ""
    except:
        return ""
    return serial_data

def serial_receive_lines(serial, n):
    lines = []
    for i in range(n):
        line = serial_receive_line(serial)
        if (line == ""):
            return line
        if (SETUP_CMD_BUSY_RESPONSE in line):
            return SETUP_CMD_BUSY_RESPONSE
        lines.append(line)
    return lines

def sys_reset(serial):
    serial_send(serial, SETUP_CMD_RESET)
    time.sleep(SETUP_CMD_TIMEOUT)
    response = serial_receive_line(serial)
    return response

def sys_gate_probe(serial):
    serial_send(serial, SETUP_CMD_PROBE)
    time.sleep(SETUP_CMD_TIMEOUT)
    response = serial_receive_lines(serial, SETUP_CMD_PROBE_N)
    return response

def sys_drain_set(serial, count):
    payload = SETUP_CMD_DRAIN_SET + ' ' + str(count) + SETUP_NEWLINE
    serial_send(serial, payload)
    time.sleep(SETUP_CMD_TIMEOUT)

def sys_si4468_tx(serial):
    serial_send(serial, SETUP_CMD_TX)
    time.sleep(SETUP_CMD_TIMEOUT)
    response = serial_receive_line(serial)
    return response

def sys_si4468_rx(serial):
    serial_send(serial, SETUP_CMD_RX)
    time.sleep(SETUP_CMD_TIMEOUT)
    response = serial_receive_line(serial)
    return response

def sys_read_power(serial):
    serial_send(serial, SETUP_CMD_POWER_GET)
    time.sleep(SETUP_CMD_TIMEOUT)
    power_count = serial_receive_line(serial)
    power_count = power_count.split(" ")[1]
    power_count = int(power_count)
    return power_count

def sys_reply(serial):
    serial_send(serial, SETUP_CMD_REPLY)
    time.sleep(SETUP_CMD_TIMEOUT)
    response = serial_receive_line(serial)
    return response

def sys_report(serial):
    serial_send(serial, SETUP_CMD_REPORT)
    time.sleep(SETUP_CMD_TIMEOUT)
    response = serial_receive_lines(serial, SETUP_CMD_REPORT_N)
    return response
