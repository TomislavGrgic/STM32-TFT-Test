#include "graphics.h"
#include "text.h"

const float cos_lut[] = {
    1.000000, 0.999848, 0.999391, 0.998630, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808, 0.981627, 0.978148, 0.974370, 0.970296, 0.965926, 0.961262, 0.956305, 0.951057, 0.945519, 0.939693, 0.933580, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891007, 0.882948, 0.874620, 0.866025, 0.857167, 0.848048, 0.838671, 0.829038, 0.819152, 0.809017, 0.798636, 0.788011, 0.777146, 0.766044, 0.754710, 0.743145, 0.731354, 0.719340, 0.707107, 0.694658, 0.681998, 0.669131, 0.656059, 0.642788, 0.629320, 0.615661, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639, 0.529919, 0.515038, 0.500000, 0.484810, 0.469472, 0.453991, 0.438371, 0.422618, 0.406737, 0.390731, 0.374607, 0.358368, 0.342020, 0.325568, 0.309017, 0.292372, 0.275637, 0.258819, 0.241922, 0.224951, 0.207912, 0.190809, 0.173648, 0.156434, 0.139173, 0.121869, 0.104528, 0.087156, 0.069757, 0.052336, 0.034899, 0.017452, -0.000000, -0.017452, -0.034899, -0.052336, -0.069756, -0.087156, -0.104529, -0.121869, -0.139173, -0.156434, -0.173648, -0.190809, -0.207912, -0.224951, -0.241922, -0.258819, -0.275637, -0.292372, -0.309017, -0.325568, -0.342020, -0.358368, -0.374607, -0.390731, -0.406737, -0.422618, -0.438371, -0.453991, -0.469472, -0.484810, -0.500000, -0.515038, -0.529919, -0.544639, -0.559193, -0.573576, -0.587785, -0.601815, -0.615661, -0.629320, -0.642788, -0.656059, -0.669131, -0.681998, -0.694658, -0.707107, -0.719340, -0.731354, -0.743145, -0.754710, -0.766044, -0.777146, -0.788011, -0.798635, -0.809017, -0.819152, -0.829038, -0.838671, -0.848048, -0.857167, -0.866025, -0.874620, -0.882948, -0.891006, -0.898794, -0.906308, -0.913545, -0.920505, -0.927184, -0.933580, -0.939693, -0.945519, -0.951057, -0.956305, -0.961262, -0.965926, -0.970296, -0.974370, -0.978148, -0.981627, -0.984808, -0.987688, -0.990268, -0.992546, -0.994522, -0.996195, -0.997564, -0.998630, -0.999391, -0.999848, -1.000000, -0.999848, -0.999391, -0.998630, -0.997564, -0.996195, -0.994522, -0.992546, -0.990268, -0.987688, -0.984808, -0.981627, -0.978148, -0.974370, -0.970296, -0.965926, -0.961262, -0.956305, -0.951057, -0.945519, -0.939693, -0.933580, -0.927184, -0.920505, -0.913545, -0.906308, -0.898794, -0.891007, -0.882948, -0.874620, -0.866025, -0.857167, -0.848048, -0.838671, -0.829038, -0.819152, -0.809017, -0.798636, -0.788011, -0.777146, -0.766044, -0.754710, -0.743145, -0.731354, -0.719340, -0.707107, -0.694658, -0.681998, -0.669131, -0.656059, -0.642787, -0.629320, -0.615662, -0.601815, -0.587785, -0.573576, -0.559193, -0.544639, -0.529919, -0.515038, -0.500000, -0.484810, -0.469472, -0.453991, -0.438371, -0.422618, -0.406737, -0.390731, -0.374607, -0.358368, -0.342020, -0.325568, -0.309017, -0.292372, -0.275637, -0.258819, -0.241922, -0.224951, -0.207912, -0.190809, -0.173648, -0.156435, -0.139173, -0.121869, -0.104528, -0.087156, -0.069757, -0.052336, -0.034899, -0.017452, 0.000000, 0.017452, 0.034899, 0.052336, 0.069757, 0.087156, 0.104528, 0.121869, 0.139173, 0.156435, 0.173648, 0.190809, 0.207911, 0.224951, 0.241922, 0.258819, 0.275637, 0.292371, 0.309017, 0.325568, 0.342020, 0.358368, 0.374607, 0.390731, 0.406737, 0.422618, 0.438371, 0.453991, 0.469472, 0.484810, 0.500000, 0.515038, 0.529919, 0.544639, 0.559193, 0.573576, 0.587785, 0.601815, 0.615662, 0.629320, 0.642788, 0.656059, 0.669131, 0.681998, 0.694658, 0.707107, 0.719340, 0.731354, 0.743145, 0.754710, 0.766044, 0.777146, 0.788011, 0.798636, 0.809017, 0.819152, 0.829038, 0.838671, 0.848048, 0.857167, 0.866025, 0.874620, 0.882948, 0.891007, 0.898794, 0.906308, 0.913546, 0.920505, 0.927184, 0.933580, 0.939693, 0.945519, 0.951057, 0.956305, 0.961262, 0.965926, 0.970296, 0.974370, 0.978148, 0.981627, 0.984808, 0.987688, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.998630, 0.999391, 0.999848
};

