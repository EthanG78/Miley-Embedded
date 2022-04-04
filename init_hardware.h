#ifndef INIT_HARDWARE_H
#define INIT_HARDWARE_H

#include <stdint.h>

#define PIN_OUTPUT ((uint16_t)0)
#define PIN_INPUT ((uint16_t)1)

void init_interrupts();
void init_digital_input();
void init_digital_outputs();
void init_primary_pll();
void init_timer1();
void init_dac();
void init_adc();
void init_spi_io();
int init_pwm();

#endif