#ifndef __SPI_1_H__
#define __SPI_1_H__

#include "stm32f0xx.h"
#include <stddef.h>


#define SPI1_CS_HIGH    1
#define SPI1_CS_LOW     0


void spi1_cs_control(uint8_t level);
int8_t spi1_write(uint8_t data);
int8_t spi1_write_buffer(uint8_t *data, size_t data_size);
int16_t spi1_read(void);
void spi1_init(void);


#endif /*__SPI_1_H__*/