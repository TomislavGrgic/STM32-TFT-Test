#include "st7735.h"
//#include <stdlib.h>

#define ST7735_LED_PIN     PA10    /** Backlight pin */
#define ST7735_DC_PIN      PA9     /** Data/Command pin */
#define ST7735_RST_PIN     PA8     /** Reset Pin */

/* Datasheet says 120 ms (page 91), but just to be safe :) */
#define ST7735_POWER_ON_DELAY_MS   130 /**Power settling delay */


static void st7735_write_12bit_pixel(ST7735_Colour_S *clr);
static void st7735_write_16bit_pixel(ST7735_Colour_S *clr);
static void st7735_write_18bit_pixel(ST7735_Colour_S *clr);
static void st7735_set_write_mode(uint8_t mode);
static void st7735_write(uint8_t reg);
static void st7735_start_com(void);
static void st7735_stop_com(void);


const uint8_t st7735_setup_sequence[] = {
    ST7735_WM_COMMAND, NORON, 
    ST7735_WM_COMMAND, GAMSET, ST7735_WM_DATA, 0x02,
    ST7735_WM_COMMAND, COLMOD, ST7735_WM_DATA, COLMOD_18BIT,
    ST7735_WM_COMMAND, IDMOFF,
    ST7735_WM_COMMAND, MADCTL, ST7735_WM_DATA, 0b0000000
};


static void st7735_write_12bit_pixel(ST7735_Colour_S *clr){
    st7735_write( (clr->green&0x0F) | (clr->red<<4) );
    st7735_write( (clr->red&0x0F) | (clr->blue<<4) );
    st7735_write(  (clr->blue&0x0F) | (clr->green<<4) );
}


static void st7735_write_16bit_pixel(ST7735_Colour_S *clr){
    st7735_write( (clr->red<<3) | (clr->green>>3) );
    st7735_write( (clr->blue&0x1f) | (clr->green<<5));
}


static void st7735_write_18bit_pixel(ST7735_Colour_S *clr){
    st7735_write( (clr->red) << 2);
    st7735_write( (clr->green) << 2);
    st7735_write( (clr->blue) << 2);
}


static void st7735_init_ll_drivers(void){
    gpio_mode(ST7735_LED_PIN, GPIO_OUTPUT);
    gpio_mode(ST7735_DC_PIN, GPIO_OUTPUT);
    gpio_mode(ST7735_RST_PIN, GPIO_OUTPUT);

    spi1_init();

    gpio_write(ST7735_RST_PIN, GPIO_HIGH);
    system_sleep(ST7735_POWER_ON_DELAY_MS); /*A delay is needed after coming out of reset*/
}


static void st7735_set_write_mode(uint8_t mode){
    gpio_write(ST7735_DC_PIN, mode);
}


static void st7735_write(uint8_t reg){
    spi1_write(reg);
}


static void st7735_start_com(void){
    spi1_cs_control(SPI1_CS_LOW);
}


static void st7735_stop_com(void){
    spi1_cs_control(SPI1_CS_LOW);
}

ST7735_Context_S temp_ctx = {0};

void* st7735_init(int16_t x, int16_t y){
    ST7735_Context_S *ctx = &temp_ctx; //Add malloc

    if(ctx == NULL){
        return NULL;
    }

    st7735_init_ll_drivers();
    st7735_set_background_colour(ctx, 0x00, 0x00, 0x00);

    st7735_start_com();
    st7735_set_write_mode(ST7735_WM_COMMAND);

    st7735_write(SLPOUT);
    system_sleep(ST7735_POWER_ON_DELAY_MS); 

    for(uint16_t i = 0; i <  sizeof(st7735_setup_sequence); i+=2){
        st7735_set_write_mode(st7735_setup_sequence[i]);
        st7735_write(st7735_setup_sequence[i+1]);
    }

    st7735_stop_com();
    ctx->height = x;
    ctx->width = y;
    ctx->pixel_count = ctx->height * ctx->width;
    ctx->pixel_write = st7735_write_18bit_pixel;

    st7735_fill_screen(ctx);
    st7735_display_state(ctx, ST7735_DISPLAY_ON);
    gpio_write(ST7735_LED_PIN, GPIO_HIGH);

    return (void*)ctx;
}


