#include "stm32f0xx.h"
#include "systick.h"
#include "clock_config.h"
#include "tft.h"

#include <math.h>

TFT_S tft;

void calculate_rotation(int x_origin, int y_origin, int x_point, int y_point, float radians, int *x, int *y){
    int x_shifted = x_point-x_origin;
    int y_shifted = y_point-y_origin;
    *x  =  (float)x_shifted*cos(radians) - (float)y_point*sin(radians);
    *y  =  (float)x_shifted*sin(radians) + (float)y_point*cos(radians);

    *x += x_origin;
    *y += y_origin;
}

void main(void){
    clock_init();
    system_tick_init();

    tft_init(&tft, ST7735_DRIVER, 160, 128);
    tft.set_colour(tft.context, 255,255,255);

    int i = 0;
    float one_second_radians = 0.1236;
    const int x_origin = 63, y_origin = 80;
    const int x_init = 20, y_init = 20;
    int x = 0, y = 0;
    int xp = 0, yp = 0;
    while(1){
        xp = x; yp = y;
        calculate_rotation(x_origin, y_origin, x_init, y_init, i*one_second_radians, &x, &y);

        tft_draw_circle(&tft, x_origin,y_origin,60);
        tft_draw_circle(&tft, x_origin,y_origin,2);
        tft.set_colour(tft.context, 0,0,0);
        tft_draw_line(&tft,x_origin, y_origin, xp,yp);
        tft.set_colour(tft.context, 255,255,255);
        //seconds
        tft_draw_line(&tft,x_origin, y_origin, x,y);
        //Minutes
        tft_draw_line(&tft,x_origin, y_origin, 20,y_origin+10);
        tft_draw_line(&tft,x_origin, y_origin+1, 20,y_origin+11);
        //hours
        tft_draw_line(&tft,x_origin, y_origin, x_origin+14,55);
        tft_draw_line(&tft,x_origin+1, y_origin, x_origin+15,55);

        system_sleep(1000);
        i++;
    }
}