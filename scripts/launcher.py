# VARPA MSc Project
# 
# Author: Dylan Muller
# Copyright (c) 2025 
# All rights reserved.
# 
# - Commercial/IP use prohibited.
# - Attribution required.
# See License.txt

import subprocess
import time
import ctypes
import os
from setup import *

def open_fixed_size_cmd(width, height, script_path):
    command = f"mode con: cols={width} lines={height} && python.exe {script_path}"
    subprocess.Popen(["cmd.exe", "/K", command], creationflags=subprocess.CREATE_NEW_CONSOLE)

script_path = os.path.abspath(SETUP_STUB_NAME)
print(SETUP_LAUNCHER_NAME + " " + SETUP_LAUNCHER_VER)
open_fixed_size_cmd(width=SETUP_CONSOLE_COL,
                    height=SETUP_CONSOLE_ROW,
                    script_path=script_path)
