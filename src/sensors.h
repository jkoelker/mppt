/*
 * vim: set ft=arduino:
 */
#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>
#include <avr/interrupt.h>

#include "config.h"

uint32_t amperage(uint8_t pin);
uint32_t voltage(uint8_t pin);


inline void __attribute__((always_inline)) sensors_init()
{
    // Disable digital input on the analog pins
    DIDR0 = (uint8_t)(_BV(BATTERY_AMP_PIN)  |
                      _BV(BATTERY_VOLT_PIN) |
                      _BV(SOURCE_AMP_PIN)   |
                      _BV(SOURCE_VOLT_PIN));

    ADMUX = (uint8_t)(_BV(REFS0) | // Default arduino mode
                      _BV(MUX3) | _BV(MUX2) | _BV(MUX1)); // Start with vcc

    ADCSRB = 0x00; // Free run mode
    ADCSRA = (uint8_t)(_BV(ADEN)  | // ADC enable
                       _BV(ADSC)  | // ADC start
                       _BV(ADATE) | // Auto trigger
                       _BV(ADIE)  | // Interrupt enable
                       _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0)); // prescaler=128
}


#endif
