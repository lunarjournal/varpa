/**
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

#include "setup.h"

#include "peripheral/timer.h"
#include "peripheral/uart.h"
#include "peripheral/twi.h"
#include "peripheral/spi.h"

#include "driver/mcp3202.h"
#include "driver/mcp4725.h"
#include "driver/f1956.h"
#include "driver/si4468.h"

#include "module/gate.h"
#include "module/drain.h"

#include "cmd.h"
#include "bsp.h"
#include "status.h"
#include "util.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

extern t_gate_status gate_status;
extern t_drain_status drain_status;
extern t_system_status system_status;

void system_info(void)
{
    CMD_PRINT("SYSTEM BOOT v" SYS_VER_STR);
    CMD_PRINT("SYSTEM INIT");
    printf("%d BYTES AVAILABLE\n", util_get_free_mem());
}

void setup(void)
{
    cli();

    gate_status.status = GATE_STATUS_INACTIVE;
    drain_status.status = DRAIN_STATUS_INACTIVE;
    system_status.status = SYSTEM_STATUS_IDLE;

    wdt_reset();
    wdt_disable();

    timer_init();

    uart_init();
    twi_init();
    spi_init();

    sei();

    _delay_ms(INIT_DELAY_MS);

    si4468_init();

    adc_init();
    att_init(0);
    dac_init(0);

    _delay_ms(INIT_DELAY_MS);

    si4468_apply_startup_config();

    _delay_ms(INIT_DELAY_MS);

    si4468_rx_mode();
}

void report_system_status()
{
    uint16_t gate_v_set = 0x0;
    uint16_t gate_v_count = 0x0;

    uint16_t drain_v_set = 0x0;
    uint16_t drain_v_adc = 0x0;
    uint16_t drain_amp_count = 0x0;
    uint16_t drain_amp = 0x0;
    uint16_t drain_v_count = 0x0;

    uint16_t fw_pwr_adc = 0x0;
    uint16_t fw_pwr_volt = 0x0;
    int16_t fw_pwr = 0x0;

    // gate status

    gate_status.q_drain = GATE_PROBE_DRAIN;
    gate_v_count = dac_read(MCP4725_ID_GATE_VOLT, 0);
    util_count_to_volt(gate_v_count, &gate_v_set);
    gate_status.v_set = gate_v_set;

    
    printf_P(PSTR("- GATE -\n"));
    printf_P(PSTR("STATUS            :  %s\n"), 
             pgm_read_word(&gate_lut[gate_status.status]));
    printf_P(PSTR("Q-VOLT    (mV)    :  %d\n"), gate_status.q_volt);
    printf_P(PSTR("Q-AMP     (mA)    :  %d\n"), gate_status.q_amp);
    printf_P(PSTR("Q-TIME    (ms)    :  %d\n"), gate_status.q_time);
    printf_P(PSTR("V-SET     (mV)    :  %d\n"), gate_status.v_set);
    printf_P(PSTR("Q-DRAIN   (mV)    :  %d\n"), gate_status.q_drain);

    // drain status
    drain_v_count = dac_read(MCP4725_ID_DRAIN_VOLT, 0);
    util_count_to_volt(drain_v_count, &drain_v_set);
    drain_status.v_set = drain_v_set;

    drain_v_count = adc_read(MCP3202_ID_DRAIN_VOLT, 0);
    util_count_to_volt(drain_v_count, &drain_v_adc);
    drain_status.v_adc = drain_v_adc;

    drain_amp_count = adc_read(MCP3202_ID_DRAIN_AMP, 0);
    util_count_to_volt(drain_amp_count, &drain_amp);
    drain_status.amp = drain_amp;

    printf_P(PSTR("- DRAIN -\n"));
    printf_P(PSTR("STATUS            :  %s\n"), 
             pgm_read_word(&drain_lut[drain_status.status]));
    printf_P(PSTR("V-ADC     (mV)    :  %d\n"), drain_status.v_adc);
    printf_P(PSTR("V-SET     (mV)    :  %d\n"), drain_status.v_set);
    printf_P(PSTR("AMP       (mA)    :  %d\n"), drain_status.amp);

    // system status
    fw_pwr_adc = adc_read_n(MCP3202_ID_FW_POWER, DRAIN_POWER_SAMPLES);
    util_count_to_volt(fw_pwr_adc, &fw_pwr_volt);
    util_count_to_pwr(fw_pwr_adc, &fw_pwr);
    system_status.v_fw = fw_pwr_volt;
    system_status.p_fw = fw_pwr;

    printf_P(PSTR("- SYSTEM -\n"));
    printf_P(PSTR("STATUS            :  %s\n"),
             pgm_read_word(&system_lut[system_status.status]));
    printf_P(PSTR("V-FWD     (mV)    :  %d\n"), system_status.v_fw);
    printf_P(PSTR("P-FWD     (cdBm)  :  %d\n"), system_status.p_fw);
    printf_P(PSTR("P-SET     (cdBm)  :  %d\n"), system_status.p_set);
    printf_P(PSTR("T-DELTA   (ms)    :  %d\n"), system_status.d_time);
}

void loop()
{
    char uart_buffer[UART_BUF_SIZE];
    char cmd_buffer[UART_BUF_SIZE];
    char* cmd_tok = 0x0;
    int16_t argv[CMD_MAX_ARGS];
    uint16_t argc = 0x0;
    bool gate_probed = false;

    while (1)
    {
        cmd_tok = 0x0;
        argc = 0x0;
    
        if(uart_ngets(uart_buffer, UART_BUF_SIZE))
        {
            cmd_tok = strtok(uart_buffer, " ");
            strcpy(cmd_buffer, cmd_tok);
            
            while ((cmd_tok = strtok(NULL, " ")) != NULL) {
                argv[argc++] = atoi(cmd_tok);
            }

            if (CMD_SWITCH(CMD_REPORT, uart_buffer))
            {
                CMD_PRINT("DUMP SYSTEM REPORT");
                report_system_status();
            }

            else if (CMD_SWITCH(CMD_POWER_GET, uart_buffer))
            {
               CMD_PRINT("DUMP POWER ADC");
               uint16_t fw_pwr_adc = adc_read_n(MCP3202_ID_FW_POWER,10);
               CMD_PRINT_FMT("%d", fw_pwr_adc);
            }

            else if (CMD_SWITCH(CMD_PROBE, uart_buffer))
            {
                uint16_t gate_probe_count = 0x0;
                uint16_t drain_current = 0x0;
                uint32_t time_start = 0x0;
                uint16_t time_delta = 0x0;

                if (!gate_probed)
                {
                    CMD_PRINT("STARTING Q-PROBE");
                    CMD_PRINT_FMT("DRAIN_Q_AMP = %d mA", DRAIN_Q_AMP);

                    time_start = timer_millis();

                    si4468_rx_mode();
                    drain_current = gate_probe(DRAIN_Q_AMP, &gate_probe_count);

                    time_delta = (uint16_t)(timer_millis() - time_start);
                    gate_status.q_time = time_delta;

                    CMD_PRINT_FMT("PROBE COMPLETED IN %d ms", time_delta);

                    if (drain_current >= DRAIN_Q_AMP)
                    {
                        gate_status.status = GATE_STATUS_PROBE_SUCCESS;
                        CMD_PRINT_FMT("Q-PROBE SUCCEEDED, IQ = %d mA", drain_current);
                        gate_probed = true;
                    }
                    else
                    {
                        gate_status.status = GATE_STATUS_PROBE_ERROR;
                        CMD_PRINT_FMT("Q-PROBE FAILED, IQ = %d mA", drain_current);
                        continue;
                    }
                }
                else
                {
                    CMD_ERROR("ACTIVE GATE PROBE");
                }
            }

            else if (CMD_SWITCH(CMD_POWER_SET, cmd_buffer))
            {
                if (argc != 1)
                {
                    CMD_ERROR("USAGE: POWER <cdBm>");
                    continue;
                }

                int16_t rf_power = argv[0];

                if (rf_power < DRAIN_POWER_LOW_LIM ||
                    rf_power > DRAIN_POWER_HIGH_LIM ||
                    (rf_power % DRAIN_POWER_MODULUS) != 0)
                {
                    CMD_ERROR("INVALID INPUT POWER");
                    continue;
                }

                if (!gate_probed)
                {
                    CMD_PRINT("NO GATE PROBE");
                    continue;

                }

                system_status.p_set = rf_power;

                CMD_PRINT_FMT("POWER SETPOINT = %d cdBm", rf_power);
                CMD_PRINT("STARTING LINEAR SWEEP");
                drain_sweep_pwr(rf_power);
                CMD_PRINT_FMT("SWEEP COMPLETED IN %d ms", system_status.d_time);
            }

            else if (CMD_SWITCH(CMD_DRAIN_SET, cmd_buffer))
            {
                if (argc != 1)
                {
                    CMD_ERROR("USAGE: DRAIN <count>");
                    continue;
                }

                uint16_t drain_count = argv[0];
                if (drain_count > 4096)
                {
                    CMD_ERROR("RANGE ERROR");
                    continue;
                }

                dac_write(MCP4725_ID_DRAIN_VOLT, drain_count, 1);
            }

            else if (CMD_SWITCH(CMD_RX, uart_buffer))
            {
                CMD_PRINT("RADIO RX MODE");
                si4468_rx_mode();
            }

            else if (CMD_SWITCH(CMD_TX, uart_buffer))
            {
                CMD_PRINT("RADIO TX MODE");
                si4468_tx_mode();
            }

            else if (CMD_SWITCH(CMD_HELP, uart_buffer))
            {
                uint16_t i = 0x0;

                CMD_PRINT("DUMP COMMANDS");
                printf("- COMMANDS -\n");
                for(i = 0; i < CMD_SIZE; i++)
                {
                    printf_P(PSTR("%-10s - %-10s\n"),
                        pgm_read_word(&cmd_lut[i][0]),
                        pgm_read_word(&cmd_lut[i][1]));
                }
            }

            else if (CMD_SWITCH(CMD_LINK_REPLY, uart_buffer))
            {
                CMD_PRINT("HELLO");
            }

            else if (CMD_SWITCH(CMD_RESET, uart_buffer))
            {
                CMD_PRINT("SOFT RESET");
                _delay_ms(10);

                bsp_soft_reset();
            }

            else
            {
                CMD_PRINT("UNKNOWN COMMAND");
            }
        }
    }
}

int main()
{
    setup();
    system_info();

    loop();

    return 0;
}
