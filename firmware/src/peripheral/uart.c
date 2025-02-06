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

#include "peripheral/ring.h"
#include "peripheral/uart.h"

#include "status.h"
#include "cmd.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/setbaud.h>

extern t_system_status system_status;

static int uart_putc(
    char c,
    FILE *stream
)
{
    (void)stream;

    if(uart_tx(c))
        return _FDEV_EOF;
    
    return c;
}

static int uart_getc(
    FILE *stream
)
{
    uint8_t c;
    (void)stream;

    if(uart_rx(&c))
        return _FDEV_EOF;
    
    return c;
}

FILE uart_out = FDEV_SETUP_STREAM(uart_putc, NULL, _FDEV_SETUP_WRITE);
FILE uart_in = FDEV_SETUP_STREAM(NULL, uart_getc, _FDEV_SETUP_READ);

static volatile ring_t uart_tx_ring;
static volatile ring_t uart_rx_ring;

static volatile uint8_t uart_tx_buf[UART_BUF_SIZE];
static volatile uint8_t uart_rx_buf[UART_BUF_SIZE];

void uart_init(void)
{
    uart_tx_ring = ring_init((uint8_t*)uart_tx_buf, UART_BUF_SIZE - 1);
    uart_rx_ring = ring_init((uint8_t*)uart_rx_buf, UART_BUF_SIZE - 1);
    
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    
    stdout = &uart_out;
    stdin = &uart_in;
}

bool uart_tx(
    uint8_t data
)
{
    bool ret;
    
    while(ring_is_full(uart_tx_ring));
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_push((ring_t*)&uart_tx_ring, data);
    }

    if(!ret)
        UCSR0B |= (1 << UDRIE0);
    
    return ret;
}

bool uart_rx(
    uint8_t* data
)
{
    bool ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_pop((ring_t*)&uart_rx_ring, data);
    }
    
    return ret;
}

size_t uart_tx_burst(
    uint8_t *data,
    size_t size
)
{
    size_t i = 0;
    
    while(i<size && !uart_tx(*data++))
        i++;
    
    return i;
}

size_t uart_rx_burst(
    uint8_t* data,
    size_t len
)
{
    size_t i = 0;
    
    while(i<len && !uart_rx(data++))
        i++;
    
    return i;
}

size_t uart_tx_available(void)
{
    size_t ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_push_available(uart_tx_ring);
    }
    
    return ret;
}

size_t uart_rx_available(void)
{
    size_t ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_pop_available(uart_rx_ring);
    }
    
    return ret;
}

bool uart_rx_peek(
    uint8_t *data
)
{
    bool ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_peek((ring_t*)&uart_rx_ring, data);
    }
    
    return ret;
}

void uart_tx_flush(void)
{
    bool empty;
    do
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            empty = ring_is_empty(uart_tx_ring);
        }
    } while(!empty);
}

char *uart_ngets(char *s, size_t n)
{
    uint8_t c;
    static size_t i = 0;
    
    
    while(!uart_rx(&c))
    {
        s[i++] = c;
        
        if(c == '\n' || i >= n-1)
        {
            s[i] = '\0';
            i = 0;
            
            return s;
        }
    }
    
    return NULL;
}

ISR(USART_UDRE_vect)
{
    uint8_t c;
    
    if(!ring_pop((ring_t*)&uart_tx_ring, &c))
        UDR0 = c;

    else
        UCSR0B &= ~(1 << UDRIE0);
}

ISR(USART_RX_vect)
{
    if (system_status.status == SYSTEM_STATUS_BUSY)
    {
        // system busy.. lock now!
        CMD_ERROR("SYSTEM BUSY");
        system_status.status = SYSTEM_STATUS_LOCKED;
    }

    if (system_status.status != SYSTEM_STATUS_LOCKED)
    {
        #ifdef UART_OVERWRITE
            ring_push_over((ring_t*)&uart_rx_ring, UDR0);
        #else
            ring_push((ring_t*)&uart_rx_ring, UDR0);
        #endif
    }
    else{
        (uint8_t)UDR0;
    }
}
