#include "chip_fuse_bits.h"
#include "init_hardware.h"
#include <stdio.h>
#include <xc.h>
#include <string.h>

#include "FatFS/ff.h"
#include "spi_driver.h"

#define FCY 64000000UL
#include <libpic30.h>

static int _hasRun = 0;

// Test opening a directory and listing all the files within that directory
int fatfs_list_directory_test(void) {
  FATFS fs;
  DIR dir;
  FRESULT res;
  char buff[256];
  static FILINFO fno;

  res = f_mount(&fs, "", 0);
  if (res == FR_OK) {
    strcpy(buff, "/");
    res = f_opendir(&dir, buff);
    if (res == FR_OK) {
      for (;;) {
        // Read a directory item
        res = f_readdir(&dir, &fno);
        // Break on error or end of dir
        if (res != FR_OK || fno.fname[0] == 0) break;
        if (fno.fattrib & AM_DIR) {
          // This is a directory
        } else {
          // This is a file
          // Get file name
          int x = 9;  // stall
        }
      }
      f_closedir(&dir);
      return 1;
    }
  }
  return 0;
}

// Test opening and reading a file
int fatfs_read_test(void) {
  FATFS fs;
  FIL fsrc;
  BYTE buffer[4096 * 2];
  UINT br;

  FRESULT fm = f_mount(&fs, "", 0);
  if (fm == FR_OK) {
    FRESULT fo = f_open(&fsrc, "StarWars3.raw", FA_READ);
    if (fo == FR_OK) {
      FRESULT fr;
      for (;;) {
        fr = f_read(&fsrc, buffer, sizeof buffer, &br);
        if (br == 0) break; /* error or eof */
      }
      spi_close();
      f_close(&fsrc);
      f_unmount("0:");
      return 1;
    }
  }

  spi_close();
  f_close(&fsrc);
  f_unmount("0:");

  return 0;
}

void switch_to_primary_oscillator() {
  // initiate clock switch to primary oscillator with PLL (NOSC=0b011)
  __builtin_write_OSCCONH(0x03);
  __builtin_write_OSCCONL(OSCCON | 0x01);
  // wait for Clock switch to occur
  while (OSCCONbits.OSWEN!= 0);

  // wait for PLL to lock
  while (OSCCONbits.LOCK != 1);
}

int main(void) {
  init_primary_pll();
  switch_to_primary_oscillator();
  init_spi_io();
  LATBbits.LATB15 = 1;
  while (1)
  {
    if (PORTCbits.RC12 == 0 && !_hasRun)
    {
      LATBbits.LATB15 = 0;
      _hasRun++;
      LATBbits.LATB15 = fatfs_read_test();
      //LATBbits.LATB15 = fatfs_list_directory_test();


      // Turn the light back off if we passed
    }
  }
  return 1; 
}