#ifndef CHIP_FUSE_BITS_H
#define CHIP_FUSE_BITS_H

#pragma config ICS = 2     // debug communication via ICD channel 2
#pragma config FNOSC = FRC    // internal FRC clock is startup clock
#pragma config IESO = OFF     // don't auto switch clock, startup with FNOSC
#pragma config POSCMD = HS    // primary oscillator in high-speed mode
#pragma config FCKSM = CSECMD // enable clock switching

#endif