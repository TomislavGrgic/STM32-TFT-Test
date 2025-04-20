#ifndef __ST7735_H__
#define __ST7735_H__

#include "stm32f0xx.h"
#include "spi1.h"
#include "gpio.h"
#include "systick.h"
#include <stddef.h>

#define ST7735_COLS    128
#define ST7735_ROWS    160

 /**
  * @defgroup ST7735_registers 
  * @{
  */
#define NOP         0x00        /**No operation*/
#define SWRESET     0x01        /**Software reset*/
#define RDDID       0x04        /**Read Display ID */
#define RDDST       0x09        /**Read display status */
#define RDDPM       0x0A        /**Read display power */
#define RDDMADCTL   0x0B        /**Read display */
#define RDDCOLMOD   0x0C        /**Read Display Pixel */
#define RDDIM       0x0D        /**Read Display Image */
#define RDDSM       0x0E        /**Read Display Signal */
#define SLPIN       0x10        /**Sleep in & booster off */
#define SLPOUT      0x11        /**Sleep out & booster on*/
#define PTLON       0x12        /**Partial mode on */
#define NORON       0x13        /**Normal operation mode */
#define INFOFF      0x20        /**Display inversion off */
#define INVON       0x21        /**Display inversion on*/
#define GAMSET      0x26        /**Set gamma */
#define DISPOFF     0x28        /**Display off*/
#define DISPON      0x29        /**Display on */
#define CASET       0x2A        /**Column address set */
#define RASET       0x2B        /**Row address set */
#define RAMWR       0x2C        /**Memory write */
#define RAMRD       0x2E        /**Memory read */
#define PTLAR       0x30        /**Partial start/end address set */
#define TEOFF       0x34        /**Tearing effect line off */
#define TEON        0x35        /**Tearing effect mode set and on */
#define MADCTL      0x36        /**Memory data access control */
#define IDMOFF      0x38        /**Idle mode off */
#define IDMON       0x39        /**Idle mode on */
#define COLMOD      0x3A        /**Colour mode */
#define RDID1       0xDA        /**Read ID1 */
#define RDID2       0xDB        /**Read ID2 */
#define RDID3       0xDC        /**Read ID3 */
/**
 * @}
 */


/**
  * @defgroup ST7735_colours Colour modes for ST7735 driver
  * @{
  */
#define COLMOD_12BIT  0x03  /**12bit colour mode */
#define COLMOD_16BIT  0x05  /**16bit colour mode */
#define COLMOD_18BIT  0x06  /**18bit colour mode */
/**
 * @}
 */


 #define ST7735_ROTATION_0  0x00 
 #define ST7735_ROTATION_1  0x01 
 #define ST7735_ROTATION_2  0x02  
 #define ST7735_ROTATION_3  0x03  


/**
 * @brief ST7735 display states.
 */
typedef enum{
  ST7735_DISPLAY_OFF = 0,    
  ST7735_DISPLAY_ON,         
  ST7735_DISPLAY_MAX
} ST7735_DisplayMode_E;


/**
 * @brief ST7735 transmission modes.
 * If D/CX is “low”, the transmission byte is interpreted as a
 * command byte. If D/CX is “high”, the transmission byte is stored in the display data RAM (memory write command), or
 * command register as parameter. 
 */
typedef enum{
    ST7735_WM_COMMAND = 0,     /**Incoming bytes are command bytes*/
    ST7735_WM_DATA,            /**Incoming bytes are data bytes*/
    ST7735_WM_MAX
} ST7735_WriteMode_E;


typedef union {
  struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  };
  uint8_t colour[3];
}ST7735_Colour_S;


typedef struct{
  uint16_t height;
  uint16_t width;
  uint16_t viewport_h;
  uint16_t viewport_w;
  uint32_t pixel_count;
  ST7735_Colour_S background;
  ST7735_Colour_S foreground;
  void (*pixel_write)(ST7735_Colour_S *);
}ST7735_Context_S;


void* st7735_init(int16_t x, int16_t y);
void st7735_deinit(void* ctx);
void st7735_fill_screen(void *ctx_in);
void st7735_set_background_colour(void *ctx_in, uint8_t red, uint8_t green, uint8_t blue);
void st7735_set_foreground_colour(void *ctx_in, uint8_t red, uint8_t green, uint8_t blue);
void st7735_set_colour_mode(void *ctx_in, uint8_t mode);
void st7735_display_state(void *ctx_in, uint8_t state);
void st7735_set_frame(void *ctx_in, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
void st7735_draw_pixel(void *ctx_in);
void st7735_write_bit_stream(void *ctx_in, const uint8_t *data, uint16_t size);

#endif /*__ST7735_H__*/