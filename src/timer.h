/*
 * vim: set ft=arduino:
 */


#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"


inline void __attribute__((always_inline)) timer_init()
{
    // CTC Mode
    TCCR0A = (uint8_t)_BV(WGM01);

    // Prescale 256 (16Mhz)
    TCCR0B = (uint8_t)_BV(CS02);

    // Enable Compare Match
    TIMSK0 = (uint8_t)_BV(OCIE0A);

    // Match value
    OCR0A = (uint8_t)(F_CPU / 256000);
}

uint32_t millis();

#endif
