all: compile

SHELL := /bin/bash

MCU = atmega328p
PROG = USBasp
AVRDUDE = avrdude.exe

LFUSE = 0xFF
HFUDE = 0xD9
EFUSE = 0xFF
LOCK = 0xFF

SCRIPTS_DIR = ../scripts
SCRIPT_RESET = reset.py

SRC_DIR = src
INC_DIR = inc

PYTHON = python.exe
CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -Wall -Os -I$(INC_DIR)
OBJ_C = avr-objcopy
AVR_SIZE = avr-size

SRC_FILES += $(SRC_DIR)/*.c
SRC_FILES += $(SRC_DIR)/peripheral/*.c
SRC_FILES += $(SRC_DIR)/module/*.c
SRC_FILES += $(SRC_DIR)/driver/*.c

BUILD_DIR = bin
OUTPUT = $(BUILD_DIR)/FW_DEBUG_V_1_0

reset:
	$(PYTHON) $(SCRIPTS_DIR)/$(SCRIPT_RESET)

build:
	mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(OUTPUT).lffw $(SRC_FILES)
	$(AVR_SIZE) -C -x $(OUTPUT).lffw
	$(OBJ_C) -j .text -j .data -O ihex $(OUTPUT).lffw $(OUTPUT).hex

flash:
	@read -p "ENTER TO BEGIN FLASH..."
	$(AVRDUDE) -p $(MCU) -c $(PROG) -v -V -u -U flash:w:$(OUTPUT).hex

fuze:
	$(AVRDUDE) -p $(MCU) -c $(PROG) -v -V -u -U lfuse:w:$(LFUSE):m	-U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m	-U lock:w:$(LOCK):m

clean:
	rm -fr $(BUILD_DIR)

upload: reset clean build flash
force-upload: clean build flash

deps:
	sudo apt install gcc-avr binutils-avr gdb-avr avr-libc avrdude make git
