/*
 * vim: set ft=arduino:
 */

#include <stdint.h>
#include <util/atomic.h>
#include "timer.h"


static volatile uint32_t milliseconds = 0;


ISR(TIMER0_COMPA_vect)
{
    ++milliseconds;
}


uint32_t millis()
{
    uint32_t ms;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ms = milliseconds;
    }

    return ms;
}
