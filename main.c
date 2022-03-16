/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  dsPIC33CK256MP506
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "FatFS/ff.h"
#include "mcc_generated_files/system.h"

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
            FRESULT fr;
            for (;;)
            {
                fr = f_read(&fsrc, buffer, sizeof buffer, &br);
                return 1;
                if (br == 0) break; /* error or eof */
            }
        }
    }
    
    f_close(&fsrc);
    f_unmount("0:");
    
    return 0;
}

int main(void)
{
    SYSTEM_Initialize();
        
    // BUTTON
    TRISCbits.TRISC12 = 1;
    // LED
    TRISBbits.TRISB15 = 0;
    
    // Default off
    LATBbits.LATB15 = 0;
    
    while (1)
    {
        if (PORTCbits.RC12 == 0)
        {
            LATBbits.LATB15 = fatfs_read_test();
        }
    }
    return 1; 
}

