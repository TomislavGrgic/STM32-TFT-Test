#include "graphics.h"
#include "text.h"

typedef struct { 
    int16_t x; 
    int16_t y; 
}Point;


static void graphics_sort_points(Point *points){
    Point tmp_point = {0};
    if (points[0].y < points[1].y) { tmp_point = points[0]; points[0] = points[1]; points[1] = tmp_point; }
    if (points[0].y < points[2].y) { tmp_point = points[0]; points[0] = points[2]; points[2] = tmp_point; }
    if (points[1].y < points[2].y) { tmp_point = points[2]; points[2] = points[1]; points[1] = tmp_point; }
}


static float graphics_slope(Point *a, Point *b){
    float ret = 0;
    if( (a->y - b->y) != 0){
        ret = (float)(a->x - b->x)/(a->y - b->y);
    }
    return ret;
}


static void draw_octal_pixel(TFT_S *tft, int16_t x_k, int16_t y_k, int16_t x_origin, int16_t y_origin){
    tft->draw_pixel(tft->context, x_origin+x_k, y_origin+y_k);
    tft->draw_pixel(tft->context, x_origin+y_k, y_origin+x_k);
    tft->draw_pixel(tft->context, x_origin-x_k, y_origin+y_k);
    tft->draw_pixel(tft->context, x_origin-y_k, y_origin+x_k);

    tft->draw_pixel(tft->context, x_origin-y_k, y_origin-x_k);
    tft->draw_pixel(tft->context, x_origin-x_k, y_origin-y_k);
    tft->draw_pixel(tft->context, x_origin+x_k, y_origin-y_k);
    tft->draw_pixel(tft->context, x_origin+y_k, y_origin-x_k);
}


static void draw_octal_line(TFT_S *tft, int16_t x_k, int16_t y_k, int16_t x_origin, int16_t y_origin){
    draw_line(tft, x_origin+x_k, y_origin+y_k, x_origin+x_k, y_origin);
    draw_line(tft, x_origin+y_k, y_origin+x_k, x_origin+y_k, y_origin);
    draw_line(tft, x_origin-x_k, y_origin+y_k, x_origin-x_k, y_origin);
    draw_line(tft, x_origin-y_k, y_origin+x_k, x_origin-y_k, y_origin);
    draw_line(tft, x_origin-y_k, y_origin-x_k, x_origin-y_k, y_origin);
    draw_line(tft, x_origin-x_k, y_origin-y_k, x_origin-x_k, y_origin);
    draw_line(tft, x_origin+x_k, y_origin-y_k, x_origin+x_k, y_origin);
    draw_line(tft, x_origin+y_k, y_origin-x_k, x_origin+y_k, y_origin);
}


static void draw_circle_template(TFT_S *tft, int16_t x, int16_t y, int16_t r, void (*cb)(TFT_S*,int16_t,int16_t,int16_t,int16_t)){
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
        cb(tft, x_k, y_k, x, y);
    }
}


static uint16_t get_padding(uint16_t width){
    uint16_t bytes = (width>>3)+1;
    uint16_t padding = (bytes*8) - width;
    return padding == 8 ? 0 : padding;
}


void draw_line(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
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


void draw_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w) {
    const int16_t x_w = x+w;
    const int16_t y_h = y+h;
    draw_line(tft,x,y,x_w,y);
    draw_line(tft,x,y,x,y_h);
    draw_line(tft,x,y_h,x_w,y_h);
    draw_line(tft,x_w,y,x_w,y_h);
}


void draw_filled_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w) {
    tft->set_frame(tft->context,x,x+w-1,y,y+h-1);
    tft->fill_screen(tft->context);
}


void draw_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3) {
    draw_line(tft,x1,y1,x2,y2);
    draw_line(tft,x1,y1,x3,y3);
    draw_line(tft,x2,y2,x3,y3);
}


void draw_filled_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3) {
    enum { TOP = 0, MIDDLE = 1, BOTTOM = 2, POINTS = 3 };
    Point points[3] = { {x1,y1},{x2,y2},{x3,y3} };

    /*Find the coordinates*/
    graphics_sort_points(points);

    /*TODO make it int based*/
    float top_bottom_rate = graphics_slope(&points[TOP], &points[BOTTOM]);
    float top_middle_rate = graphics_slope(&points[TOP], &points[MIDDLE]);
    float bottom_middle_rate = graphics_slope(&points[MIDDLE], &points[BOTTOM]);

    float top_middle_accumulator = 0.0;
    float bottom_middle_accumulator = 0.0;
    float top_bottom_accumulator = 0.0;

    /*Draw the bottom triangle*/
    for(int16_t i = points[BOTTOM].y; i < points[MIDDLE].y; i++){
        top_bottom_accumulator += top_bottom_rate;
        bottom_middle_accumulator += bottom_middle_rate;
        int16_t x_left = points[BOTTOM].x + top_bottom_accumulator;
        int16_t x_right = points[BOTTOM].x + bottom_middle_accumulator;
        draw_line(tft,x_left,i,x_right,i);
    }

   for(int16_t i = points[MIDDLE].y;i < points[TOP].y; i++){
        top_bottom_accumulator += top_bottom_rate;
        top_middle_accumulator += top_middle_rate;
        int16_t x_left = points[BOTTOM].x + top_bottom_accumulator;
        int16_t x_right = points[MIDDLE].x + top_middle_accumulator;
        draw_line(tft,x_left,i,x_right,i);
    }
}


void draw_circle(TFT_S *tft, int16_t x, int16_t y, int16_t r){
    draw_circle_template(tft,x,y,r,draw_octal_pixel);
}


void draw_filled_circle(TFT_S *tft, int16_t x, int16_t y, int16_t r) {
    draw_circle_template(tft,x,y,r,draw_octal_line);
    draw_line(tft,x,y+r,x,y-r);
}

/*
    Heigth and width need to be multiples of 8.
*/
void draw_bitmap(TFT_S *tft, const uint8_t *data, int16_t x, int16_t y, int16_t w, int16_t h) {
    tft->set_frame(tft->context,x,x+w-1,y,y+h-1);
    tft->bitstream(tft->context,data,w*h);
}


void draw_text(TFT_S *tft, const char* text, int16_t x, int16_t y) {
    uint8_t str_size = strlen(text);
    uint8_t x_offset = 0;
    uint8_t y_offset = 0;

    for(uint8_t i = 0; i < str_size; i++){
        if(y_offset >= 128){
            x_offset += 15;
            y_offset = 0;
        }
        
        if(text[i] == ' ' && y_offset == 0){
            continue;
        }
        
        const uint8_t *character = (const uint8_t *)&text_map[text[i]];
        draw_bitmap(tft, character, x + x_offset, y+y_offset, 14, 8);
        y_offset += 8;
    }  
}