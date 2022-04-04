/*
 * File:   spi_driver.c
 * Author: ethang
 *
 * Created on March 21, 2022, 12:06 PM
 */
#include "spi_driver.h"

#include <xc.h>

// Closes SPI operations
void spi_close(void) { SPI1CON1Lbits.SPIEN = 0; }

// Opens SPI with a clock frequency of
// 125 KHz for initialization process with
// SD card
void spi_open_initializer(void) {
  // Disable SPI interrupts
  IEC0bits.SPI1RXIE = 0;
  IEC0bits.SPI1TXIE = 0;

  // Stop and reset SPI module
  SPI1CON1Lbits.SPIEN = 0;

  // Clear the buffer
  SPI1BUFLbits.SPI1BUFL = 0;
  SPI1BUFHbits.SPI1BUFH = 0;

  // Disable enhanced buffer mode
  SPI1CON1Lbits.ENHBUF = 0;

  // SPI calculates clock frequency using
  // the following formula
  // Baud Rate = Fp / (2 * (SPIxBRG  + 1))
  // For a Baud Rate of 125 000, SPIxBRG = 15
  SPI1BRGLbits.BRG = 0x03;

  SPI1STATLbits.SPIROV = 0;

  // Disable audio protocol
  SPI1CON1Hbits.AUDEN = 0;

  // 8 bit data
  SPI1CON2Lbits.WLENGTH = 0b00111;

  // 8- bit communication
  SPI1CON1Lbits.MODE32 = 0;
  SPI1CON1Lbits.MODE16 = 0;

  // Transmit happens on transition from
  // active clock state to Idle clock state
  SPI1CON1Lbits.CKE = 1;

  // Host Mode enable
  SPI1CON1Lbits.MSTEN = 1;

  // Open SPI
  SPI1CON1Lbits.SPIEN = 1;
}

// Opens SPI with a higher clock frequency
// for reading operations to SD card
void spi_open_reading(void) {
  // Disable SPI interrupts
  IEC0bits.SPI1RXIE = 0;
  IEC0bits.SPI1TXIE = 0;

  // Stop and reset SPI module
  SPI1CON1Lbits.SPIEN = 0;

  // Clear the buffer
  SPI1BUFLbits.SPI1BUFL = 0;
  SPI1BUFHbits.SPI1BUFH = 0;

  // Disable enhanced buffer mode
  SPI1CON1Lbits.ENHBUF = 0;

  // SPI calculates clock frequency using
  // the following formula
  // Baud Rate = Fp / (2 * (SPIxBRG  + 1))
  // For a Baud Rate of 2 000 000, SPIxBRG = 0
  // TODO: We may need to lower the baud rate
  // for reliable communication
  SPI1BRGLbits.BRG = 0x02;
  //SPI1BRGLbits.BRG = 0;

  SPI1STATLbits.SPIROV = 0;

  // Disable audio protocol
  SPI1CON1Hbits.AUDEN = 0;

  // 8 bit data
  SPI1CON2Lbits.WLENGTH = 0b00111;

  // 8- bit communication
  SPI1CON1Lbits.MODE32 = 0;
  SPI1CON1Lbits.MODE16 = 0;

  // Transmit happens on transition from
  // active clock state to Idle clock state
  SPI1CON1Lbits.CKE = 1;

  // Host Mode enable
  SPI1CON1Lbits.MSTEN = 1;

  // Open SPI
  SPI1CON1Lbits.SPIEN = 1;
}

// Full duplex exchanges byte between
// controller and peripheral
uint8_t spi_exchangeByte(const uint8_t b) {
  // We might only need to write to 8 LSB
  // since we are exchanging bytes
  SPI1BUFLbits.DATA = b;

  while (!SPI1STATLbits.SPIRBF) {
    continue;
  }

  // TODO: This might only be returning the 8 MSB
  // which wouldn't work....
  // Might need to shift data by 8 bits
  return SPI1BUFLbits.DATA;
}

// Sets the SPI chip select low
void spi_cs_low() { LATBbits.LATB15 = 0; }

// Sets the SPI chip select high
void spi_cs_high() { LATBbits.LATB15 = 1; }

// Sets the data out line on
// the controller high
void spi_do_high() { LATCbits.LATC12 = 1; }

// Sets the data out line on the controller low
void spi_do_low() { LATCbits.LATC12 = 0; }