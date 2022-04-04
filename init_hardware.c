#include "init_hardware.h"

#include <xc.h>
#include <stdbool.h>

#define SINGLE_INIT(ret)          \
  static int initialized = false; \
  if (initialized) {              \
    return ret;                   \
  }                               \
  initialized = true;             

void init_interrupts() {
  SINGLE_INIT()
  INTCON1bits.NSTDIS = 0;  // allow nested interrupts
  INTCON2bits.GIE = 1;     // enable global interrupts
}

void init_digital_input() {
  SINGLE_INIT()
  ANSELCbits.ANSELC3 = 0;  // CS_2 is a digital pin
  TRISCbits.TRISC3 = 1;    // CS_2 is an input
}

void init_digital_outputs() {
  SINGLE_INIT()
  // Controls gain stage with gain 0: Digital pin
  TRISDbits.TRISD3 = 0; // Output pin SDA_1
    
  // Controls gain stage -1.82: Digital pin
  TRISDbits.TRISD2 = 0; // Output pin SCL_1
    
  //Controls gain stage -1.199: Digital pin
  TRISBbits.TRISB11 = 0; // Output pin TX_1

  // Controls gain stage -1.03: Digital pin
  TRISCbits.TRISC14 = 0; // Output pin RX_1
}

void init_primary_pll() {
  SINGLE_INIT()
  CLKDIVbits.PLLPRE = 4;     // PLL input clock div
  PLLFBDbits.PLLFBDIV = 16;  // PLL feedback div
  PLLDIVbits.POST1DIV = 1;   // F_PLL output div1
  PLLDIVbits.POST2DIV = 1;   // F_PLL output div2

  // initiate clock switch to primary oscillator with PLL (NOSC=0b011)
  __builtin_write_OSCCONH(0x03);
  __builtin_write_OSCCONL(OSCCON | 0x01);
  // wait for Clock switch to occur
  while (OSCCONbits.OSWEN != 0)
    ;

  // wait for PLL to lock
  while (OSCCONbits.LOCK != 1)
    ;
}

void init_timer1() {
  SINGLE_INIT()
  IPC0bits.T1IP = 1;     // timer1 interrupt priority = 1
  IFS0bits.T1IF = 0;     // reset timer1 interrupt flag
  IEC0bits.T1IE = 1;     // enable timer 1 interrupt
  T1CONbits.TMWDIS = 0;  // allow back to back writes
  T1CONbits.TCKPS = 0;   // divide input clock by 1
  T1CONbits.TECS = 2;    // use internal Fosc clock
  T1CONbits.TON = 1;     // timer on
  PR1 = 800;             // cycles before interrupt -> 40KHz
}

void init_dac() {
  SINGLE_INIT()
  DACCTRL1Lbits.CLKSEL = 2;      // use AF_PLL as DAC clock
  DACCTRL1Lbits.CLKDIV = 0;      // divide input clock by 1
  DAC1CONLbits.DACOEN = 1;       // enable DAC 1 output on pin DACOUT1
  DAC1CONLbits.DACEN = 1;        // enable Master DAC 1
  DACCTRL1Lbits.DACON = 1;       // turn ON all Master DACs */
  DAC1DATHbits.DACDATH = 0x4D9;  // dac initial output value
}

void init_adc() {
  SINGLE_INIT()
  // SET TO CS_1 AND SELECT ALTERNATE PIN
  ANSELBbits.ANSELB2 = 1;  // CS_1 is an analog pin
  TRISBbits.TRISB2 = 1;    // CS_1 is an input
  ADCON4Hbits.C0CHS = 1;   // Select alternate pin for ADC0

  ADCON3Hbits.CLKSEL = 0;     // Clock from Peripheral oscillator
  ADCON3Hbits.CLKDIV = 0b11;  // No clock divider
  ADCORE0Hbits.ADCS = 0;      // clock divider 1:2
  ADCON3Lbits.REFSEL = 0;     // AVdd is voltage reference
  ADCORE0Hbits.RES = 0b11;    // 12 bit resolution
  ADCON1Hbits.FORM = 0;       // Integer format output
  ADMOD0Lbits.SIGN0 = 0;      // Unsigned
  ADMOD0Lbits.DIFF0 = 0;      // single ended input

  // Turning on dedicated core 0
  ADCON5Hbits.WARMTIME = 0b11111;  // Initialization time to max
  ADCON1Lbits.ADON = 1;            // Turn on ADC module
  ADCON5Lbits.C0PWR = 1;           // Turn on analog power for core 0
  while (ADCON5Lbits.C0RDY == 0)
    ;                    // Wait until core 0 is ready
  ADCON3Hbits.C0EN = 1;  // Turn on digital power to enable triggers to core 0

  // Configure ADC interrupts
  ADIELbits.IE0 = 1;  // enable interrupt
  _ADCAN0IF = 0;      // clear interrupt flag
  _ADCAN0IE = 1;      // Enable interrupt

  ADTRIG0Lbits.TRGSRC0 = 1;  // Common software trigger
}

void init_spi_io(void) {
  SINGLE_INIT()
  TRISBbits.TRISB14 = PIN_INPUT;   // SDI1 input
  TRISCbits.TRISC12 = PIN_OUTPUT;  // SDO1 output
  TRISBbits.TRISB13 = PIN_OUTPUT;  // SCK1 output
  TRISBbits.TRISB15 = PIN_OUTPUT;  // CS output

  __builtin_write_RPCON(0x0000);  // unlock PPS

  RPOR6bits.RP45R = 6;     // RB13->SPI1:SCK1OUT
  RPINR20bits.SDI1R = 46;  // RB14->SPI1:SDI1

  __builtin_write_RPCON(0x0800);  // lock PPS
}

int init_pwm() {
  SINGLE_INIT(1)
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


