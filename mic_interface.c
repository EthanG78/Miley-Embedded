#include "mic_interface.h"

#include <xc.h>

#include "init_hardware.h"

#define MIC_SAMPLE_BIAS 2048
#define BUFFER_SIZE 128

uint16_t samples_in_buffer = 0;
uint16_t average_buffer[BUFFER_SIZE];
uint32_t amplitude_sum = 0;
uint32_t avg_amplitude = 0;

void init_mic_interface() {
  init_interrupts();
  init_timer1();
  init_adc();
}

uint16_t mic_avg_amplitude() { return avg_amplitude; }

static void handle_mic_sample(const uint16_t sample) {
  const uint16_t amplitude = sample > MIC_SAMPLE_BIAS
                                 ? sample - MIC_SAMPLE_BIAS
                                 : MIC_SAMPLE_BIAS - sample;
  amplitude_sum += amplitude;

  if (samples_in_buffer == BUFFER_SIZE) {
    amplitude_sum -= average_buffer[0];
    
    // shift old samples by one &
    int sample_idx;
    for (sample_idx = 1; sample_idx < BUFFER_SIZE; ++sample_idx) {
      average_buffer[sample_idx - 1] = average_buffer[sample_idx];
    }
  } else {
    ++samples_in_buffer;
  }

  // add new sample to buffer
  average_buffer[samples_in_buffer - 1] = sample;

  // update average
  avg_amplitude = amplitude_sum / samples_in_buffer;
}

// ADC Conversion complete ISR
static void _ISR _ADCAN0Interrupt(void) {
  handle_mic_sample(ADCBUF0);
  _ADCAN0IF = 0;  // clear interrupt flag
}
