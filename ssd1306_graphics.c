/*
  +----------------------------------------------------------------------+
  | PHP SSD1306 Extension - Graphics Functions                          |
  +----------------------------------------------------------------------+
  | Copyright (c) Project Saturn Studios, LLC                           |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ssd1306.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

/* Complete 5x7 font for ASCII characters 32-126 */
static const unsigned char font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 32 (space)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // 33 !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 34 "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 35 #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 36 $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 37 %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 38 &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 39 '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // 40 (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // 41 )
    {0x08, 0x2A, 0x1C, 0x2A, 0x08}, // 42 *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 43 +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 44 ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 45 -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 46 .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 47 /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 48 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 49 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 50 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 51 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 52 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 53 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 54 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 55 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 56 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 57 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 58 :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 59 ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 60 <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 61 =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 62 >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 63 ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // 64 @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // 65 A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 66 B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 67 C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 68 D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 69 E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 70 F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 71 G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 72 H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 73 I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 74 J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 75 K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 76 L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 77 M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 78 N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 79 O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 80 P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 81 Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 82 R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 83 S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 84 T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 85 U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 86 V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 87 W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 88 X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 89 Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 90 Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // 91 [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 92 backslash
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // 93 ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 94 ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 95 _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 96 `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 97 a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 98 b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 99 c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 100 d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 101 e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 102 f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 103 g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 104 h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 105 i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 106 j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 107 k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 108 l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 109 m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 110 n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 111 o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 112 p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 113 q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 114 r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 115 s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 116 t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 117 u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 118 v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 119 w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 120 x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 121 y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 122 z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 123 {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 124 |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 125 }
    {0x10, 0x08, 0x08, 0x10, 0x08}, // 126 ~
};

/* PHP Graphics Functions */

/* {{{ proto void ssd1306_draw_pixel(int x, int y, int color)
   Draw a pixel at specified coordinates */
PHP_FUNCTION(ssd1306_draw_pixel)
{
    zend_long x, y, color;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "lll", &x, &y, &color) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    ssd1306_set_pixel_internal(SSD1306_G(display), x, y, color);
}
/* }}} */

/* {{{ proto void ssd1306_draw_line(int x0, int y0, int x1, int y1, int color)
   Draw a line between two points */
PHP_FUNCTION(ssd1306_draw_line)
{
    zend_long x0, y0, x1, y1, color;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "lllll", &x0, &y0, &x1, &y1, &color) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    /* Bresenham's line algorithm */
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        ssd1306_set_pixel_internal(SSD1306_G(display), x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
/* }}} */

/* {{{ proto void ssd1306_draw_rect(int x, int y, int w, int h, int color)
   Draw a rectangle outline */
PHP_FUNCTION(ssd1306_draw_rect)
{
    zend_long x, y, w, h, color;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "lllll", &x, &y, &w, &h, &color) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    /* Draw rectangle outline */
    for (int i = 0; i < w; i++) {
        ssd1306_set_pixel_internal(SSD1306_G(display), x + i, y, color);         /* Top */
        ssd1306_set_pixel_internal(SSD1306_G(display), x + i, y + h - 1, color); /* Bottom */
    }
    for (int i = 0; i < h; i++) {
        ssd1306_set_pixel_internal(SSD1306_G(display), x, y + i, color);         /* Left */
        ssd1306_set_pixel_internal(SSD1306_G(display), x + w - 1, y + i, color); /* Right */
    }
}
/* }}} */

/* {{{ proto void ssd1306_fill_rect(int x, int y, int w, int h, int color)
   Draw a filled rectangle */
PHP_FUNCTION(ssd1306_fill_rect)
{
    zend_long x, y, w, h, color;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "lllll", &x, &y, &w, &h, &color) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    /* Fill rectangle */
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            ssd1306_set_pixel_internal(SSD1306_G(display), x + i, y + j, color);
        }
    }
}
/* }}} */

/* {{{ proto void ssd1306_draw_circle(int x0, int y0, int r, int color)
   Draw a circle outline */
PHP_FUNCTION(ssd1306_draw_circle)
{
    zend_long x0, y0, r, color;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "llll", &x0, &y0, &r, &color) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    /* Bresenham's circle algorithm */
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        /* Draw 8 octants */
        ssd1306_set_pixel_internal(SSD1306_G(display), x0 + x, y0 + y, color);
        ssd1306_set_pixel_internal(SSD1306_G(display), x0 - x, y0 + y, color);
        ssd1306_set_pixel_internal(SSD1306_G(display), x0 + x, y0 - y, color);
        ssd1306_set_pixel_internal(SSD1306_G(display), x0 - x, y0 - y, color);
        ssd1306_set_pixel_internal(SSD1306_G(display), x0 + y, y0 + x, color);
        ssd1306_set_pixel_internal(SSD1306_G(display), x0 - y, y0 + x, color);
        ssd1306_set_pixel_internal(SSD1306_G(display), x0 + y, y0 - x, color);
        ssd1306_set_pixel_internal(SSD1306_G(display), x0 - y, y0 - x, color);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