const float sin_lut[] = {
    0.000000, 0.017452, 0.034899, 0.052336, 0.069756, 0.087156, 0.104528, 0.121869, 0.139173, 0.156434, 0.173648, 0.190809, 0.207912, 0.224951, 0.241922, 0.258819, 0.275637, 0.292372, 0.309017, 0.325568, 0.342020, 0.358368, 0.374607, 0.390731, 0.406737, 0.422618, 0.438371, 0.453991, 0.469472, 0.484810, 0.500000, 0.515038, 0.529919, 0.544639, 0.559193, 0.573576, 0.587785, 0.601815, 0.615661, 0.629320, 0.642788, 0.656059, 0.669131, 0.681998, 0.694658, 0.707107, 0.719340, 0.731354, 0.743145, 0.754710, 0.766044, 0.777146, 0.788011, 0.798636, 0.809017, 0.819152, 0.829038, 0.838671, 0.848048, 0.857167, 0.866025, 0.874620, 0.882948, 0.891007, 0.898794, 0.906308, 0.913545, 0.920505, 0.927184, 0.933580, 0.939693, 0.945519, 0.951057, 0.956305, 0.961262, 0.965926, 0.970296, 0.974370, 0.978148, 0.981627, 0.984808, 0.987688, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.998630, 0.999391, 0.999848, 1.000000, 0.999848, 0.999391, 0.998630, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808, 0.981627, 0.978148, 0.974370, 0.970296, 0.965926, 0.961262, 0.956305, 0.951057, 0.945519, 0.939693, 0.933580, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891006, 0.882948, 0.874620, 0.866025, 0.857167, 0.848048, 0.838671, 0.829038, 0.819152, 0.809017, 0.798635, 0.788011, 0.777146, 0.766044, 0.754710, 0.743145, 0.731354, 0.719340, 0.707107, 0.694658, 0.681998, 0.669131, 0.656059, 0.642788, 0.629320, 0.615661, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639, 0.529919, 0.515038, 0.500000, 0.484810, 0.469472, 0.453991, 0.438371, 0.422618, 0.406737, 0.390731, 0.374607, 0.358368, 0.342020, 0.325568, 0.309017, 0.292372, 0.275637, 0.258819, 0.241922, 0.224951, 0.207912, 0.190809, 0.173648, 0.156434, 0.139173, 0.121869, 0.104528, 0.087156, 0.069756, 0.052336, 0.034899, 0.017452, -0.000000, -0.017452, -0.034899, -0.052336, -0.069756, -0.087156, -0.104528, -0.121869, -0.139173, -0.156434, -0.173648, -0.190809, -0.207912, -0.224951, -0.241922, -0.258819, -0.275637, -0.292372, -0.309017, -0.325568, -0.342020, -0.358368, -0.374607, -0.390731, -0.406737, -0.422618, -0.438371, -0.453991, -0.469472, -0.484810, -0.500000, -0.515038, -0.529919, -0.544639, -0.559193, -0.573576, -0.587785, -0.601815, -0.615661, -0.629320, -0.642788, -0.656059, -0.669131, -0.681998, -0.694658, -0.707107, -0.719340, -0.731354, -0.743145, -0.754710, -0.766045, -0.777146, -0.788011, -0.798635, -0.809017, -0.819152, -0.829038, -0.838671, -0.848048, -0.857167, -0.866025, -0.874620, -0.882948, -0.891006, -0.898794, -0.906308, -0.913545, -0.920505, -0.927184, -0.933580, -0.939693, -0.945519, -0.951056, -0.956305, -0.961262, -0.965926, -0.970296, -0.974370, -0.978148, -0.981627, -0.984808, -0.987688, -0.990268, -0.992546, -0.994522, -0.996195, -0.997564, -0.998630, -0.999391, -0.999848, -1.000000, -0.999848, -0.999391, -0.998630, -0.997564, -0.996195, -0.994522, -0.992546, -0.990268, -0.987688, -0.984808, -0.981627, -0.978148, -0.974370, -0.970296, -0.965926, -0.961262, -0.956305, -0.951056, -0.945519, -0.939693, -0.933580, -0.927184, -0.920505, -0.913545, -0.906308, -0.898794, -0.891006, -0.882948, -0.874620, -0.866025, -0.857167, -0.848048, -0.838671, -0.829038, -0.819152, -0.809017, -0.798635, -0.788011, -0.777146, -0.766045, -0.754710, -0.743145, -0.731354, -0.719340, -0.707107, -0.694659, -0.681998, -0.669131, -0.656059, -0.642788, -0.629320, -0.615661, -0.601815, -0.587785, -0.573577, -0.559193, -0.544639, -0.529919, -0.515038, -0.500000, -0.484809, -0.469471, -0.453991, -0.438371, -0.422618, -0.406736, -0.390731, -0.374607, -0.358368, -0.342020, -0.325568, -0.309017, -0.292372, -0.275638, -0.258819, -0.241922, -0.224951, -0.207912, -0.190809, -0.173648, -0.156434, -0.139173, -0.121869, -0.104529, -0.087156, -0.069756, -0.052336, -0.034900, -0.017453
};


