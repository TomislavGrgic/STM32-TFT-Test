#include "tft.h"

#define ST7735

#ifdef ST7735
#include "st7735.h"

static void* st7735_adapter_init(int16_t x, int16_t y){
    return st7735_init(x,y);
}

static void st7735_adapter_deinit(void *ctx){
    st7735_deinit(ctx);
}

static void st7735_adapter_fill_screen(void *ctx){
    st7735_fill_screen(ctx);
}

static void st7735_adapter_set_foreground(void *ctx, uint8_t red, uint8_t green, uint8_t blue){
    st7735_set_foreground_colour(ctx, red, green, blue);
}

static void st7735_adapter_set_background(void *ctx, uint8_t red, uint8_t green, uint8_t blue){
    st7735_set_background_colour(ctx, red, green, blue);
}

static void st7735_adapter_set_cursor(void *ctx, uint16_t x, uint16_t y){
    st7735_set_frame(ctx,x,x,y,y);
}

static void st7735_adapter_set_frame(void *ctx, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2){
    st7735_set_frame(ctx,x1,x2,y1,y2);
}

static void st7735_adapter_draw_pixel(void *ctx, uint16_t x, uint16_t y){
    st7735_set_frame(ctx,x,x,y,y);
    st7735_draw_pixel(ctx);
}

static void st7735_adapter_bitstream(void *ctx_in, const uint8_t *data, uint16_t size){
    st7735_write_bit_stream(ctx_in, data, size);
}

#endif

void tft_init(TFT_S *tft, Drivers_E driver, uint16_t w, uint16_t h) {

    switch((uint8_t)driver){
#ifdef ST7735 
        case (uint8_t)ST7735_DRIVER:
            tft->init = st7735_adapter_init;
            tft->deinit = st7735_adapter_deinit;
            tft->fill_screen = st7735_adapter_fill_screen;
            tft->set_foreground = st7735_adapter_set_foreground;
            tft->set_background = st7735_adapter_set_background;
            tft->set_cursor = st7735_adapter_set_cursor;
            tft->set_frame = st7735_adapter_set_frame;
            tft->draw_pixel = st7735_adapter_draw_pixel;
            tft->bitstream = st7735_adapter_bitstream;
            tft->context = tft->init(w,h);
            break;
#endif
    }

    /*If the address is not null there is nothing to be done, exit the function*/
    if(tft->context != NULL){
        return;
    }
    
    /*If code gets here that means context was not allocated. 
    Before reseting the tft struct check if there is a deinit callback*/
    if(tft->deinit != NULL){
        tft->deinit(tft->context);
    }
}

void tft_deinit(TFT_S *tft){
    tft->deinit(tft->context);
    tft->init = NULL;
    tft->deinit = NULL;
    tft->fill_screen = NULL;
    tft->set_cursor = NULL;
    tft->set_frame = NULL;
    tft->draw_pixel = NULL;
}

