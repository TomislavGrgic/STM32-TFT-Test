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
        /*draw_filled_triangle(&tft, 50,50,50,80,80,80);
        draw_fast_rectangle(&tft, 30,30,10,10);
        draw_filled_circle(&tft, 80,80,20);
        draw_text(&tft, "TEST123", 10,10);*/
        for(int i = 0; i < 360; i++){
            tft.set_foreground(tft.context, 255,255,255);
            draw_rectangle(&tft, 50,50,50,50,i);
            tft.set_foreground(tft.context, 0,0,0);
            draw_rectangle(&tft, 50,50,50,50,i); 
        }
        system_sleep(1000);
    }
}