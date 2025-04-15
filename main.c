#include "stm32f0xx.h"
#include "systick.h"
#include "clock_config.h"
#include "tft.h"

TFT_S tft;

void test_triangles(int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3){
    tft_draw_filled_triangle(&tft, x1,y1,x2,y2,x3,y3);
    tft_draw_triangle(&tft, x1,y1+80,x2,y2+80,x3,y3+80);
}

void main(void){
    clock_init();
    system_tick_init();

    tft_init(&tft, ST7735_DRIVER, 160, 128);
    tft.set_colour(tft.context, 255,255,255);

    while(1){
        test_triangles(50,0,0,50,70,50);
    }
}