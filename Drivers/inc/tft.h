#ifndef __TFT_H__
#define __TFT_H__

#include <stdint.h>

typedef enum{
    ST7735_DRIVER,
} Drivers_E;


typedef struct{
    void* (*init)(int16_t, int16_t);
    void (*deinit)(void *);
    void (*fill_screen)(void *, uint8_t, uint8_t, uint8_t);
    void (*set_cursor)(void *, uint16_t, uint16_t);
    void (*set_colour)(void *, uint8_t, uint8_t, uint8_t);
    void (*set_frame)(void *, uint16_t, uint16_t, uint16_t, uint16_t);
    void (*draw_pixel)(void *, uint16_t, uint16_t);
    void *context;
} TFT_S;

void tft_init(TFT_S *tft, Drivers_E driver, uint16_t w, uint16_t h);
void tft_deinit(TFT_S *tft);

void tft_draw_line(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void tft_draw_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w);
void tft_draw_filled_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w);
void tft_draw_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void tft_draw_filled_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);

#endif /*__TFT_H__*/