typedef struct { 
    int16_t x; 
    int16_t y; 
}Point;


static float graphics_cos(uint16_t deg) {
    uint16_t max_deg = (sizeof(cos_lut)/sizeof(float));
    while( deg >= max_deg ){
        deg -= max_deg;
    }
    return cos_lut[deg];
}


static float graphics_sin(uint16_t deg) {
    uint16_t max_deg = (sizeof(sin_lut)/sizeof(float));
    while( deg >= max_deg ){
        deg -= max_deg;
    }
    return sin_lut[deg];
}


static void graphics_rotate_point(Point *point, uint16_t deg){
    int16_t x = point->x;
    int16_t y = point->y;
    point->x = x*graphics_cos(deg)-y*graphics_sin(deg);
    point->y = x*graphics_sin(deg)+y*graphics_cos(deg);
}


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
    draw_fast_line(tft, x_origin+x_k, y_origin+y_k, x_origin+x_k, y_origin);
    draw_fast_line(tft, x_origin+y_k, y_origin+x_k, x_origin+y_k, y_origin);
    draw_fast_line(tft, x_origin-x_k, y_origin+y_k, x_origin-x_k, y_origin);
    draw_fast_line(tft, x_origin-y_k, y_origin+x_k, x_origin-y_k, y_origin);
    draw_fast_line(tft, x_origin-y_k, y_origin-x_k, x_origin-y_k, y_origin);
    draw_fast_line(tft, x_origin-x_k, y_origin-y_k, x_origin-x_k, y_origin);
    draw_fast_line(tft, x_origin+x_k, y_origin-y_k, x_origin+x_k, y_origin);
    draw_fast_line(tft, x_origin+y_k, y_origin-x_k, x_origin+y_k, y_origin);
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


