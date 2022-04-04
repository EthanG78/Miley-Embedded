#include "door_sensor.h"
#include "init_hardware.h"

#include <xc.h>

void init_door_sensor() {
  init_interrupts();
  init_digital_input();
}

void wait_for_door_trigger() {
  // wait for door to close
  while (PORTCbits.RC3 == 0) {
    continue;
  }
  
  // wait for door to open
  while(PORTCbits.RC3 == 1) {
    continue;
  }
}