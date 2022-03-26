#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>

void spi_close(void);
void spi_open_initializer(void);
void spi_open_reading(void);
uint8_t spi_exchangeByte(uint8_t);

void spi_cs_low(void);
void spi_cs_high(void);

void spi_do_high();
void spi_do_low();

#endif