void draw_fast_line(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if( (x1 != x2) && (y1 != y2) ){
        draw_line(tft, x1, y1, x2, y2);
        return;
    }

    const uint16_t x_start = x1 < x2 ? x1 : x2;
    const uint16_t x_stop = x2 < x1 ? x1 : x2;
    const uint16_t y_start = y1 < y2 ? y1 : y2;
    const uint16_t y_stop = y2 < y1 ? y1 : y2;

    tft->set_frame(tft->context,x_start,x_stop,y_start,y_stop);
    tft->fill_screen(tft->context);
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


/** If no rotation needed, use the fast version. */
void draw_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w, uint16_t deg) {
    enum {NUM_OF_POINTS = 4, };
    Point points[NUM_OF_POINTS] = { {x,y}, {x+w,y}, {x,y+h}, {x+w,y+h} };
    if( deg == 0 ){
        draw_fast_line(tft,points[0].x,points[0].y,points[1].x,points[1].y);
        draw_fast_line(tft,points[0].x,points[0].y,points[2].x,points[2].y);
        draw_fast_line(tft,points[3].x,points[3].y,points[1].x,points[1].y);
        draw_fast_line(tft,points[3].x,points[3].y,points[2].x,points[2].y);
        return;
    }

    const Point center = {x+(w>>1),y+(h>>1)};

    for(uint16_t i = 0; i < NUM_OF_POINTS; i++){
        points[i].x -= center.x;
        points[i].y -= center.y;
    }

    for(uint16_t i = 0; i < NUM_OF_POINTS; i++){
        graphics_rotate_point(&points[i], deg);
    }
    
    for(uint16_t i = 0; i < NUM_OF_POINTS; i++){
        points[i].x += center.x;
        points[i].y += center.y;
    }

    draw_fast_line(tft,points[0].x,points[0].y,points[1].x,points[1].y);
    draw_fast_line(tft,points[0].x,points[0].y,points[2].x,points[2].y);
    draw_fast_line(tft,points[3].x,points[3].y,points[1].x,points[1].y);
    draw_fast_line(tft,points[3].x,points[3].y,points[2].x,points[2].y);
}


/** If no rotation needed, use the fast version. */
void draw_filled_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w, uint16_t deg) {
    enum {NUM_OF_POINTS = 4, };
    Point points[NUM_OF_POINTS] = { {x,y}, {x+w,y}, {x,y+h}, {x+w,y+h} };

    if( deg == 0 ){
        draw_filled_triangle(tft, points[0].x,points[0].y,points[1].x,points[1].y,points[2].x,points[2].y);
        draw_filled_triangle(tft, points[3].x,points[3].y,points[1].x,points[1].y,points[2].x,points[2].y);
        return; 
    }
    
    const Point center = {x+(w>>1),y+(h>>1)};

    for(uint16_t i = 0; i < NUM_OF_POINTS; i++){
        points[i].x -= center.x;
        points[i].y -= center.y;
    }

    for(uint16_t i = 0; i < NUM_OF_POINTS; i++){
        graphics_rotate_point(&points[i], deg);
    }
    
    for(uint16_t i = 0; i < NUM_OF_POINTS; i++){
        points[i].x += center.x;
        points[i].y += center.y;
    }

    draw_filled_triangle(tft, points[0].x,points[0].y,points[1].x,points[1].y,points[2].x,points[2].y);
    draw_filled_triangle(tft, points[3].x,points[3].y,points[1].x,points[1].y,points[2].x,points[2].y);
}



void draw_fast_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w) {
    const int16_t x_w = x+w;
    const int16_t y_h = y+h;
    draw_fast_line(tft,x,y,x_w,y);
    draw_fast_line(tft,x,y,x,y_h);
    draw_fast_line(tft,x,y_h,x_w,y_h);
    draw_fast_line(tft,x_w,y,x_w,y_h);
}


void draw_fast_filled_rectangle(TFT_S *tft, uint16_t x, uint16_t y, uint16_t h, uint16_t w) {
    tft->set_frame(tft->context,x,x+w-1,y,y+h-1);
    tft->fill_screen(tft->context);
}


void draw_triangle(TFT_S *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3) {
    draw_fast_line(tft,x1,y1,x2,y2);
    draw_fast_line(tft,x1,y1,x3,y3);
    draw_fast_line(tft,x2,y2,x3,y3);
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
        draw_fast_line(tft,x_left,i,x_right,i);
    }

   for(int16_t i = points[MIDDLE].y;i < points[TOP].y; i++){
        top_bottom_accumulator += top_bottom_rate;
        top_middle_accumulator += top_middle_rate;
        int16_t x_left = points[BOTTOM].x + top_bottom_accumulator;
        int16_t x_right = points[MIDDLE].x + top_middle_accumulator;
        draw_fast_line(tft,x_left,i,x_right,i);
    }
}


void draw_circle(TFT_S *tft, int16_t x, int16_t y, int16_t r){
    draw_circle_template(tft,x,y,r,draw_octal_pixel);
}


void draw_filled_circle(TFT_S *tft, int16_t x, int16_t y, int16_t r) {
    draw_fast_line(tft,x,y+r,x,y-r);
    draw_circle_template(tft,x,y,r,draw_octal_line);
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

    /*TODO remove magic nums*/
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