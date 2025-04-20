#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "tft.h"

void draw_fast_line(TFT_S *tft, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
void draw_line(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void draw_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w);
void draw_filled_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w);
void draw_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void draw_filled_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void draw_circle(TFT_S *tft, int16_t x, int16_t y, int16_t r);
void draw_filled_circle(TFT_S *tft, int16_t x, int16_t y, int16_t r);
void draw_bitmap(TFT_S *tft, const uint8_t *data, int16_t x, int16_t y, int16_t w, int16_t h);
void draw_text(TFT_S *tft, const char* text, int16_t x, int16_t y);

#endif /*__GRAPHICS_H__*/