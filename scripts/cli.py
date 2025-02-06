# Author: Dylan Muller
# Copyright (c) 2025
# All rights reserved.
#
# - Commercial/IP use prohibited.
# - Attribution required.
# See License.txt

import TermTk as ttk
import os
import sys
import msvcrt
import ctypes
import time
import serial
import serial.tools.list_ports
from threading import Thread, Lock
import signal

from setup import *
from device import *

GWL_STYLE = -16
WS_SIZEBOX = 0x00040000
WS_MAXIMIZEBOX = 0x00010000
CLI_TITLE = "RFCLI"
CLI_VER = "1.00.01"

class gate_status:
    status = ""
    q_volt = 0
    q_amp = 0
    q_time = 0
    v_set = 0
    q_drain = 0

class drain_status:
    status = ""
    v_adc = 0
    v_set = 0
    amp = 0

class system_status:
    status = ""
    v_fwd = 0
    p_fwd = 0
    p_set = 0
    t_delta = 0

class global_status:
    gate = gate_status()
    drain = drain_status()
    system = system_status()

status = global_status()
root = ttk.TTk(title= CLI_TITLE + " " + CLI_VER,
               sigmask=())
serial_port = None
update_thread = None
lock = Lock()

def cli_inputs_focused():
    if (cli_LEDrainOverride.hasFocus() or
        cli_LEGateQDrain.hasFocus() or
        cli_LESystemPSet.hasFocus() or
        cli_LESystemSWRLim.hasFocus()):
        return True
    return False

def cli_update_thread():
    global serial_port

    while(True):
        with lock:
            if(serial_port.is_open):
                reply = sys_reply(serial_port)
                if (SETUP_CMD_BUSY_RESPONSE in reply):
                    serial_port.close()
                    continue
                if (cli_inputs_focused() == False):
                    rv = cli_update_status(serial_port)
                    if(rv == 0):
                        serial_port.close()
                    if(cli_FSystemStatus.color() != le_green):
                        cli_FSystemStatus.setColor(le_green)
                else:
                    if(cli_FSystemStatus.color() != le_red):
                        cli_FSystemStatus.setText("USER INPUT")
                        cli_FSystemStatus.setColor(le_red)

                if (cli_BSystemConnect.text() != "Disconnect"):
                    cli_BSystemConnect.setText("Disconnect")
            else:
                if (cli_inputs_focused() == False):
                    cli_FSystemStatus.setText("NO-LINK")
                    cli_BSystemConnect.setText("Connect")
        time.sleep(SETUP_CLI_UPDATE_RATE)

def cli_BSystemConnect_clicked():
    global serial_port
    global update_thread
    with lock:        
        if (serial_port is None or serial_port.is_open == False):
            target = cli_CBSystemPort.currentText()
            if (target == SETUP_STR_EMPTY):
                display_message("No valid COM port")
                return
            else:
                init_uart_link(target)
                if (serial_port and serial_port.is_open):
                    reply = sys_reply(serial_port)
                    if (SETUP_CMD_BUSY_RESPONSE in reply):
                        display_message("Target is busy")
                        serial_port.close()
                        return
                    elif (SETUP_CMD_REPLY_RESPONSE not in reply):
                        display_message("Target does not respond")
                        serial_port.close()
                        return
                else:
                    display_message("Failed establishing link")
                    serial_port.close()
                    return
                
            if (update_thread == None):
                update_thread = Thread(target=cli_update_thread)
                update_thread.start()
        else:
            serial_port.close()

def cli_update_gate_frame(line, param):
    global status
    if (SETUP_REPORT_STATUS in line):
        status.gate.status = param
        cli_FGateStatus.setText(param)
    elif (SETUP_REPORT_Q_VOLT in line):
        status.gate.q_volt = int(param)
        cli_FGateQV.setText(param)
    elif (SETUP_REPORT_Q_AMP in line):
        status.gate.q_amp = int(param)
        cli_FGateQA.setText(param)
    elif (SETUP_REPORT_Q_TIME in line):
        status.gate.q_time = int(param)
        cli_FGateQTime.setText(param)
    elif (SETUP_REPORT_V_SET in line):
        status.gate.v_set = int(param)
        cli_FGateVSet.setText(param)
    elif (SETUP_REPORT_Q_DRAIN in line):
        status.gate.q_drain = int(param)

def cli_update_drain_frame(line, param):
    global status
    if (SETUP_REPORT_STATUS in line):
        status.drain.status = param
        cli_FDrainStatus.setText(param)
    elif (SETUP_REPORT_V_ADC in line):
        status.drain.v_adc = int(param)
        cli_FDrainVADC.setText(param)
    elif (SETUP_REPORT_V_SET in line):
        status.drain.amp = int(param)
        cli_FDrainVSet.setText(param)