/* }}} */

/* {{{ proto void ssd1306_fill_circle(int x0, int y0, int r, int color)
   Draw a filled circle */
PHP_FUNCTION(ssd1306_fill_circle)
{
    zend_long x0, y0, r, color;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "llll", &x0, &y0, &r, &color) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    /* Fill circle using horizontal lines */
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x * x + y * y <= r * r) {
                ssd1306_set_pixel_internal(SSD1306_G(display), x0 + x, y0 + y, color);
            }
        }
    }
}
/* }}} */

/* {{{ proto void ssd1306_print(string text)
   Print text at current cursor position */
PHP_FUNCTION(ssd1306_print)
{
    char *text;
    size_t text_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &text, &text_len) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    ssd1306_t *display = SSD1306_G(display);
    
    for (size_t i = 0; i < text_len; i++) {
        char c = text[i];
        
        if (c == '\n') {
            display->cursor_y += 8 * display->text_size;
            display->cursor_x = 0;
            continue;
        }
        
        if (c == '\r') {
            display->cursor_x = 0;
            continue;
        }
        
        /* Handle character wrapping */
        if (display->wrap && (display->cursor_x + 6 * display->text_size > display->width)) {
            display->cursor_x = 0;
            display->cursor_y += 8 * display->text_size;
        }
        
        /* Skip if we're past the bottom */
        if (display->cursor_y >= display->height) {
            break;
        }
        
        /* Draw character */
        ssd1306_draw_char_internal(display, display->cursor_x, display->cursor_y, c, 
                                  display->text_color, display->text_bg_color, display->text_size);
        
        display->cursor_x += 6 * display->text_size;
    }
}
/* }}} */

/* {{{ proto void ssd1306_set_cursor(int x, int y)
   Set text cursor position */
PHP_FUNCTION(ssd1306_set_cursor)
{
    zend_long x, y;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &x, &y) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    SSD1306_G(display)->cursor_x = x;
    SSD1306_G(display)->cursor_y = y;
}
/* }}} */

/* {{{ proto void ssd1306_set_text_size(int size)
   Set text size multiplier */
PHP_FUNCTION(ssd1306_set_text_size)
{
    zend_long size;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &size) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    if (size < 1) size = 1;
    SSD1306_G(display)->text_size = size;
}
/* }}} */

/* {{{ proto void ssd1306_set_text_color(int color)
   Set text color */
PHP_FUNCTION(ssd1306_set_text_color)
{
    zend_long color;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &color) == FAILURE) {
        return;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    SSD1306_G(display)->text_color = color;
}
/* }}} */

/* {{{ proto int ssd1306_get_width()
   Get display width */
PHP_FUNCTION(ssd1306_get_width)
{
    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_LONG(0);
    }

    RETURN_LONG(SSD1306_G(display)->width);
}
/* }}} */

/* {{{ proto int ssd1306_get_height()
   Get display height */
PHP_FUNCTION(ssd1306_get_height)
{
    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_LONG(0);
    }

    RETURN_LONG(SSD1306_G(display)->height);
}
/* }}} */

/* {{{ proto int ssd1306_get_pixel(int x, int y)
   Get pixel color at coordinates */
PHP_FUNCTION(ssd1306_get_pixel)
{
    zend_long x, y;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &x, &y) == FAILURE) {
        RETURN_LONG(0);
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_LONG(0);
    }

    RETURN_LONG(ssd1306_get_pixel_internal(SSD1306_G(display), x, y));
}
/* }}} */

/* Internal function to draw a character */
void ssd1306_draw_char_internal(ssd1306_t *display, int x, int y, char c, int color, int bg, int size)
{
    if (c < 32 || c > 126) c = 32; /* Replace non-printable with space */
    
    int char_index = c - 32;
    if (char_index >= sizeof(font5x7) / sizeof(font5x7[0])) {
        char_index = 0; /* Default to space */
    }
    
    for (int i = 0; i < 5; i++) {
        unsigned char line = font5x7[char_index][i];
        for (int j = 0; j < 8; j++) {
            if (line & (1 << j)) {
                if (size == 1) {
                    ssd1306_set_pixel_internal(display, x + i, y + j, color);
                } else {
                    /* Draw scaled character */
                    for (int a = 0; a < size; a++) {
                        for (int b = 0; b < size; b++) {
                            ssd1306_set_pixel_internal(display, x + i * size + a, y + j * size + b, color);
                        }
                    }
                }
            } else if (bg != color) {
                if (size == 1) {
                    ssd1306_set_pixel_internal(display, x + i, y + j, bg);
                } else {
                    /* Draw scaled background */
                    for (int a = 0; a < size; a++) {
                        for (int b = 0; b < size; b++) {
                            ssd1306_set_pixel_internal(display, x + i * size + a, y + j * size + b, bg);
                        }
                    }
                }
            }
        }
    }
}
