#include "FatFS/ff.h"
#include "chip_fuse_bits.h"
#include "init_hardware.h"
#include "audio_output.h"
#include "door_sensor.h"

#define FCY 64000000UL
#include <libpic30.h>
#include <xc.h>
#include <stdlib.h>

#define AUDIO_TIMEOUT_MS 1

// returns a random file name from the provided directory
// the directory object will be refreshed if the end of the directory is reached
TCHAR* random_file_name(const char* const directory_name, DIR* directory,
                      FILINFO* read_file) {
  int remaining_reads;
  for (remaining_reads = rand() % 8; remaining_reads >= 0;) {
    // read a directory item
    if (f_readdir(directory, read_file) != FR_OK) {
      return NULL;
    }

    // end of directory, refresh directory object
    const TCHAR first_letter = read_file->fname[0];
    if (first_letter == '\0') {
      f_closedir(directory);
      if(f_opendir(directory, directory_name) != FR_OK) {
        return NULL;
      }
      continue;
    }
    
    // directory, hidden file or special directory -> ignore
    if (read_file->fattrib & AM_DIR || first_letter == '.') {
      continue;
    }


    --remaining_reads;
  }

  return read_file->fname;
}


int main(void) {
  init_primary_pll();
  init_audio_output();
  init_mic_interface();
  init_door_sensor();

  TRISDbits.TRISD15 = PIN_OUTPUT;
  LATDbits.LATD15 = 1;  // enable speaker

  FATFS sd_card;
  // mount sd card
  if (f_mount(&sd_card, "", 0) != FR_OK) {
    return 1;
  }
  
  DIR directory;
  if(f_opendir(&directory, "/") != FR_OK) {
    goto cleanup;
  }

  while (1) {
    // read SD card directory for random file name
    FILINFO file_info;
    char* file_name = random_file_name("/", &directory, &file_info);
    if (file_name == NULL) {
      goto cleanup;
    }
    
    FIL file;
    FRESULT r = f_open(&file, file_name, FA_READ);
    if (r != FR_OK) {
      return 1;
    }
    
    if (preload_audio_buffer(&file) != 0) {
      continue;
    }
    
    __delay_ms(AUDIO_TIMEOUT_MS)
    wait_for_door_trigger();
    play_audio_file(&file);
    f_close(&file);
  }

cleanup:
  f_closedir(&directory);
cleanup_mount:
  f_unmount("0:");
  return 1;
}
