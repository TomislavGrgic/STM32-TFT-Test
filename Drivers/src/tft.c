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

static void st7735_adapter_fill_screen(void *ctx, uint8_t red, uint8_t green, uint8_t blue){
    st7735_set_background_colour(ctx, red, green, blue);
    st7735_fill_screen(ctx);
}

static void st7735_adapter_set_colour(void *ctx, uint8_t red, uint8_t green, uint8_t blue){
    st7735_set_foreground_colour(ctx, red, green, blue);
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

#endif

void tft_init(TFT_S *tft, Drivers_E driver, uint16_t w, uint16_t h) {

    switch((uint8_t)driver){
#ifdef ST7735 
        case (uint8_t)ST7735_DRIVER:
            tft->init = st7735_adapter_init;
            tft->deinit = st7735_adapter_deinit;
            tft->fill_screen = st7735_adapter_fill_screen;
            tft->set_colour = st7735_adapter_set_colour;
            tft->set_cursor = st7735_adapter_set_cursor;
            tft->set_frame = st7735_adapter_set_frame;
            tft->draw_pixel = st7735_adapter_draw_pixel;
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


void tft_draw_line(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
    int16_t x_delta = x2-x1;
    int16_t y_delta = y2-y1;

    int16_t x_delta_abs = x_delta > 0 ? x_delta : -x_delta;
    int16_t y_delta_abs = y_delta > 0 ? y_delta : -y_delta;

    uint8_t is_x_primary = x_delta_abs > y_delta_abs ? 1 : 0;
    int16_t primary_start = x_delta_abs > y_delta_abs ? x1 : y1;
    int16_t primary_end = x_delta_abs > y_delta_abs ? x2 : y2;
    int8_t primary_direction = (primary_end-primary_start) > 0 ? 1 : -1; 

    int16_t secondery_start = x_delta_abs > y_delta_abs ? y1 : x1;
    int16_t secondery_end = x_delta_abs > y_delta_abs ? y2 : x2;
    int8_t secondery_direction = (secondery_end-secondery_start) > 0 ? 1 : -1; 

    int16_t primary_delta = x_delta_abs > y_delta_abs ? x_delta_abs : y_delta_abs;
    int16_t secondery_delta = x_delta_abs > y_delta_abs ? y_delta_abs : x_delta_abs;

    int16_t error = 0;
    int16_t error_tresh = primary_delta-secondery_delta;

    for(int16_t i = primary_start; i != primary_end; i += primary_direction){
        int16_t draw_x = is_x_primary ? i : secondery_start;
        int16_t draw_y = is_x_primary ? secondery_start : i;
        tft->draw_pixel(tft->context, draw_x, draw_y);
        error += secondery_delta;

        if(error >= error_tresh){
            secondery_start += secondery_direction;
            error -= primary_delta;
        }
    }
    tft->draw_pixel(tft->context, x2, y2);
}


void tft_draw_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w) {
    const int16_t x_w = x+w;
    const int16_t y_h = y+h;
    tft_draw_line(tft,x,y,x_w,y);
    tft_draw_line(tft,x,y,x,y_h);
    tft_draw_line(tft,x,y_h,x_w,y_h);
    tft_draw_line(tft,x_w,y,x_w,y_h);
}


void tft_draw_filled_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w) {
    const int16_t x_w = x+w;    
    for(int16_t i = 0; i < h; i++){
        tft_draw_line(tft,x,y+i,x_w,y+i);
    }
}

void tft_draw_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3) {
    tft_draw_line(tft,x1,y1,x2,y2);
    tft_draw_line(tft,x1,y1,x3,y3);
    tft_draw_line(tft,x2,y2,x3,y3);
}

typedef struct {
    int16_t x;
    int16_t y;
}Point;


static void tft_sort_points(Point *points){
    Point tmp_point = {0};
    if (points[0].y < points[1].y) { tmp_point = points[0]; points[0] = points[1]; points[1] = tmp_point; }
    if (points[0].y < points[2].y) { tmp_point = points[0]; points[0] = points[2]; points[2] = tmp_point; }
    if (points[1].y < points[2].y) { tmp_point = points[2]; points[2] = points[1]; points[1] = tmp_point; }
}

static float tft_slope(Point *a, Point *b){
    float ret = 0;
    if( (a->y - b->y) != 0){
        ret = (float)(a->x - b->x)/(a->y - b->y);
    }
    return ret;
}


void tft_draw_filled_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3) {
    enum { TOP = 0, MIDDLE = 1, BOTTOM = 2, POINTS = 3 };
    Point points[3] = { {x1,y1},{x2,y2},{x3,y3} };

    /*Find the coordinates*/
    tft_sort_points(points);

    /*TODO make it int based*/
    float top_bottom_rate = tft_slope(&points[TOP], &points[BOTTOM]);
    float top_middle_rate = tft_slope(&points[TOP], &points[MIDDLE]);
    float bottom_middle_rate = tft_slope(&points[MIDDLE], &points[BOTTOM]);

    float top_middle_accumulator = 0.0;
    float bottom_middle_accumulator = 0.0;
    float top_bottom_accumulator = 0.0;

    /*Draw the bottom triangle*/
    for(int16_t i = points[BOTTOM].y; i < points[MIDDLE].y; i++){
        top_bottom_accumulator += top_bottom_rate;
        bottom_middle_accumulator += bottom_middle_rate;
        int16_t x_left = points[BOTTOM].x + top_bottom_accumulator;
        int16_t x_right = points[BOTTOM].x + bottom_middle_accumulator;
        tft_draw_line(tft,x_left,i,x_right,i);
    }

   for(int16_t i = points[MIDDLE].y;i < points[TOP].y; i++){
        top_bottom_accumulator += top_bottom_rate;
        top_middle_accumulator += top_middle_rate;
        int16_t x_left = points[BOTTOM].x + top_bottom_accumulator;
        int16_t x_right = points[MIDDLE].x + top_middle_accumulator;
        tft_draw_line(tft,x_left,i,x_right,i);
    }
}


static void tft_draw_octal_pixel(TFT_S *tft, int16_t x_k, int16_t y_k, int16_t x_origin, int16_t y_origin){
    tft->draw_pixel(tft->context, x_origin+x_k, y_origin+y_k);
    tft->draw_pixel(tft->context, x_origin+y_k, y_origin+x_k);
    tft->draw_pixel(tft->context, x_origin-x_k, y_origin+y_k);
    tft->draw_pixel(tft->context, x_origin-y_k, y_origin+x_k);
    tft->draw_pixel(tft->context, x_origin-y_k, y_origin-x_k);
    tft->draw_pixel(tft->context, x_origin-x_k, y_origin-y_k);
    tft->draw_pixel(tft->context, x_origin+x_k, y_origin-y_k);
    tft->draw_pixel(tft->context, x_origin+y_k, y_origin-x_k);
}


void tft_draw_circle(TFT_S *tft, int16_t x, int16_t y, int16_t r){
    int16_t x_k = 0;
    int16_t y_k = r;
    int16_t d_k = 3-2*r;

    while(x_k < y_k){
        if( d_k < 0){
            x_k++;
            d_k = d_k + 4*x_k+6;
        } else {
            x_k++;
            y_k--;
            d_k = d_k + 4*(x_k-y_k)+10;
        }
        tft_draw_octal_pixel(tft, x_k, y_k, x, y);
    }
}