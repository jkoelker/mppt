/*
 * vim: set ft=arduino:
 */
#include <inttypes.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "mppt.h"
#include "timer.h"
#include "uart.h"



static void init()
{
    // Disable Timer2
    TIMSK2 = 0x00;

    mppt_init();
    timer_init();
    uart0_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
}


static void sendln(const char *s)
{
    uart0_puts(s);
    uart0_putc('\r');
    uart0_putc('\n');
}


static void report()
{
    sendln("Hi");
}


int main()
{
    uint32_t current_time;
    uint32_t last_report = 0;
    init();

    // Enable Interrupts
    sei();

    for (;;) {
        mppt();

        current_time = millis();
        if (current_time != last_report && (current_time % 5000) == 0) {
            report();
            last_report = current_time;
        }
        /*
        printf("%d", state);
        printf("%lu",(unsigned long) battery_amps);
        printf("%lu", (unsigned long)battery_volt);
        printf("%lu", (unsigned long)source_amps);
        printf("%lu", (unsigned long)source_volt);
        printf("%lu", (unsigned long)battery_watts);
        printf("%lu", (unsigned long)source_watts);
        */
    };
}
