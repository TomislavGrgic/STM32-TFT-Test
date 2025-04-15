#include "systick.h"

static uint64_t system_ms = 0;

void SysTick_Handler(void){
    system_ms++;
}

void system_tick_init(void){
    SysTick->CTRL = 0;
    SysTick->LOAD = 6000 - 1;
    SysTick->VAL = 0;
    /*Systick clock is set to zero meaning it will divide SYSCLK by 8.*/
    SysTick->CTRL = (1u<<SysTick_CTRL_TICKINT_Pos) | (1u<<SysTick_CTRL_ENABLE_Pos);
    __enable_irq();
}

uint64_t system_get_ms(void){
    __disable_irq();
    uint64_t ms_out = system_ms;
    __enable_irq();
    return ms_out;  
}

void system_sleep(uint32_t delay){
    uint64_t sleep_end = system_get_ms() + delay;
    while(sleep_end > system_get_ms());
}