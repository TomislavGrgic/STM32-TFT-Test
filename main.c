#include "stm32f0xx.h"
#include "systick.h"
#include "clock_config.h"

#include "tft.h"
#include "graphics.h"

TFT_S tft;


void main(void){
    clock_init();
    system_tick_init();

    tft_init(&tft, ST7735_DRIVER, 160, 128);
    tft.set_foreground(tft.context, 255,255,255);
    tft.set_background(tft.context, 0,0,0);

    while(1){
        draw_filled_triangle(&tft, 0,0,50,50,20,80);
        system_sleep(1000);
    }
}