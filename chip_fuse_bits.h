#pragma once

#pragma config ICS = 2     // debug communication via ICD channel 2
#pragma config FNOSC = FRC    // internal FRC clock is startup clock
#pragma config IESO = OFF     // don't auto switch clock, startup with FNOSC
#pragma config FCKSM = CSECMD // enable clock switching
#pragma config POSCMD = HS    // primary oscillator in high-speed mode
