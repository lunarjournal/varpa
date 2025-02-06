# Author: Dylan Muller
# Copyright (c) 2025 
# All rights reserved.
# 
# - Commercial/IP use prohibited.
# - Attribution required.
# See License.txt

from TermTk import TTkUtil, TTkUiLoader, TTk, TTkColor

# DSA815 analyzer config
SETUP_DSA815_IP = '169.254.0.3'
SETUP_DSA815_PORT = 5555

# Serial link config
SETUP_SERIAL_PORT = 'COM4'
SETUP_SERIAL_BAUD = 19200
SETUP_SERIAL_TIMEOUT = 1

SETUP_CLI_UPDATE_RATE = 0.1
SETUP_CLI_BUSY_TIMEOUT = 5

# Timer delay
SETUP_LOOP_DELAY = 0.1

# Misc
SETUP_NEWLINE = "\n"
SETUP_CMD_TIMEOUT = 0.25
SETUP_RESET_TIMEOUT = 5

# Commands
SETUP_CMD_REPORT = "REPORT" + SETUP_NEWLINE
SETUP_CMD_REPORT_N = 19
SETUP_CMD_RESET = "RESET" + SETUP_NEWLINE
SETUP_CMD_PROBE = "PROBE" + SETUP_NEWLINE
SETUP_CMD_PROBE_N = 4
SETUP_CMD_TX = "TX" + SETUP_NEWLINE
SETUP_CMD_RX = "RX" + SETUP_NEWLINE
SETUP_CMD_DRAIN_SET = "DRAIN"
SETUP_CMD_POWER_GET = "POWER?" + SETUP_NEWLINE
SETUP_CMD_REPLY = "REPLY" + SETUP_NEWLINE
SETUP_CMD_REPLY_RESPONSE = "HELLO"
SETUP_CMD_BUSY_RESPONSE = "BUSY"
SETUP_STR_EMPTY = ""

SETUP_REPORT_FRAME_GATE = "- GATE -"
SETUP_REPORT_FRAME_DRAIN = "- DRAIN -"
SETUP_REPORT_FRAME_SYSTEM = "- SYSTEM -"

SETUP_REPORT_STATUS = "STATUS"
SETUP_REPORT_Q_VOLT = "Q-VOLT"
SETUP_REPORT_Q_AMP = "Q-AMP"
SETUP_REPORT_Q_TIME = "Q-TIME"
SETUP_REPORT_V_SET = "V-SET"
SETUP_REPORT_Q_DRAIN = "Q-DRAIN"
SETUP_REPORT_V_ADC = "V-ADC"
SETUP_REPORT_AMP = "AMP"
SETUP_REPORT_V_FWD = "V-FWD"
SETUP_REPORT_P_FWD = "P-FWD"
SETUP_REPORT_P_SET = "P-SET"
SETUP_REPORT_T_DELTA = "T-DELTA"

SETUP_CAL_POINTS = 10
SETUP_DRAIN_INC_FINE = 50
SETUP_DRAIN_INC_CORASE = 200

SETUP_CONSOLE_COL = 79
SETUP_CONSOLE_ROW = 27
SETUP_STUB_NAME = "stub.py"

SETUP_LAUNCHER_NAME = "RFCLI LAUNCHER"
SETUP_LAUNCHER_VER = "1.00.00"

main_window = TTkUiLoader.loadDict(TTkUtil.base64_deflate_2_obj(
    "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" +
    "qYaFE3H4P2ZwWfk="))

bg_red = TTkColor.bg('#e86448')
bg_green = TTkColor.bg('#8ce848')

fg_white = TTkColor.fg('#ffffff')
fg_black = TTkColor.fg('#000000')

le_red = bg_red + fg_white
le_green = bg_green + fg_black

# Gate Widgets
cli_FGateStatus = main_window.getWidgetByName("FGateStatus")
cli_FGateQV = main_window.getWidgetByName("FGateQV")
cli_FGateQA = main_window.getWidgetByName("FGateQA")
cli_FGateQTime = main_window.getWidgetByName("FGateQTime")
cli_FGateVSet = main_window.getWidgetByName("FGateVSet")
cli_CBGateFunc = main_window.getWidgetByName("CBGateFunc")
cli_LEGateQDrain = main_window.getWidgetByName("LEGateQDrain")
cli_BGateProbe = main_window.getWidgetByName("BGateProbe")

# Drain Widgets
cli_FDrainStatus = main_window.getWidgetByName("FDrainStatus")
cli_FDrainVSet = main_window.getWidgetByName("FDrainVSet")
cli_FDrainVADC = main_window.getWidgetByName("FDrainVADC")
cli_FDrainA = main_window.getWidgetByName("FDrainA")
cli_CBDrainFunc = main_window.getWidgetByName("CBDrainFunc")
cli_LEDrainOverride = main_window.getWidgetByName("LEDrainOverride")
cli_BDrainOverride = main_window.getWidgetByName("BDrainOverride")

# System Widgets
cli_FSystemStatus = main_window.getWidgetByName("FSystemStatus")
cli_FSystemVFwd = main_window.getWidgetByName("FSystemVFwd")
cli_FSystemAtt = main_window.getWidgetByName("FSystemAtt")
cli_CBSystemPStrat = main_window.getWidgetByName("CBSystemPStrat")
cli_CBSystemPort = main_window.getWidgetByName("CBSystemPort")
cli_LESystemPSet = main_window.getWidgetByName("LESystemPSet")
cli_BSystemConnect = main_window.getWidgetByName("BSystemConnect")
cli_LESystemSWRLim = main_window.getWidgetByName("LESystemSWRLim")
cli_FSystemSWR = main_window.getWidgetByName("FSystemSWR")
cli_FSystemPFwd = main_window.getWidgetByName("FSystemPFwd")
cli_FSystemPRev = main_window.getWidgetByName("FSystemPRev")
cli_BSystemReboot = main_window.getWidgetByName("BSystemReboot")
cli_BSystemSave = main_window.getWidgetByName("BSystemSave")
cli_FSystemTSet = main_window.getWidgetByName("FSystemTSet")
cli_BSystemDefaults = main_window.getWidgetByName("BSystemDefaults")
cli_BSystemSetPower = main_window.getWidgetByName("BSystemSetPower")
cli_FSystemVRev = main_window.getWidgetByName("FSystemVRev")
