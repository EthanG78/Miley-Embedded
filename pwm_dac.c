//
//// DSPIC33CK256MP506 Configuration Bit Settings
//
//// 'C' source line config statements
//
//// FSEC
//#pragma config BWRP = OFF               // Boot Segment Write-Protect bit (Boot Segment may be written)
//#pragma config BSS = DISABLED           // Boot Segment Code-Protect Level bits (No Protection (other than BWRP))
//#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
//#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
//#pragma config GSS = DISABLED           // General Segment Code-Protect Level bits (No Protection (other than GWRP))
//#pragma config CWRP = OFF               // Configuration Segment Write-Protect bit (Configuration Segment may be written)
//#pragma config CSS = DISABLED           // Configuration Segment Code-Protect Level bits (No Protection (other than CWRP))
//#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table bit (Disabled AIVT)
//
//// FBSLIM
//#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits (Enter Hexadecimal value)
//
//// FSIGN
//
//// FOSCSEL
//#pragma config FNOSC = FRC              // Oscillator Source Selection (Primary Oscillator (XT, HS, EC))
//#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)
//
//// FOSC
//#pragma config POSCMD = HS              // Primary Oscillator Mode Select bits (HS Crystal Oscillator Mode)
//#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
//#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are enabled)
//#pragma config PLLKEN = ON              // PLL Lock Status Control (PLL lock signal will be used to disable PLL clock output if lock is lost)
//#pragma config XTCFG = G3               // XT Config (24-32 MHz crystals)
//#pragma config XTBST = ENABLE           // XT Boost (Boost the kick-start)
//
//// FWDT
//#pragma config RWDTPS = PS2147483648    // Run Mode Watchdog Timer Post Scaler select bits (1:2147483648)
//#pragma config RCLKSEL = LPRC           // Watchdog Timer Clock Select bits (Always use LPRC)
//#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer operates in Window mode)
//#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
//#pragma config SWDTPS = PS2147483648    // Sleep Mode Watchdog Timer Post Scaler select bits (1:2147483648)
//#pragma config FWDTEN = ON_SW           // Watchdog Timer Enable bit (WDT controlled via SW, use WDTCON.ON bit)
//
//// FPOR
//#pragma config BISTDIS = DISABLED       // Memory BIST Feature Disable (mBIST on reset feature disabled)
//
//// FICD
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGC2 and PGD2)
//#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
//#pragma config NOBTSWP = DISABLED       // BOOTSWP instruction disable bit (BOOTSWP instruction is disabled)
//
//// FDMTIVTL
//#pragma config DMTIVTL = 0x0            // Dead Man Timer Interval low word (Enter Hexadecimal value)
//
//// FDMTIVTH
//#pragma config DMTIVTH = 0x0            // Dead Man Timer Interval high word (Enter Hexadecimal value)
//
//// FDMTCNTL
//#pragma config DMTCNTL = 0x0            // Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)
//
//// FDMTCNTH
//#pragma config DMTCNTH = 0x0            // Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Enter Hexadecimal value)
//
//// FDMT
//#pragma config DMTDIS = OFF             // Dead Man Timer Disable bit (Dead Man Timer is Disabled and can be enabled by software)
//
//// FDEVOPT
//#pragma config ALTI2C1 = OFF            // Alternate I2C1 Pin bit (I2C1 mapped to SDA1/SCL1 pins)
//#pragma config ALTI2C2 = OFF            // Alternate I2C2 Pin bit (I2C2 mapped to SDA2/SCL2 pins)
//#pragma config ALTI2C3 = OFF            // Alternate I2C3 Pin bit (I2C3 mapped to SDA3/SCL3 pins)
//#pragma config SMBEN = SMBUS            // SM Bus Enable (SMBus input threshold is enabled)
//#pragma config SPI2PIN = PPS            // SPI2 Pin Select bit (SPI2 uses I/O remap (PPS) pins)
//
//// FALTREG
//#pragma config CTXT1 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits (Not Assigned)
//#pragma config CTXT2 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (Not Assigned)
//#pragma config CTXT3 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits (Not Assigned)
//#pragma config CTXT4 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits (Not Assigned)
//
//// FBTSEQ
//#pragma config BSEQ = 0xFFF             // Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active (Enter Hexadecimal value)
//#pragma config IBSEQ = 0xFFF            // The one's complement of BSEQ; must be calculated by the user and written during device programming. (Enter Hexadecimal value)
//
//// #pragma config statements should precede project file includes.
//// Use project enums instead of #define for ON and OFF.
//
//#include <xc.h>
//
//int setup_pwm() {
//  PCLKCONbits.DIVSEL = 0;  // 1:2 divide ratio
//  PCLKCONbits.MCLKSEL = 0; // master PWM clock is Fosc
//  PG8CONLbits.CLKSEL = 1; // PWM module uses master PWM clock
//  PG8CONHbits.SOCS = 0;   // PWM period self-triggers after previous completes
//  PG8CONHbits.TRGMOD = 1; // module is retriggerable
//  PG8IOCONHbits.PMOD = 1; // operate in independent mode
//  PG8IOCONHbits.PENH = 0; // pwm output does not drive PWM8H pin
//  PG8IOCONHbits.PENL = 1; // pwm output drives PMW8L pin
//  PG8EVTLbits.UPDTRG = 1; // A write of the PGxDC register automatically sets the UPDATE bit
//  PG8PER = 65535; // 2**16-1
//  PG8DC = 30000;
//  PG8PHASE = 0; // 0 phase shift
//  PG8CONLbits.ON = 1; // enable PWM module
//  return 0;
//}
//
//int setup_pll() {
//  // Configure PLL prescaler, both PLL postscalers, and PLL feedback divider
//  CLKDIVbits.PLLPRE = 1; // N1=1
//  PLLDIVbits.POST1DIV = 1; // N2=1
//  PLLDIVbits.POST2DIV = 1; // N3=1
//  PLLFBDbits.PLLFBDIV = 200; // M = 125
//}
//
//
//int main(void)
//{
//  TRISCbits.TRISC13 = 0;
//  TRISCbits.TRISC12 = 0;
//  
//  setup_pll();
//  
//  // Initiate Clock Switch to FRC with PLL (NOSC=0b001)
//  __builtin_write_OSCCONH(0x01);
//  __builtin_write_OSCCONL(OSCCON | 0x01);
//  
//  // Wait for Clock switch to occur
//  while (OSCCONbits.OSWEN!= 0);
//  
//  // Wait for PLL to lock
//  //while (OSCCONbits.LOCK!= 1);
//  
//  setup_pwm();
//    
//  while (1)
//  {
//        
//  }
//  return 1; 
//}

