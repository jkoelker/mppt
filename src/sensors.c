/*
 * vim: set ft=arduino:
 */
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "config.h"
#include "sensors.h"


static volatile uint16_t _amperage_battery = 0;
static volatile uint16_t _amperage_source = 0;
static volatile uint16_t _voltage_source = 0;
static volatile uint16_t _voltage_battery = 0;
static volatile uint16_t _vcc = 0;

#define AMP_BATTERY_ADMUX  (uint8_t)(_BV(REFS0) | _BV(BATTERY_AMP_PIN))
#define AMP_SOURCE_ADMUX   (uint8_t)(_BV(REFS0) | _BV(SOURCE_VOLT_PIN))
#define VCC_ADMUX          (uint8_t)(_BV(REFS0) | \
                                     _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
#define VOLT_BATTERY_ADMUX (uint8_t)(_BV(REFS0) | _BV(BATTERY_VOLT_PIN))
#define VOLT_SOURCE_ADMUX  (uint8_t)(_BV(REFS0) | _BV(SOURCE_AMP_PIN))

#define VCC() READVCC_CALIBRATION_CONST / _vcc
#define VOLTAGE(v) v / 1024.0 * VCC()
// Simplified ((VOLTAGE(a) - (VCC()/ 2)) / mva) * 1000
#define AMPERAGE(a, mva) (500 * VOLTAGE(a) - VCC()) / mva


uint32_t amperage(uint8_t pin)
{
    switch (pin) {
    case BATTERY_AMP_PIN:
        return AMPERAGE(_amperage_battery, BATTERY_MVA);

    case SOURCE_AMP_PIN:
        return AMPERAGE(_amperage_source, SOURCE_MVA);
    }

    return 0;
}


uint32_t voltage(uint8_t pin)
{
    switch (pin) {
    case BATTERY_VOLT_PIN:
        return VOLTAGE(_voltage_battery);

    case SOURCE_VOLT_PIN:
        return VOLTAGE(_voltage_source);
    }

    return 0;
}


ISR(ADC_vect)
{
    switch (ADMUX) {
    case VCC_ADMUX:
        ADMUX = AMP_BATTERY_ADMUX;
        _vcc = ADC;
        break;

    case AMP_BATTERY_ADMUX:
        ADMUX = AMP_SOURCE_ADMUX;
        _amperage_battery = ADC;
        break;

    case AMP_SOURCE_ADMUX:
        ADMUX = VOLT_BATTERY_ADMUX;
        _amperage_source = ADC;
        break;

    case VOLT_BATTERY_ADMUX:
        ADMUX = VOLT_SOURCE_ADMUX;
        _voltage_battery = ADC;
        break;

    case VOLT_SOURCE_ADMUX:
        ADMUX = VCC_ADMUX;
        _voltage_source = ADC;
        break;
    }
}
