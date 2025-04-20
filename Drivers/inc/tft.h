#ifndef __TFT_H__
#define __TFT_H__

#include <stdint.h>

typedef enum{
    ST7735_DRIVER,
} Drivers_E;


typedef struct{
    void* (*init)(int16_t, int16_t);
    void (*deinit)(void *);
    void (*fill_screen)(void *);
    void (*set_cursor)(void *, uint16_t, uint16_t);
    void (*set_foreground)(void *, uint8_t, uint8_t, uint8_t);
    void (*set_background)(void *, uint8_t, uint8_t, uint8_t);
    void (*set_frame)(void *, uint16_t, uint16_t, uint16_t, uint16_t);
    void (*draw_pixel)(void *, uint16_t, uint16_t);
    void (*bitstream)(void *, const uint8_t *, uint16_t);
    void *context;
} TFT_S;

void tft_init(TFT_S *tft, Drivers_E driver, uint16_t w, uint16_t h);
void tft_deinit(TFT_S *tft);

#endif /*__TFT_H__*/