//code example for 50 MIPS system clock using POSC with 10 MHz external crystal
// Select FRC on POR
#pragma config FNOSC = FRC // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config IESO = OFF
/// Enable Clock Switching and Configure POSC in XT mode
#pragma config POSCMD = HS
#pragma config FCKSM = CSECMD

#include <xc.h>
#include <math.h>
#include <stdlib.h>

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
  PG8EVTLbits.UPDTRG = 1;   // A write of the PGxDC register automatically sets the UPDATE bit
  PG8PER = 65536; // 2**16-1
  PG8DC =  1000;//PG8PER/2;
  PG8PHASE = 0; // 0 phase shift
  PG8CONLbits.ON = 1; // enable PWM module
  
  if(PCLKCONbits.HRERR) {
    return 1;
  }
  
  // wait for high resolution circuitry to become available
  while(PCLKCONbits.HRRDY != 1);
  return 0;
}

int setup_dac() {
  DACCTRL1Lbits.CLKSEL = 2; // use AF_PLL as DAC clock
  DACCTRL1Lbits.CLKDIV = 0; // divide input clock by 1
  DAC1CONLbits.DACOEN = 1; /* Enable DAC 1 output on pin DACOUT1 */
  DAC1CONLbits.DACEN = 1; /* Enable Master DAC 1 */
  DACCTRL1Lbits.DACON = 1; /* Turn ON all Master DACs */
  DAC1DATHbits.DACDATH = 0x4D9; // set output to 1v
  return 0;
}

void switch_to_primary_oscillator() {
  // Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)
  __builtin_write_OSCCONH(0x03);
  __builtin_write_OSCCONL(OSCCON | 0x01);
  // Wait for Clock switch to occur
  while (OSCCONbits.OSWEN!= 0);
  
  // Wait for PLL to lock
  while (OSCCONbits.LOCK!= 1);
}

void init_timer1() {
  T1CONbits.TMWDIS = 0; // allow back to back writes
  T1CONbits.TCKPS = 0;  // divide input clock by 1
  T1CONbits.TECS = 2;   // use internal Fosc clock
  T1CONbits.TON = 1;    // timer on
}

void init_interrupts() {
  INTCON1bits.NSTDIS = 0; // allow nested interrupts
  IPC0bits.T1IP = 1;      // timer1 interrupt priority = 1
  IFS0bits.T1IF = 0;      // reset timer1 interrupt flag
  IEC0bits.T1IE = 1;      // enable timer 1 interrupt
  PR1 = 3200;            // cycles before interrupt -> 40KHz
  INTCON2bits.GIE = 1;     // enable global interrupts
}

void init_primary_pll() {
  // system clock is 32MHz * 16 = 512MHz
  CLKDIVbits.PLLPRE = 1; // N1=1
  PLLFBDbits.PLLFBDIV = 16; // PLL feedback div 
  PLLDIVbits.POST1DIV = 1;  // F_PLL output div1
  PLLDIVbits.POST2DIV = 1;  // F_PLL output div2
}

// OVERCLOCKING THE HIGH_RES PWM MODULE TO THE TITS (factor of 2)
void init_auxiliary_pll() {
  //code example for AFVCO = 1 GHz and AFPLLO = 500 MHz using 8 MHz internal FRC
  // Configure the source clock for the APLL
  ACLKCON1bits.FRCSEL = 0; // Select POSC as the clock source
  // Configure the APLL prescaler, APLL feedback divider, and both APLL postscalers.
  ACLKCON1bits.APLLPRE = 1; // N1 = 1
  APLLFBD1bits.APLLFBDIV = 35; // M = 125
  APLLDIV1bits.APOST1DIV = 1; // N2 = 2
  APLLDIV1bits.APOST2DIV = 1; // N3 = 1
  // Enable APLL
  ACLKCON1bits.APLLEN = 1;
}

// Timer 1 in combination with a timer compare interrupt is used to multiplex
// between the digits on the 7-seg display and to "spin" the reels

static void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
  TMR1 = 0; // reset timer count
  static int sample = 0;
  //DAC1DATHbits.DACDATH = ++sample;
  PG8DC = ++sample;
  IFS0bits.T1IF = 0; // clear Timer1 interrupt flag
}

int main()
{
  init_primary_pll();
  switch_to_primary_oscillator();
  init_auxiliary_pll();
  
  init_interrupts();
  init_timer1();
  //setup_dac();
  setup_pwm();  
  
  while (1)
  {
        
  }
}