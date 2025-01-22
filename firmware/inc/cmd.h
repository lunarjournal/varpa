/**
 * VARPA MSc Project
 * 
 * Author: Dylan Muller
 * Copyright (c) 2025 
 * All rights reserved.
 * 
 * - Commercial/IP use prohibited.
 * - Attribution required.
 * See License.txt
 *
 */

#ifndef CMD_H_
#define CMD_H_

#include <avr/pgmspace.h>

#define CMD_MAX_ARGS 5
#define CMD_NEWLINE "\n"

#define CMD_DEF_ARG(x) x
#define CMD_DEF_SINGLE(x) x CMD_NEWLINE

#define CMD_ERROR(x) printf("ERROR %s\n", x)
#define CMD_PRINT(x) printf("INFO %s\n", x)
#define CMD_PRINT_FMT(x, y) printf("INFO " x CMD_NEWLINE, y)
#define CMD_SWITCH(x, y) !strcmp(x, y)

#define CMD_POWER_GET_STR "POWER?"
#define CMD_POWER_GET_DESC "QUERY RF POWER"

#define CMD_POWER_SET_STR "POWER"
#define CMD_POWER_SET_DESC "SET RF POWER"

#define CMD_TX_STR "TX"
#define CMD_TX_DESC "SWITCH TO TX MODE"

#define CMD_RX_STR "RX"
#define CMD_RX_DESC "SWITCH TO RX MODE"

#define CMD_DRAIN_SET_STR "DRAIN"
#define CMD_DRAIN_SET_DESC "SET DRAIN COUNTER"

#define CMD_PROBE_STR "PROBE"
#define CMD_PROBE_DESC "START GATE PROBE"

#define CMD_RESET_STR "RESET"
#define CMD_RESET_DESC "EXECUTE SOFT RESET"

#define CMD_REPORT_STR "REPORT"
#define CMD_REPORT_DESC "DUMP SYSTEM REPORT"

#define CMD_HELP_STR "HELP"
#define CMD_HELP_DESC "DUMP COMMAND LIST"

#define CMD_LINK_REPLY_STR "REPLY"

#define CMD_SIZE 9
#define CMD_ENTRY_SIZE 2

#define CMD_POWER_GET CMD_DEF_SINGLE(CMD_POWER_GET_STR)
#define CMD_POWER_SET CMD_DEF_ARG(CMD_POWER_SET_STR)
#define CMD_TX CMD_DEF_SINGLE(CMD_TX_STR)
#define CMD_RX CMD_DEF_SINGLE(CMD_RX_STR)
#define CMD_DRAIN_SET CMD_DEF_ARG(CMD_DRAIN_SET_STR)
#define CMD_PROBE CMD_DEF_SINGLE(CMD_PROBE_STR)
#define CMD_RESET CMD_DEF_SINGLE(CMD_RESET_STR)
#define CMD_REPORT CMD_DEF_SINGLE(CMD_REPORT_STR)
#define CMD_HELP CMD_DEF_SINGLE(CMD_HELP_STR)
#define CMD_LINK_REPLY CMD_DEF_SINGLE(CMD_LINK_REPLY_STR)

extern const char* const cmd_lut[][2];

#endif
