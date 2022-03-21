/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef SPI_DRIVER_H
#define	SPI_DRIVER_H

#include <xc.h> 

void spi_close(void);
void spi_open_initializer(void);
void spi_open_reading(void);
uint8_t spi_exchangeByte(uint8_t b);

void spi_cs_low(void);
void spi_cs_high(void);

void spi_do_high();
void spi_do_low();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* SPI_DRIVER_H */

