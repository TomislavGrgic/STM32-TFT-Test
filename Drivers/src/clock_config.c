#include "clock_config.h"


void clock_init(void){
    RCC->CR2 &= ~RCC_CR2_HSI48ON;
    RCC->CR2 |= RCC_CR2_HSI48ON;

    while ((RCC->CR2 & RCC_CR2_HSI48RDY) == 0);

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE_DIV1;
    
    FLASH->ACR |= FLASH_ACR_LATENCY;

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI48;

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI48);
}
