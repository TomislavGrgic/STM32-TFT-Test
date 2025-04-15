#include "spi1.h"
#include "systick.h"

static void spi1_cs_init(void){
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    GPIOB->MODER &= ~(GPIO_MODER_MODER0);
    GPIOB->MODER |= (0x01 << GPIO_MODER_MODER0_Pos);

    uint32_t lock_bits = GPIO_LCKR_LCK0;
    GPIOB->LCKR = GPIO_LCKR_LCKK | lock_bits;
    GPIOB->LCKR = lock_bits;
    GPIOB->LCKR = GPIO_LCKR_LCKK | lock_bits;
    GPIOB->LCKR;
}


void spi1_cs_control(uint8_t level){
    if(level == SPI1_CS_LOW){
        GPIOB->ODR &= ~(GPIO_ODR_0);
    } else {
        GPIOB->ODR |= GPIO_ODR_0;
    }
}


int8_t spi1_write(uint8_t data){
    uint64_t timeout_start = system_get_ms();

    while ( (SPI1->SR & SPI_SR_BSY) || !(SPI1->SR & SPI_SR_TXE) ){
        if(system_get_ms() - timeout_start > 500){
            return -1;
        }
    }

    *((volatile uint8_t *)&SPI1->DR) = (uint8_t)data;

    timeout_start = system_get_ms() + 500;
    while( (SPI1->SR & SPI_SR_TXE) == 0){
        if(system_get_ms() - timeout_start  > 500){
            return -1;
        }
    }

    return 0;
}


int8_t spi1_write_buffer(uint8_t *data, size_t data_size){
    if(data == NULL){
        return -1;
    }

    for(size_t i = 0; i < data_size; i++){
        if( spi1_write(data[i]) == -1 ){
            return -1;
        }
    }
    
    return 0;
}


int16_t spi1_read(void){
    uint64_t timeout_start = system_get_ms();

    while ( !(SPI1->SR & SPI_SR_RXNE) ){
        if(system_get_ms() - timeout_start > 500){
            return -1;
        }
    }

    return (int16_t) *((volatile uint8_t *)&SPI1->DR);
}


void spi1_init(void){
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7); 
    GPIOA->MODER |= (0b10 << GPIO_MODER_MODER5_Pos) | (0b10 << GPIO_MODER_MODER6_Pos) | (0b10 << GPIO_MODER_MODER7_Pos); 

    /*Clears the Alt function to 0 and since SPI1 Alt function is zero no need to set it after.*/
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5 | GPIO_AFRL_AFRL6 | GPIO_AFRL_AFRL7);

    /*Lock SPI1 pins*/
    uint32_t lock_bits = GPIO_LCKR_LCK5 | GPIO_LCKR_LCK6 | GPIO_LCKR_LCK7;
    GPIOA->LCKR = GPIO_LCKR_LCKK | lock_bits;
    GPIOA->LCKR = lock_bits;
    GPIOA->LCKR = GPIO_LCKR_LCKK | lock_bits;
    GPIOA->LCKR;

    spi1_cs_init();

    SPI1->CR1 &= ~(SPI_CR1_BR);
    //SPI1->CR1 |= 1u << SPI_CR1_BR_Pos;
    SPI1->CR1 |= SPI_CR1_CPHA | SPI_CR1_CPOL | SPI_CR1_MSTR;
    SPI1->CR1 &= ~(SPI_CR1_BIDIMODE | SPI_CR1_RXONLY | SPI_CR1_LSBFIRST | SPI_CR1_CRCEN | SPI_CR1_SSM);

    SPI1->CR2 &= ~(SPI_CR2_DS);
    SPI1->CR2 |=  0b0111u << SPI_CR2_DS_Pos;
    SPI1->CR2 &= ~(SPI_CR2_FRF | SPI_CR2_NSSP);
    SPI1->CR2 |= SPI_CR2_SSOE | SPI_CR2_FRXTH;

    SPI1->CR1 |= SPI_CR1_SPE;
}

