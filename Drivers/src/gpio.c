#include "gpio.h"

#define GPIO_MODER_MASK         0x03
#define GPIO_PORTS_Pos          17
#define GPIO_IO_MASK            0x01
#define GPIO_CONFIG_MASK        0x30
#define GPIO_PUPDR_MASK         0x03
#define GPIO_OTYPER_MASK        0x01


static GPIO_TypeDef* gpio_get_port(uint32_t pin){
    switch(pin&PORT_MASK){
        case PORTA:
            return GPIOA;
        case PORTB:
            return GPIOB;
        case PORTC:
            return GPIOC;
        case PORTD:
            return GPIOD;
        case PORTE:
            return GPIOE;
        case PORTF:
            return GPIOF;
        default:
            return NULL;
    }
}


void gpio_init(void){
    //TODO add stuff to initialize GPIO pins
}


void gpio_mode(uint32_t pin, uint32_t mode){
    GPIO_TypeDef *gpio_port = gpio_get_port(pin);
    
    if(gpio_port == NULL){
        return;
    }
    
    RCC->AHBENR |= 1u<<(GPIO_PORTS_Pos + ((pin&PORT_MASK) >> 4));
    
    const uint8_t io_type = ( (mode&GPIO_IO_MASK) );
    const uint8_t config = (mode&GPIO_CONFIG_MASK) >> 4;

    if(io_type == GPIO_INPUT){
        gpio_port->PUPDR &= ~( GPIO_PUPDR_MASK << ( (pin&PIN_MASK)*2 ) );
        gpio_port->PUPDR |= config << ( (pin&PIN_MASK)*2 );
    } else if(io_type == GPIO_OUTPUT) {
        gpio_port->OTYPER &= ~( GPIO_OTYPER_MASK << (pin&PIN_MASK) );
        gpio_port->OTYPER |= config << (pin&PIN_MASK);
    }

    gpio_port->MODER &= ~( GPIO_MODER_MASK << ( (pin&PIN_MASK)*2) );
    gpio_port->MODER |= io_type << ( (pin&PIN_MASK)*2 );
}


void gpio_write(uint32_t pin, uint32_t value){
    GPIO_TypeDef *gpio_port = gpio_get_port(pin);

    if(gpio_port == NULL){
        return;
    }

    if(value == GPIO_LOW){
        gpio_port->ODR &= ~(1u<<(pin&PIN_MASK));
    } else {
        gpio_port->ODR |= 1u<<(pin&PIN_MASK);
    }     
}


int8_t gpio_read(uint32_t pin){
    GPIO_TypeDef *gpio_port = gpio_get_port(pin);

    if(gpio_port == NULL){
        return -1;
    }

    const uint32_t gpio_state = (gpio_port->IDR & (1u<<(pin&PIN_MASK)));
    return (int8_t) (gpio_state != 0);     
}