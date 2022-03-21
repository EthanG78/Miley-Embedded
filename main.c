/*
 * File:   main.c
 * Author: ethang
 *
 * Created on March 21, 2022, 12:05 PM
 */
// For debugging
#pragma config ICS = 2

#include "xc.h"
#include "spi_driver.h"
#include "FatFS/ff.h"

#define FCY 4000000UL
#include <libpic30.h>

// Initializes system I/O, 
void init(void)
{
    // SPI RELATED TRIS 
    TRISCbits.TRISC14 = 1;          // RC14 SDI1 input
    TRISCbits.TRISC15 = 0;          // RC15 SDO1 output
    TRISBbits.TRISB13 = 0;          // RB13 SCK1 output
    TRISBbits.TRISB14 = 0;          // RB14 CS output
    
    // SD Card Trigger Button
    TRISCbits.TRISC12 = 1;
    
    // Status LED (default off)
    TRISBbits.TRISB15 = 0;
    LATBbits.LATB15 = 0;
    
    __builtin_write_RPCON(0x0000);  // unlock PPS

    RPOR6bits.RP45R = 6;            //RB13->SPI1:SCK1OUT
    RPINR20bits.SDI1R = 62;         //RC14->SPI1:SDI1
    //RPOR15bits.RP63R = 5;           //RC15->SPI1:SDO1

    __builtin_write_RPCON(0x0800);  // lock PPS
}

// Test FatFS functionality
int fatfs_read_test(void)
{
    FATFS fs;
    FIL fsrc;
    BYTE buffer[4096];
    UINT br;
    
    FRESULT fm = f_mount(&fs, "", 0);
    if (fm == FR_OK)
    {
        FRESULT fo = f_open(&fsrc, "test.txt", FA_READ);
        if (fo == FR_OK)
        {
            return 1;
            FRESULT fr;
            for (;;)
            {
                fr = f_read(&fsrc, buffer, sizeof buffer, &br);  
                if (br == 0) break; /* error or eof */
            }
        }
    }
    
    f_close(&fsrc);
    f_unmount("0:");
    
    return 0;
}

int spi_read_write_test(void)
{
    // Initialize to 125 KHz
    spi_open_initializer();
    
    __delay_ms(200);
    
    spi_exchangeByte(0xEB);
    
    uint8_t rx = spi_exchangeByte(0xFF);
    
    spi_close();
    
    return 1;
}

int main(void) {
    init();
    
    // Only let FatFS test run once
    int hasRun = 0;
    
    LATCbits.LATC15 = 1;
        
    while (1)
    {
        if (PORTCbits.RC12 == 0 && !hasRun)
        {
            hasRun++;
            LATBbits.LATB15 = fatfs_read_test();
            //LATBbits.LATB15 = spi_read_write_test();
        }
    }
    return 1; 
}
