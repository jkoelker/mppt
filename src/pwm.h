/*
 * vim: set ft=arduino:
 */

/*
 * Based on https://github.com/PaulStoffregen/TimerOne
 */

#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef TIMER1_RESOLUTION
#define TIMER1_RESOLUTION 65536
#endif

#define TIMER1_A_PIN 9


uint8_t pwm_clock_select_bits;
uint16_t pwm_current_period;


extern inline __attribute__((always_inline)) void pwm_init()
{
    TCCR1B = (uint8_t)_BV(WGM13);
    TCCR1A = 0;
}


inline __attribute__((always_inline)) void pwm_period(uint32_t ms)
{
    const uint32_t cycles = (F_CPU / 2000000) * ms;

    if (cycles < TIMER1_RESOLUTION) {
        pwm_clock_select_bits = (uint8_t)_BV(CS10);
        pwm_current_period = cycles;

    } else if (cycles < TIMER1_RESOLUTION * 8) {
        pwm_clock_select_bits = (uint8_t)_BV(CS11);
        pwm_current_period = cycles / 8;

    } else if (cycles < TIMER1_RESOLUTION * 64) {
        pwm_clock_select_bits = (uint8_t)(_BV(CS11) | _BV(CS10));
        pwm_current_period = cycles / 64;

    } else if (cycles < TIMER1_RESOLUTION * 256) {
        pwm_clock_select_bits = (uint8_t)_BV(CS12);
        pwm_current_period = cycles / 256;

    } else if (cycles < TIMER1_RESOLUTION * 1024) {
        pwm_clock_select_bits = (uint8_t)(_BV(CS12) | _BV(CS10));
        pwm_current_period = cycles / 1024;

    } else {
        pwm_clock_select_bits = (uint8_t)(_BV(CS12) | _BV(CS10));
        pwm_current_period = TIMER1_RESOLUTION - 1;
    }

    ICR1 = pwm_current_period;
    DDRB |= (1 << DDB1);
    TCCR1A |= _BV(COM1A1);
    TCCR1B = (uint8_t)(_BV(WGM13) | pwm_clock_select_bits);
}


inline __attribute__((always_inline)) void pwm_resume()
{
    TCCR1B = (uint8_t)(_BV(WGM13) | pwm_clock_select_bits);
}


inline __attribute__((always_inline)) void pwm_start()
{
    TCCR1B = 0;
    TCNT1 = 0;
    TCCR1B = (uint8_t)(_BV(WGM13) | pwm_clock_select_bits);
}


inline __attribute__((always_inline)) void pwm_stop()
{
    TCCR1B = (uint8_t)_BV(WGM13);
}


inline __attribute__((always_inline)) void pwm_duty(uint16_t duty)
{
    OCR1A = (pwm_current_period * duty) >> 10;
}


inline __attribute__((always_inline)) void pwm_disable()
{
   TCCR1A &= ~_BV(COM1A1);
}

#endif
