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

#include "peripheral/ring.h"

#define RING_INC_ROLL_OVER(n, s, e) (((n)+1>=(e)) ? (s) : (n)+1)

ring_t ring_init(
    uint8_t *buf, 
    size_t len
)
{
    return RING_INIT(buf, len);
}

bool ring_is_empty(
    ring_t ring
)
{
    return ring.read == ring.write;
}

bool ring_is_full(
    ring_t ring
)
{
    return RING_INC_ROLL_OVER(ring.write, ring.buf, ring.end) == ring.read;
}

size_t ring_push_available(
    ring_t ring
)
{
    if(ring.write < ring.read)
        return ring.read - ring.write - 1;
    else
        return (ring.end - ring.buf) - (ring.write - ring.read) - 1;
}

size_t ring_pop_available(
    ring_t ring
)
{
    if(ring.read <= ring.write)
        return ring.write - ring.read;
    else
        return (ring.end - ring.buf) - (ring.read - ring.write);
}

bool ring_push(
    ring_t *ring,
    uint8_t data
)
{
    if(ring_is_full(*ring))
        return 1;
    
    *ring->write = data;
    ring->write = RING_INC_ROLL_OVER(ring->write, ring->buf, ring->end);
    
    return 0;
}

bool ring_push_over(
    ring_t *ring,
    uint8_t data
)
{
    *ring->write = data;
    ring->write = RING_INC_ROLL_OVER(ring->write, ring->buf, ring->end);
    
    if(ring->read == ring->write)
    {
        ring->read = RING_INC_ROLL_OVER(ring->read, ring->buf, ring->end);
        return 1;
    }
    
    return 0;
}

bool ring_pop(
    ring_t *ring,
    uint8_t *data
)
{
    if(ring_is_empty(*ring))
        return 1;
    
    *data = *ring->read;
    ring->read = RING_INC_ROLL_OVER(ring->read, ring->buf, ring->end);
    
    return 0;
}

bool ring_peek(
    ring_t *ring,
    uint8_t *data
)
{
    if(ring_is_empty(*ring))
        return 1;
    
    *data = *ring->read;
    
    return 0;
}
