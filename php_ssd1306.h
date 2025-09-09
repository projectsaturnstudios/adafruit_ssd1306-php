/*
  +----------------------------------------------------------------------+
  | PHP SSD1306 Extension                                                |
  +----------------------------------------------------------------------+
  | Copyright (c) Project Saturn Studios, LLC                           |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,     |
  | that is bundled with this package in the file LICENSE, and is       |
  | available through the world-wide-web at the following url:          |
  | http://www.php.net/license/3_01.txt                                 |
  | If you did not receive a copy of the PHP license and are unable to  |
  | obtain it through the world-wide-web, please send a note to         |
  | license@php.net so we can mail you a copy immediately.             |
  +----------------------------------------------------------------------+
  | Author: Project Saturn Studios, LLC                                  |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_SSD1306_H
#define PHP_SSD1306_H

extern zend_module_entry ssd1306_module_entry;
#define phpext_ssd1306_ptr &ssd1306_module_entry

#define PHP_SSD1306_VERSION "1.0.0"

#ifdef PHP_WIN32
#	define PHP_SSD1306_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SSD1306_API __attribute__ ((visibility("default")))
#else
#	define PHP_SSD1306_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* SSD1306 Constants */
#define SSD1306_I2C_ADDRESS         0x3C
#define SSD1306_I2C_ADDRESS_ALT     0x3D

/* Display dimensions */
#define SSD1306_LCDWIDTH_128        128
#define SSD1306_LCDHEIGHT_64        64
#define SSD1306_LCDHEIGHT_32        32
#define SSD1306_LCDWIDTH_96         96
#define SSD1306_LCDHEIGHT_16        16

/* Color definitions */
#define SSD1306_BLACK               0
#define SSD1306_WHITE               1
#define SSD1306_INVERSE             2

/* VCC modes */
#define SSD1306_EXTERNALVCC         0x01
#define SSD1306_SWITCHCAPVCC        0x02

/* SSD1306 Commands */
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40

/* Scrolling commands */
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL  0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL   0x2A
#define SSD1306_DEACTIVATE_SCROLL                     0x2E
#define SSD1306_ACTIVATE_SCROLL                       0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA              0xA3

/* Structure to hold SSD1306 display state */
typedef struct {
    int i2c_fd;              /* I2C file descriptor */
    int i2c_addr;            /* I2C address */
    int width;               /* Display width */
    int height;              /* Display height */
    int pages;               /* Number of pages (height/8) */
    unsigned char *buffer;   /* Display buffer */
    int buffer_size;         /* Buffer size in bytes */
    int vcc_state;           /* VCC state (external/internal) */
    int contrast;            /* Display contrast (0-255) */
    int rotation;            /* Display rotation */
    int cursor_x;            /* Text cursor X position */
    int cursor_y;            /* Text cursor Y position */
    int text_size;           /* Text size multiplier */
    int text_color;          /* Text color */
    int text_bg_color;       /* Text background color */
    int wrap;                /* Text wrapping enabled */
} ssd1306_t;

/* Function declarations */
PHP_MINIT_FUNCTION(ssd1306);
PHP_MSHUTDOWN_FUNCTION(ssd1306);
PHP_MINFO_FUNCTION(ssd1306);

/* SSD1306 PHP Functions */
PHP_FUNCTION(ssd1306_begin);
PHP_FUNCTION(ssd1306_end);
PHP_FUNCTION(ssd1306_display);
PHP_FUNCTION(ssd1306_clear_display);
PHP_FUNCTION(ssd1306_invert_display);
PHP_FUNCTION(ssd1306_dim);
PHP_FUNCTION(ssd1306_set_contrast);
PHP_FUNCTION(ssd1306_draw_pixel);
PHP_FUNCTION(ssd1306_draw_line);
PHP_FUNCTION(ssd1306_draw_rect);
PHP_FUNCTION(ssd1306_fill_rect);
PHP_FUNCTION(ssd1306_draw_circle);
PHP_FUNCTION(ssd1306_fill_circle);
PHP_FUNCTION(ssd1306_draw_char);
PHP_FUNCTION(ssd1306_print);
PHP_FUNCTION(ssd1306_set_cursor);
PHP_FUNCTION(ssd1306_set_text_size);
PHP_FUNCTION(ssd1306_set_text_color);
PHP_FUNCTION(ssd1306_get_width);
PHP_FUNCTION(ssd1306_get_height);
PHP_FUNCTION(ssd1306_get_pixel);
PHP_FUNCTION(ssd1306_start_scroll_right);
PHP_FUNCTION(ssd1306_start_scroll_left);
PHP_FUNCTION(ssd1306_start_scroll_diag_right);
PHP_FUNCTION(ssd1306_start_scroll_diag_left);
PHP_FUNCTION(ssd1306_stop_scroll);

/* Internal C functions */
int ssd1306_init(ssd1306_t *display, int i2c_bus, int i2c_addr, int width, int height, int vcc_state);
int ssd1306_init_sequence(ssd1306_t *display);
int ssd1306_command(ssd1306_t *display, unsigned char cmd);
int ssd1306_data(ssd1306_t *display, unsigned char *data, int len);
void ssd1306_cleanup(ssd1306_t *display);
int ssd1306_update_display(ssd1306_t *display);
void ssd1306_set_pixel_internal(ssd1306_t *display, int x, int y, int color);
int ssd1306_get_pixel_internal(ssd1306_t *display, int x, int y);
void ssd1306_draw_char_internal(ssd1306_t *display, int x, int y, char c, int color, int bg, int size);

/* Global display instance */
ZEND_BEGIN_MODULE_GLOBALS(ssd1306)
    ssd1306_t *display;
ZEND_END_MODULE_GLOBALS(ssd1306)

/* Declare the global variable */
#ifdef ZTS
extern int ssd1306_globals_id;
#define SSD1306_G(v) TSRMG(ssd1306_globals_id, zend_ssd1306_globals *, v)
#else
extern zend_ssd1306_globals ssd1306_globals;
#define SSD1306_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(ssd1306, v)
#endif

#endif	/* PHP_SSD1306_H */
