#include <stdio.h>
#include <string.h>
#include <xc.h>

#include "FatFS/ff.h"
#include "chip_fuse_bits.h"
#include "init_hardware.h"
#include "spi_driver.h"

#define FCY 64000000UL
#include <libpic30.h>

typedef uint16_t SAMPLE;

// Test opening a directory and listing all the files within that directory
int fatfs_list_directory_test(void) {
  FATFS fs;
  DIR dir;
  FRESULT res;
  BYTE buff[256];
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
  UINT br;WWWZzz       E

  FRESULT fm = f_mount(&fs, "", 0);
  if (fm == FR_OK) {
    FRESULT fo = f_open(&fsrc, "corporate_song_4min55.raw", FA_READ);
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
  while (OSCCONbits.OSWEN != 0)
    ;

  // wait for PLL to lock
  while (OSCCONbits.LOCK != 1)
    ;
}

#define num_buffers (2)
#define buffer_size (8192)  // 512B blocks * 8 blocks
BYTE buffer_bytes[num_buffers][buffer_size];
BYTE* buffers;
BYTE* currentBuffer;
SAMPLE* currentSample;

static void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
  TMR1 = 0;  // reset timer count
  DAC1DATHbits.DACDATH = *currentSample;
  const ptrdiff_t total_buffer_bytes = (num_buffers * buffer_size);
  const ptrdiff_t next_sample_offset = ((BYTE*)(currentSample + 1)) - buffers;
  currentSample = buffers + next_sample_offset % total_buffer_bytes;
  currentBuffer =
      buffers + buffer_size * (((BYTE*)currentSample - buffers) / buffer_size);
  IFS0bits.T1IF = 0;  // clear Timer1 interrupt flag
}

void set_timer1_interrupt(const uint16_t i) { IEC0bits.T1IE = i; }

BYTE* load_next_buffer(FIL* file) {
  BYTE* next_buffer = currentBuffer == buffers ? buffers + buffer_size : buffers;

  UINT bytes_read;
  if (f_read(file, next_buffer, buffer_size, &bytes_read) != FR_OK) {
    return NULL;
  }
  if (bytes_read < buffer_size) {
    if (bytes_read > 0) {
      memset(next_buffer + bytes_read, 0, buffer_size - bytes_read);
    }
    return NULL;
  }

  return next_buffer;
}

// timer interrupts are off
// mount the sd card for reading
// read first chunk into current buffer -> wait for door
// in the door trigger ISR:
//  -  turn timer interrupt on
//
// in the timer isr:
//  - place current sample on the DAC
//  - increment the current sample
//  - find location in current buffer, decide if its time to load next buffer

// For later
// repeat operations instead of erroring out
// select random song, instead of hard-coded one
// consider ambient noise
// delay between playing songs

int main(void) {
  buffers = (BYTE*)buffer_bytes;
  currentBuffer = buffers;
  currentSample = NULL;
  
  init_primary_pll();
  switch_to_primary_oscillator();
  init_spi_io();
  init_dac();
  init_timer1();
  init_interrupts();

  LATBbits.LATB15 = 0;
  TRISDbits.TRISD15 = PIN_OUTPUT;
  LATDbits.LATD15 = 1; // enable speaker
  

  FATFS sd_card;
  // mount sd card
  if (f_mount(&sd_card, "", 0) != FR_OK) {
    return 1;
  }

  while (1) {
    // open selected file
    FIL file;
    if (f_open(&file, "corporate_song_4min55.raw", FA_READ) != FR_OK) {
      return 1;
    }

    // pre-load first buffer of samples
    currentBuffer = load_next_buffer(&file);
    currentSample = (SAMPLE*)currentBuffer;
    if (currentBuffer == NULL) {
      continue;  // restart process
    }

    // wait for door to trigger
    while (PORTCbits.RC12 == 1)
      ;
    set_timer1_interrupt(1);

    // load buffers as current buffer empties, interrupted by ISRs
    BYTE* loaded_buffer = currentBuffer;
    do {
      if (loaded_buffer == currentBuffer) { // started to play last loaded buffers samples
        BYTE* load_buf = currentBuffer;
        loaded_buffer = load_next_buffer(&file);
        if(load_buf != currentBuffer) {
          LATBbits.LATB15 = 1;
        }
      }
    } while (loaded_buffer != NULL);
    
    set_timer1_interrupt(0);
    f_close(&file);
  }

  spi_close();
  f_unmount("0:");
  return 1;
}
