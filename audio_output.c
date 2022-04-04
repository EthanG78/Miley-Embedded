#include "audio_output.h"
#include "init_hardware.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <xc.h>

#define NUM_BUFFERS 2
#define BUFFER_BYTES 8192  // factor of 512B SD card blocks

bool playing_audio = false;
const ptrdiff_t total_buffer_bytes = (NUM_BUFFERS * BUFFER_BYTES);
BYTE buffer_bytes[NUM_BUFFERS][BUFFER_BYTES];
BYTE* buffers;
BYTE* current_buffer;
SAMPLE* current_sample;

void init_audio_output() {
  init_interrupts();
  init_timer1();
  init_dac();
  init_spi_io();
  init_digital_outputs();

  buffers = (BYTE*)buffer_bytes;
  current_buffer = buffers;
  current_sample = NULL;
}

static BYTE* load_next_buffer(FIL* const file) {
  BYTE* next_buffer =
      buffers + (current_buffer + BUFFER_BYTES - buffers) % total_buffer_bytes;

  UINT bytes_read;
  if (f_read(file, next_buffer, BUFFER_BYTES, &bytes_read) != FR_OK) {
    return NULL;
  }
  if (bytes_read < BUFFER_BYTES) {
    if (bytes_read > 0) {
      memset(next_buffer + bytes_read, 0, BUFFER_BYTES - bytes_read);
    }
    return NULL;
  }

  return next_buffer;
}

int preload_audio_buffer(FIL* const file) {
  current_buffer = load_next_buffer(file);
  current_sample = (SAMPLE*)current_buffer;
  if (current_buffer == NULL) {
    return 1;
  }
  return 0;
}

void play_audio_file(FIL* const file) {
  playing_audio = true;

  // load buffers as current buffer empties, interrupted by ISRs
  const BYTE* loaded_buffer = current_buffer;
  do {
    // started to play last loaded buffers samples
    if (loaded_buffer == current_buffer) {
      loaded_buffer = load_next_buffer(file);
    }
  } while (loaded_buffer != NULL);

  playing_audio = false;
}

static void next_audio_sample() {
  const ptrdiff_t next_sample_offset = ((BYTE*)(current_sample + 1)) - buffers;
  current_sample = (SAMPLE*)(buffers + next_sample_offset % total_buffer_bytes);

  const ptrdiff_t current_sample_buffer_idx =
      ((BYTE*)current_sample - buffers) / BUFFER_BYTES;
  current_buffer = buffers + BUFFER_BYTES * current_sample_buffer_idx;
}

static void set_amplifier_gain(const uint16_t noise) {
  uint16_t level_4 = 0;
  uint16_t level_3 = 0;
  uint16_t level_2 = 0;
  uint16_t level_1 = 0;

  if(noise >= 3072){
    level_4 = 1;		//activate gain stage: 1 V/V in loud environment
  } else if(noise >= 2048){
    level_3 = 1;		//activate gain stage: 0.82 V/V
  } else if(noise >= 1024){
    level_2 = 1;		//activate gain stage: 0.199 V/V
  } else if(noise >= 0) {
    level_1 = 1;		//activate gain stage: 0.03 V/V in quiet environment
  }
  
  LATDbits.LATD3 = level_4;
  LATDbits.LATD2 = level_3;
  LATBbits.LATB11 = level_2;
  LATCbits.LATC14 = level_1;
}

static SAMPLE sin_wave() {
  static uint16_t sample_idx = 0;
  const static int period = 30;
  const float radians = 2 * 3.1415926535 * ((float)(sample_idx) / period);
  ++sample_idx;
  return 1840 * sin(radians) + 2047;
}


static void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
  if (playing_audio) {
    //DAC1DATHbits.DACDATH = sin_wave();
    DAC1DATHbits.DACDATH = *current_sample;
    next_audio_sample();
  } else {
    ADCON3Lbits.SWCTRG = 1;  // trigger ADC conversion
    const uint16_t noise = mic_avg_amplitude();
    set_amplifier_gain(noise);
  }
  IFS0bits.T1IF = 0;  // clear Timer1 Interrupt Flag
}