void st7735_deinit(void* ctx){
    if(ctx == NULL){
        return;
    }
    
    //free(ctx);
    ctx = NULL;
}


void st7735_set_colour_mode(void *ctx_in, uint8_t mode){
    ST7735_Context_S *ctx = (ST7735_Context_S *)ctx_in;

    st7735_start_com();
    st7735_set_write_mode(ST7735_WM_COMMAND);
    st7735_write(COLMOD);
    st7735_set_write_mode(ST7735_WM_DATA);
    st7735_write(mode);
    st7735_stop_com();
    ctx->pixel_count = ctx->height*ctx->width;

    switch (mode) {
        case COLMOD_12BIT:
            ctx->pixel_write = st7735_write_12bit_pixel;
            ctx->pixel_count = ctx->pixel_count>>1; 
            break;
        
        case COLMOD_16BIT:
            ctx->pixel_write = st7735_write_16bit_pixel; 
            break;

        case COLMOD_18BIT:
        default:
            ctx->pixel_write = st7735_write_18bit_pixel; 
            break;
    }
}


void st7735_display_state(void *ctx_in, uint8_t state){
    ST7735_Context_S *ctx = (ST7735_Context_S *)ctx_in;

    st7735_start_com();
    st7735_set_write_mode(ST7735_WM_COMMAND);
    st7735_write( DISPOFF + (state&1) );
    st7735_stop_com();
    system_sleep(ST7735_POWER_ON_DELAY_MS); 
}


void st7735_draw_pixel(void *ctx_in) {
    ST7735_Context_S *ctx = (ST7735_Context_S *)ctx_in;

    st7735_start_com();
    st7735_set_write_mode(ST7735_WM_COMMAND);
    st7735_write(RAMWR);
    st7735_set_write_mode(ST7735_WM_DATA);
    ctx->pixel_write(&ctx->foreground);
    st7735_stop_com();
}



void st7735_fill_screen(void *ctx_in){
    ST7735_Context_S *ctx = (ST7735_Context_S *)ctx_in;

    st7735_start_com();

    st7735_set_write_mode(ST7735_WM_COMMAND);
    st7735_write(RAMWR);
    st7735_set_write_mode(ST7735_WM_DATA);

    for(int i = 0; i < ctx->pixel_count; i++){
        ctx->pixel_write(&ctx->background);
    }

    st7735_stop_com();
}


void st7735_set_frame(void *ctx_in, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2){
    ST7735_Context_S *ctx = (ST7735_Context_S *)ctx_in;

    st7735_start_com();

    st7735_set_write_mode(ST7735_WM_COMMAND);
    st7735_write(CASET);
    st7735_set_write_mode(ST7735_WM_DATA);
    st7735_write((x1>>8)&0xFF);
    st7735_write(x1&0xFF);
    st7735_write((x2>>8)&0xFF);
    st7735_write(x2&0xFF);

    st7735_set_write_mode(ST7735_WM_COMMAND);
    st7735_write(RASET);
    st7735_set_write_mode(ST7735_WM_DATA);
    st7735_write((y1>>8)&0xFF);
    st7735_write(y1&0xFF);
    st7735_write((y2>>8)&0xFF);
    st7735_write(y2&0xFF);

    st7735_stop_com();

    ctx->pixel_count = (x2-x1+1)*(y2-y1+1);
}


void st7735_set_rotation(void *ctx_in, uint8_t rotation){
    ST7735_Context_S *ctx = (ST7735_Context_S *)ctx_in;

    uint8_t rows = 0;
    uint8_t cols = 0;
    uint8_t rot_config = 0;
}


void st7735_set_background_colour(void *ctx_in, uint8_t red, uint8_t green, uint8_t blue) {
    ST7735_Context_S *ctx = (ST7735_Context_S *)ctx_in;
    ctx->background.red = red;
    ctx->background.green = green;
    ctx->background.blue = blue;
}

void st7735_set_foreground_colour(void *ctx_in, uint8_t red, uint8_t green, uint8_t blue) {
    ST7735_Context_S *ctx = (ST7735_Context_S *)ctx_in;
    ctx->foreground.red = red;
    ctx->foreground.green = green;
    ctx->foreground.blue = blue;
}