#ifndef INIT_HARDWARE_H
#define INIT_HARDWARE_H

#include <stdint.h>

#define PIN_OUTPUT ((uint16_t)0)
#define PIN_INPUT ((uint16_t)1)

void init_spi_io();
void init_timer1();
void init_interrupts();
void init_primary_pll();

int setup_pwm();
void setup_dac();

#endif