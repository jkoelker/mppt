/*
 * vim: set ft=arduino:
 */

#ifndef MPPT_H
#define MPPT_H

#include <stdint.h>
#include "config.h"


#define _EMA(c, p, s) p + (2.0 / (s + 1)) * (c - p)
#if EMA_SPAN >= 1
#define EMA(c, p, s) _EMA(c, p, s)
#else
#define EMA(c, p, s) (c)
#endif


enum mode {
    off, on, bulk_charge, float_charge
} state;


uint32_t battery_amps;
uint32_t battery_volt;
uint32_t source_amps;
uint32_t source_volt;
uint32_t battery_watts;
uint32_t source_watts;


void mppt_init();
void mppt();


#endif
