/*
 * vim: set ft=arduino:
 */
#include "config.h"
#include "mppt.h"
#include "pwm.h"
#include "sensors.h"

#define SMOOTH(c, p) EMA(c, p, EMA_SPAN)
#define SENSOR_UPDATE() \
    battery_amps = SMOOTH(amperage(BATTERY_AMP_PIN), battery_amps); \
    battery_volt = SMOOTH(voltage(BATTERY_VOLT_PIN), battery_volt); \
    battery_watts = battery_amps * battery_volt;                    \
    source_amps = SMOOTH(amperage(SOURCE_AMP_PIN), source_amps);    \
    source_volt = SMOOTH(voltage(SOURCE_VOLT_PIN), source_volt);    \
    source_watts = source_amps * source_volt


static uint16_t _pwm;
static uint16_t _bulk_delta;
static uint32_t _last_source_watts;


static void pwm(uint16_t duty)
{
    _pwm = duty;
    pwm_duty(_pwm);
}

static void state_off()
{
    pwm_stop();
    state = off;
}


static void state_float_charge()
{
    pwm(PWM_MIN);

    if (state == off) {
        pwm_start();
    }

    state = float_charge;
}


static void state_on()
{
    pwm(PWM_START);

    if (state == off) {
        pwm_start();
    }

    state = on;
}


static void state_bulk_charge()
{
    _bulk_delta = PWM_INCREMENT;
    state = bulk_charge;
}


static void mppt_off()
{
    if ((battery_volt > HIGH_BATTERY_MVOLTS) &&
            (battery_volt < MAX_BATTERY_MVOLTS) &&
            (source_volt > battery_volt)) {
        state_float_charge();

    } else if ((battery_volt > MIN_BATTERY_MVOLTS) &&
               (battery_volt < MAX_BATTERY_MVOLTS) &&
               (source_volt > battery_volt)) {
        state_on();
    }
}


static void mppt_on()
{
    if (source_watts < MIN_SOURCE_MWATTS) {
        state_off();

    } else if (battery_volt > MAX_BATTERY_MVOLTS) {
        state_float_charge();

    } else if (source_watts < LOW_SOURCE_MWATTS) {
        pwm(PWM_MAX);

    } else {
        double duty = (double)battery_volt / (double)source_volt;
        pwm((uint16_t)(duty * PWM_FULL));
        state_bulk_charge();
    }
}


static void mppt_bulk()
{
    if (source_watts < MIN_SOURCE_MWATTS) {
        state_off();

    } else if (battery_volt > MAX_BATTERY_MVOLTS) {
        state_float_charge();

    } else if (source_watts < LOW_SOURCE_MWATTS) {
        state_on();

    } else {
        if (_last_source_watts >= source_watts) {
            _bulk_delta = -_bulk_delta;
        }
        _last_source_watts = source_watts;
        pwm((uint16_t)(_pwm + _bulk_delta));
    }
}


static void mppt_float()
{
    if (source_watts < MIN_SOURCE_MWATTS) {
        state_off();
        state = off;

    } else if (battery_volt > MAX_BATTERY_MVOLTS) {
        pwm(_pwm - 1);

    } else if (battery_volt < MAX_BATTERY_MVOLTS) {
        uint16_t duty = _pwm + 1;

        if (duty >= PWM_FULL) {
            state_bulk_charge();

        } else {
            pwm(duty);
        }
    }
}


void mppt_init()
{
    battery_amps = 0;
    battery_volt = 0;
    source_amps = 0;
    source_volt = 0;
    battery_watts = 0;
    source_watts = 0;
    state = on;

    sensors_init();
    pwm_init();

    pwm_period(PWM_PERIOD);
}


void mppt()
{
    SENSOR_UPDATE();
    switch (state) {
    case off:
        mppt_off();
        break;

    case on:
        mppt_on();
        break;

    case bulk_charge:
        mppt_bulk();
        break;

    case float_charge:
        mppt_float();
        break;
    }
}
