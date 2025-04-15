#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f0xx.h"
#include <stdint.h>

void system_tick_init(void);
uint64_t system_get_ms(void);
void system_sleep(uint32_t delay);

#endif /*__SYSTICK_H__*/