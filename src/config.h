/*
 * vim: set ft=arduino:
 */
#ifndef CONFIG_H
#define CONFIG_H

// All Values in mV or mW
#ifndef LOW_SOURCE_MWATTS
#define LOW_SOURCE_MWATTS 1000
#endif

#ifndef MIN_SOURCE_MWATTS
#define MIN_SOURCE_MWATTS 500
#endif

#ifndef MIN_BATTERY_MVOLTS
#define MIN_BATTERY_MVOLTS 11000
#endif

#ifndef MAX_BATTERY_MVOLTS
#define MAX_BATTERY_MVOLTS 14100
#endif

#ifndef HIGH_BATTERY_MVOLTS
#define HIGH_BATTERY_MVOLTS 13000
#endif

// ADC Measurement Pins. 0 == A0 ... 5 == A5
#ifndef BATTERY_AMP_PIN
#define BATTERY_AMP_PIN 0
#endif

#ifndef BATTERY_VOLT_PIN
#define BATTERY_VOLT_PIN 1
#endif

#ifndef SOURCE_AMP_PIN
#define SOURCE_AMP_PIN 2
#endif

#ifndef SOURCE_VOLT_PIN
#define SOURCE_VOLT_PIN 3
#endif

// Number of samples to smooth readings
#ifndef EMA_SPAN
#define EMA_SPAN 50
#endif

#ifndef PWM_FULL
#define PWM_FULL 1023
#endif

#ifndef PWM_MAX
#define PWM_MAX PWM_FULL
#endif

// 60%
#ifndef PWM_MIN
#define PWM_MIN 613
#endif

// 90%
#ifndef PWM_START
#define PWM_START 920
#endif

// the value the increment to the pwm value for the ppt algorithm
#ifndef PWM_INCREMENT
#define PWM_INCREMENT 1
#endif

#ifndef PWM_PERIOD
#define PWM_PERIOD 20
#endif


// 1125300 = 1.1*1023*1000
#ifndef READVCC_CALIBRATION_CONST
#define READVCC_CALIBRATION_CONST 1125300
#endif

// Source mV per Amp
#ifndef SOURCE_MVA
#define SOURCE_MVA 66
#endif

// Battery mV per Amp
#ifndef BATTERY_MVA
#define BATTERY_MVA 66
#endif

#ifndef TIMER_MS
#define TIMER_MS 20
#endif

#ifndef BAUDRATE
#define BAUDRATE 9600
#endif

// Must be a power of 2
#ifndef UART_TX0_BUFFER_SIZE
#define UART_TX0_BUFFER_SIZE 64
#endif

// Must be a power of 2
#ifndef UART_RX0_BUFFER_SIZE
#define UART_RX0_BUFFER_SIZE 64
#endif

#endif
