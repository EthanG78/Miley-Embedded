#include "init_hardware.h"

#include <xc.h>

void init_spi_io(void) {
  // SPI RELATED TRIS
  TRISCbits.TRISC14 = PIN_INPUT;   // RC14 SDI1 input
  TRISCbits.TRISC15 = PIN_OUTPUT;  // RC15 SDO1 output
  TRISBbits.TRISB13 = PIN_OUTPUT;  // RB13 SCK1 output
  TRISBbits.TRISB14 = PIN_OUTPUT;  // RB14 CS output

  // SD Card Trigger Button
  TRISCbits.TRISC12 = PIN_INPUT;

  // Status LED (default off)
  TRISBbits.TRISB15 = PIN_OUTPUT;
  LATBbits.LATB15 = 0;

  __builtin_write_RPCON(0x0000);  // unlock PPS

  RPOR6bits.RP45R = 6;     // RB13->SPI1:SCK1OUT
  RPINR20bits.SDI1R = 62;  // RC14->SPI1:SDI1

  __builtin_write_RPCON(0x0800);  // lock PPS
}

void init_timer1() {
  T1CONbits.TMWDIS = 0;  // allow back to back writes
  T1CONbits.TCKPS = 0;   // divide input clock by 1
  T1CONbits.TECS = 2;    // use internal Fosc clock
  T1CONbits.TON = 1;     // timer on
  PR1 = 3200;            // cycles before interrupt -> 40KHz
}

void init_interrupts() {
  INTCON1bits.NSTDIS = 0;  // allow nested interrupts
  IPC0bits.T1IP = 1;       // timer1 interrupt priority = 1
  IFS0bits.T1IF = 0;       // reset timer1 interrupt flag
  IEC0bits.T1IE = 1;       // enable timer 1 interrupt
  INTCON2bits.GIE = 1;     // enable global interrupts
}

void init_primary_pll() {
  CLKDIVbits.PLLPRE = 4;    // PLL input clock div
  PLLFBDbits.PLLFBDIV = 16;  // PLL feedback div
  PLLDIVbits.POST1DIV = 1;  // F_PLL output div1
  PLLDIVbits.POST2DIV = 1;  // F_PLL output div2
}

int setup_pwm() {
  PCLKCONbits.DIVSEL = 0;   // 1:2 divide ratio (smallest)
  PCLKCONbits.MCLKSEL = 3;  // master PWM clock is AF_PLLo
  PG8CONLbits.CLKSEL = 1;   // PWM module uses master PWM clock
  PG8CONLbits.HREN = 1;     // enable high resolution mode
  PG8CONHbits.SOCS = 0;     // PWM period self-triggers after previous completes
  PG8CONHbits.TRGMOD = 0;   // module operates on single trigger
  PG8IOCONHbits.PMOD = 1;   // operate in independent mode
  PG8IOCONHbits.PENH = 0;   // pwm output does not drive PWM8H pin
  PG8IOCONHbits.PENL = 1;   // pwm output drives PMW8L pin
  PG8IOCONHbits.POLL = 0;   // active high output
  PG8EVTLbits.UPDTRG = 1;   // writes to PGxDC register sets the UPDATE bit
  PG8PER = ~0;              // pwm period -> max out 16bits
  PG8DC = 1000;             // pwm duty cycle
  PG8PHASE = 0;             // 0 phase shift
  PG8CONLbits.ON = 1;       // enable PWM module

  // check for error activating high resolution mode
  if (PCLKCONbits.HRERR) {
    return 1;
  }

  // wait for high resolution circuitry to become available
  while (!PCLKCONbits.HRRDY) {
    continue;
  }

  return 0;
}

void setup_dac() {
  DACCTRL1Lbits.CLKSEL = 2;      // use AF_PLL as DAC clock
  DACCTRL1Lbits.CLKDIV = 0;      // divide input clock by 1
  DAC1CONLbits.DACOEN = 1;       // enable DAC 1 output on pin DACOUT1
  DAC1CONLbits.DACEN = 1;        // enable Master DAC 1
  DACCTRL1Lbits.DACON = 1;       // turn ON all Master DACs */
  DAC1DATHbits.DACDATH = 0x4D9;  // dac initial output value
}
