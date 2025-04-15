#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f0xx.h"
#include <stdint.h>
#include <stddef.h>
#include "gpio_pins.h"

#define GPIO_INPUT              0x00
#define GPIO_INPUT_PULL_UP      0x10
#define GPIO_INPUT_PULL_DOWN    0x20

#define GPIO_OUTPUT                 0x01
#define GPIO_OUTPUT_OPEN_DRAIN      0x11

#define GPIO_LOW        0x00
#define GPIO_HIGH       0x01

void gpio_init(void);
void gpio_mode(uint32_t pin, uint32_t mode);
void gpio_write(uint32_t pin, uint32_t value);
int8_t gpio_read(uint32_t pin);

#endif /*__GPIO_H__*/