def cli_update_system_frame(line, param):
    global status
    if (SETUP_REPORT_STATUS in line):
        status.system.status = param
        cli_FSystemStatus.setText(param)
    elif (SETUP_REPORT_V_FWD in line):
        status.system.v_fwd = int(param)
        cli_FSystemVFwd.setText(param)
    elif (SETUP_REPORT_P_FWD in line):
        status.system.p_fwd = int(param)
        cli_FSystemPFwd.setText(param)
    elif (SETUP_REPORT_P_SET in line):
        status.system.p_set = int(param)
    elif (SETUP_REPORT_T_DELTA in line):
        status.system.t_delta = int(param)
        cli_FSystemTSet.setText(param)

def cli_update_status(serial):
    serial.flush()
    report = sys_report(serial)
    if (report == SETUP_STR_EMPTY or
        SETUP_CMD_BUSY_RESPONSE in report):
        return 0
    current_frame = 0
    fields_updated = 0
    for line in report:
        if (SETUP_REPORT_FRAME_GATE in line):
            current_frame = SETUP_REPORT_FRAME_GATE
            continue
        elif (SETUP_REPORT_FRAME_DRAIN in line):
            current_frame = SETUP_REPORT_FRAME_DRAIN
            continue
        elif (SETUP_REPORT_FRAME_SYSTEM in line):
            current_frame = SETUP_REPORT_FRAME_SYSTEM
            continue
        if (current_frame != 0 and serial_port):
            param = extract_param(line)
            if (param == False):
                return 0
            fields_updated += 1
            if (current_frame == SETUP_REPORT_FRAME_GATE):
                cli_update_gate_frame(line, param)
            elif (current_frame == SETUP_REPORT_FRAME_DRAIN):
                cli_update_drain_frame(line, param)
            elif (current_frame == SETUP_REPORT_FRAME_SYSTEM):
                cli_update_system_frame(line, param)
    return fields_updated

def signal_handler(sig, frame):
    display_message("test")
    exit_terminal()

def extract_param(line):
    split = line.split(':')
    if (len(split) != 2):
        return False
    return split[1].strip()

def uart_established(serial):
    if (serial and serial.is_open):
        return True
    return False

def init_uart_link(port):
    global serial_port
    try:
        if (serial_port is None):
            serial_port = serial.Serial(
                port=port,
                baudrate=SETUP_SERIAL_BAUD,
                timeout=SETUP_SERIAL_TIMEOUT)
            serial_port.open()
        else:
            serial_port.open()
    except serial.SerialException as e:
        return None
     
def list_com_ports():
    com = []
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        com.append(port.device)
    return com

def set_win_attributes():
    hwnd = ctypes.windll.kernel32.GetConsoleWindow()
    current_style = ctypes.windll.user32.GetWindowLongW(hwnd, GWL_STYLE)
    ctypes.windll.user32.SetWindowLongW(hwnd, GWL_STYLE, current_style &        
                                        ~WS_SIZEBOX & ~WS_MAXIMIZEBOX)

def cleanup_terminal():
    os.system('cls')
    os.system('echo \x1b[?25h')

    sys.stdout.flush()
    sys.stderr.flush()

    while msvcrt.kbhit():
        msvcrt.getch()
    os._exit(0)

def exit_terminal():
    root.quit()
    cleanup_terminal()

def display_message(text):
    messageBox = ttk.TTkMessageBox(text=text)
    ttk.TTkHelper.overlay(None, messageBox, 25, 10, True)

cli_ctrl_c = ttk.TTkK.CTRL | ttk.TTkK.Key_C
root.layout().addWidget(main_window)
ttk.TTkShortcut(cli_ctrl_c).activated.connect(exit_terminal)
cli_BSystemConnect.clicked.connect(cli_BSystemConnect_clicked)
cli_CBSystemPort.addItems(list_com_ports())

cli_LEGateQDrain.setInputType(ttk.TTkK.Input_Number)
cli_LESystemSWRLim.setInputType(ttk.TTkK.Input_Number)
cli_LEDrainOverride.setInputType(ttk.TTkK.Input_Number)
cli_LESystemPSet.setInputType(ttk.TTkK.Input_Number)

cli_FSystemStatus.setColor(le_green)

#BSystemExit.clicked.connect(exit_terminal)
#set_win_attributes()
root.mainloop()

#currentIndexChanged
