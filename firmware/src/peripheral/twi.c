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

#include "peripheral/twi.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

#define TWBR_VALUE ((F_CPU/TWI_FREQUENCY - 16) / (2 * TWI_PRESCALER))

static uint8_t twi_address;
static uint8_t* twi_data;
static size_t twi_index;
static size_t twi_len;

void twi_init(void)
{
    TWBR = TWBR_VALUE;
    TWSR = (TWPS1_VALUE << TWPS1) | (TWPS0_VALUE << TWPS0);
    
    TWCR = (1 << TWINT) | (1 << TWEN);
}

bool twi_busy(void)
{
    return TWCR & (1<<TWIE);
}

void twi_flush(void)
{
    while(TWCR & (1<<TWIE));
}

void twi_start(
    uint8_t address,
    uint8_t *data,
    size_t len
)
{
    twi_flush();
    
    twi_address = address;
    twi_data = data;
    twi_len = len;
    
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

ISR(TWI_vect)
{
    switch(TW_STATUS)
    {
        case TW_START:
        case TW_REP_START:
            twi_index = 0;
            TWDR = twi_address;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
            break;
        
        case TW_MT_SLA_ACK:
        case TW_MT_DATA_ACK:
            if(twi_index < twi_len)
            {
                TWDR = twi_data[twi_index++];
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
            }
            else
            {
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
            }
            break;
        
        case TW_MR_DATA_ACK:
            twi_data[twi_index++] = TWDR;
        case TW_MR_SLA_ACK:
            if(twi_index < twi_len-1)
            {
                TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
            }
            else
            {
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
            }
            break;
        
        case TW_MR_DATA_NACK:
            twi_data[twi_index++] = TWDR;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
            break;
        
        case TW_MT_ARB_LOST:
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
            break;
        
        case TW_MT_SLA_NACK:
        case TW_MT_DATA_NACK:
        case TW_MR_SLA_NACK:
        default:
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    }
}
