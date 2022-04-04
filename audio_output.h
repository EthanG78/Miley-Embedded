#ifndef AUDIO_OUTPUT_H
#define AUDIO_OUTPUT_H

#include "FatFS/ff.h"
#include <stdint.h>

typedef uint16_t SAMPLE;

void init_audio_output();
int preload_audio_buffer(FIL* const);
void play_audio_file(FIL* const);

#endif
