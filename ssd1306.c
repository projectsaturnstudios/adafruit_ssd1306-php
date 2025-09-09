/*
  +----------------------------------------------------------------------+
  | PHP SSD1306 Extension                                                |
  +----------------------------------------------------------------------+
  | Copyright (c) Project Saturn Studios, LLC                           |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ssd1306.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <stdlib.h>

/* Global variables */
ZEND_DECLARE_MODULE_GLOBALS(ssd1306)

/* Define the global variable */
#ifdef ZTS
int ssd1306_globals_id;
#else
zend_ssd1306_globals ssd1306_globals;
#endif

/* Initialize globals */
static void php_ssd1306_init_globals(zend_ssd1306_globals *ssd1306_globals)
{
    ssd1306_globals->display = NULL;
}

/* Function argument info */
ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_begin, 0, 0, 0)
    ZEND_ARG_INFO(0, i2c_bus)
    ZEND_ARG_INFO(0, i2c_addr)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, height)
    ZEND_ARG_INFO(0, vcc_state)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_void, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_bool, 0, 0, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_int, 0, 0, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_draw_pixel, 0, 0, 3)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
    ZEND_ARG_INFO(0, color)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_draw_line, 0, 0, 5)
    ZEND_ARG_INFO(0, x0)
    ZEND_ARG_INFO(0, y0)
    ZEND_ARG_INFO(0, x1)
    ZEND_ARG_INFO(0, y1)
    ZEND_ARG_INFO(0, color)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_draw_rect, 0, 0, 5)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
    ZEND_ARG_INFO(0, w)
    ZEND_ARG_INFO(0, h)
    ZEND_ARG_INFO(0, color)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_draw_circle, 0, 0, 4)
    ZEND_ARG_INFO(0, x0)
    ZEND_ARG_INFO(0, y0)
    ZEND_ARG_INFO(0, r)
    ZEND_ARG_INFO(0, color)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_print, 0, 0, 1)
    ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_set_cursor, 0, 0, 2)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_get_pixel, 0, 0, 2)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ssd1306_scroll, 0, 0, 2)
    ZEND_ARG_INFO(0, start)
    ZEND_ARG_INFO(0, stop)
ZEND_END_ARG_INFO()

/* Function entries */
const zend_function_entry ssd1306_functions[] = {
    PHP_FE(ssd1306_begin,                arginfo_ssd1306_begin)
    PHP_FE(ssd1306_end,                  arginfo_ssd1306_void)
    PHP_FE(ssd1306_display,              arginfo_ssd1306_void)
    PHP_FE(ssd1306_clear_display,        arginfo_ssd1306_void)
    PHP_FE(ssd1306_invert_display,       arginfo_ssd1306_bool)
    PHP_FE(ssd1306_dim,                  arginfo_ssd1306_bool)
    PHP_FE(ssd1306_set_contrast,         arginfo_ssd1306_int)
    PHP_FE(ssd1306_draw_pixel,           arginfo_ssd1306_draw_pixel)
    PHP_FE(ssd1306_draw_line,            arginfo_ssd1306_draw_line)
    PHP_FE(ssd1306_draw_rect,            arginfo_ssd1306_draw_rect)
    PHP_FE(ssd1306_fill_rect,            arginfo_ssd1306_draw_rect)
    PHP_FE(ssd1306_draw_circle,          arginfo_ssd1306_draw_circle)
    PHP_FE(ssd1306_fill_circle,          arginfo_ssd1306_draw_circle)
    PHP_FE(ssd1306_print,                arginfo_ssd1306_print)
    PHP_FE(ssd1306_set_cursor,           arginfo_ssd1306_set_cursor)
    PHP_FE(ssd1306_set_text_size,        arginfo_ssd1306_int)
    PHP_FE(ssd1306_set_text_color,       arginfo_ssd1306_int)
    PHP_FE(ssd1306_get_width,            arginfo_ssd1306_void)
    PHP_FE(ssd1306_get_height,           arginfo_ssd1306_void)
    PHP_FE(ssd1306_get_pixel,            arginfo_ssd1306_get_pixel)
    PHP_FE(ssd1306_start_scroll_right,   arginfo_ssd1306_scroll)
    PHP_FE(ssd1306_start_scroll_left,    arginfo_ssd1306_scroll)
    PHP_FE(ssd1306_start_scroll_diag_right, arginfo_ssd1306_scroll)
    PHP_FE(ssd1306_start_scroll_diag_left,  arginfo_ssd1306_scroll)
    PHP_FE(ssd1306_stop_scroll,          arginfo_ssd1306_void)
    PHP_FE_END
};

/* Module entry */
zend_module_entry ssd1306_module_entry = {
    STANDARD_MODULE_HEADER,
    "ssd1306",
    ssd1306_functions,
    PHP_MINIT(ssd1306),
    PHP_MSHUTDOWN(ssd1306),
    NULL,
    NULL,
    PHP_MINFO(ssd1306),
    PHP_SSD1306_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SSD1306
ZEND_GET_MODULE(ssd1306)
#endif


/* Module initialization */
PHP_MINIT_FUNCTION(ssd1306)
{
    ZEND_INIT_MODULE_GLOBALS(ssd1306, php_ssd1306_init_globals, NULL);
    
    /* Register constants */
    REGISTER_LONG_CONSTANT("SSD1306_I2C_ADDRESS", SSD1306_I2C_ADDRESS, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SSD1306_I2C_ADDRESS_ALT", SSD1306_I2C_ADDRESS_ALT, CONST_CS | CONST_PERSISTENT);
    
    REGISTER_LONG_CONSTANT("SSD1306_LCDWIDTH_128", SSD1306_LCDWIDTH_128, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SSD1306_LCDHEIGHT_64", SSD1306_LCDHEIGHT_64, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SSD1306_LCDHEIGHT_32", SSD1306_LCDHEIGHT_32, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SSD1306_LCDWIDTH_96", SSD1306_LCDWIDTH_96, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SSD1306_LCDHEIGHT_16", SSD1306_LCDHEIGHT_16, CONST_CS | CONST_PERSISTENT);
    
    REGISTER_LONG_CONSTANT("SSD1306_BLACK", SSD1306_BLACK, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SSD1306_WHITE", SSD1306_WHITE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SSD1306_INVERSE", SSD1306_INVERSE, CONST_CS | CONST_PERSISTENT);
    
    REGISTER_LONG_CONSTANT("SSD1306_EXTERNALVCC", SSD1306_EXTERNALVCC, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SSD1306_SWITCHCAPVCC", SSD1306_SWITCHCAPVCC, CONST_CS | CONST_PERSISTENT);

    return SUCCESS;
}

/* Module shutdown */
PHP_MSHUTDOWN_FUNCTION(ssd1306)
{
    if (SSD1306_G(display)) {
        ssd1306_cleanup(SSD1306_G(display));
        efree(SSD1306_G(display));
        SSD1306_G(display) = NULL;
    }
    return SUCCESS;
}

/* Module info */
PHP_MINFO_FUNCTION(ssd1306)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "SSD1306 OLED Display Support", "enabled");
    php_info_print_table_row(2, "Version", PHP_SSD1306_VERSION);
    php_info_print_table_row(2, "I2C Support", "enabled");
    php_info_print_table_end();
}

/* PHP Functions */

/* {{{ proto bool ssd1306_begin([int i2c_bus, int i2c_addr, int width, int height, int vcc_state])
   Initialize SSD1306 display */
PHP_FUNCTION(ssd1306_begin)
{
    zend_long i2c_bus = 1;
    zend_long i2c_addr = SSD1306_I2C_ADDRESS;
    zend_long width = SSD1306_LCDWIDTH_128;
    zend_long height = SSD1306_LCDHEIGHT_64;
    zend_long vcc_state = SSD1306_SWITCHCAPVCC;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "|lllll", &i2c_bus, &i2c_addr, &width, &height, &vcc_state) == FAILURE) {
        RETURN_FALSE;
    }

    /* Clean up existing display if any */
    if (SSD1306_G(display)) {
        ssd1306_cleanup(SSD1306_G(display));
        efree(SSD1306_G(display));
    }

    /* Allocate new display structure */
    SSD1306_G(display) = emalloc(sizeof(ssd1306_t));
    memset(SSD1306_G(display), 0, sizeof(ssd1306_t));

    /* Initialize display */
    if (ssd1306_init(SSD1306_G(display), i2c_bus, i2c_addr, width, height, vcc_state) != 0) {
        efree(SSD1306_G(display));
        SSD1306_G(display) = NULL;
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto void ssd1306_end()
   Cleanup SSD1306 display */
PHP_FUNCTION(ssd1306_end)
{
    if (SSD1306_G(display)) {
        ssd1306_cleanup(SSD1306_G(display));
        efree(SSD1306_G(display));
        SSD1306_G(display) = NULL;
    }
}
/* }}} */

/* {{{ proto bool ssd1306_display()
   Update display with buffer contents */
PHP_FUNCTION(ssd1306_display)
{
    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    if (ssd1306_update_display(SSD1306_G(display)) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto void ssd1306_clear_display()
   Clear display buffer */
PHP_FUNCTION(ssd1306_clear_display)
{
    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        return;
    }

    memset(SSD1306_G(display)->buffer, 0, SSD1306_G(display)->buffer_size);
}
/* }}} */

/* {{{ proto bool ssd1306_invert_display(bool invert)
   Invert display colors */
PHP_FUNCTION(ssd1306_invert_display)
{
    zend_bool invert;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "b", &invert) == FAILURE) {
        RETURN_FALSE;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    unsigned char cmd = invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY;
    if (ssd1306_command(SSD1306_G(display), cmd) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ssd1306_dim(bool dim)
   Dim display */
PHP_FUNCTION(ssd1306_dim)
{
    zend_bool dim;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "b", &dim) == FAILURE) {
        RETURN_FALSE;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    unsigned char contrast = dim ? 0 : SSD1306_G(display)->contrast;
    if (ssd1306_command(SSD1306_G(display), SSD1306_SETCONTRAST) != 0 ||
        ssd1306_command(SSD1306_G(display), contrast) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ssd1306_set_contrast(int contrast)
   Set display contrast (0-255) */
PHP_FUNCTION(ssd1306_set_contrast)
{
    zend_long contrast;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &contrast) == FAILURE) {
        RETURN_FALSE;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    if (contrast < 0 || contrast > 255) {
        php_error_docref(NULL, E_WARNING, "Contrast must be between 0 and 255");
        RETURN_FALSE;
    }

    SSD1306_G(display)->contrast = contrast;
    
    if (ssd1306_command(SSD1306_G(display), SSD1306_SETCONTRAST) != 0 ||
        ssd1306_command(SSD1306_G(display), contrast) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ssd1306_start_scroll_right(int start, int stop)
   Start horizontal scroll right */
PHP_FUNCTION(ssd1306_start_scroll_right)
{
    zend_long start, stop;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &start, &stop) == FAILURE) {
        RETURN_FALSE;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    if (ssd1306_command(SSD1306_G(display), SSD1306_RIGHT_HORIZONTAL_SCROLL) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||  /* Dummy byte */
        ssd1306_command(SSD1306_G(display), start) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||  /* Time interval */
        ssd1306_command(SSD1306_G(display), stop) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||  /* Dummy byte */
        ssd1306_command(SSD1306_G(display), 0xFF) != 0 ||  /* Dummy byte */
        ssd1306_command(SSD1306_G(display), SSD1306_ACTIVATE_SCROLL) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ssd1306_start_scroll_left(int start, int stop)
   Start horizontal scroll left */
PHP_FUNCTION(ssd1306_start_scroll_left)
{
    zend_long start, stop;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &start, &stop) == FAILURE) {
        RETURN_FALSE;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    if (ssd1306_command(SSD1306_G(display), SSD1306_LEFT_HORIZONTAL_SCROLL) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||  /* Dummy byte */
        ssd1306_command(SSD1306_G(display), start) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||  /* Time interval */
        ssd1306_command(SSD1306_G(display), stop) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||  /* Dummy byte */
        ssd1306_command(SSD1306_G(display), 0xFF) != 0 ||  /* Dummy byte */
        ssd1306_command(SSD1306_G(display), SSD1306_ACTIVATE_SCROLL) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ssd1306_start_scroll_diag_right(int start, int stop)
   Start diagonal scroll right */
PHP_FUNCTION(ssd1306_start_scroll_diag_right)
{
    zend_long start, stop;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &start, &stop) == FAILURE) {
        RETURN_FALSE;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    if (ssd1306_command(SSD1306_G(display), SSD1306_SET_VERTICAL_SCROLL_AREA) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||
        ssd1306_command(SSD1306_G(display), SSD1306_G(display)->height) != 0 ||
        ssd1306_command(SSD1306_G(display), SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||
        ssd1306_command(SSD1306_G(display), start) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||
        ssd1306_command(SSD1306_G(display), stop) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x01) != 0 ||
        ssd1306_command(SSD1306_G(display), SSD1306_ACTIVATE_SCROLL) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ssd1306_start_scroll_diag_left(int start, int stop)
   Start diagonal scroll left */
PHP_FUNCTION(ssd1306_start_scroll_diag_left)
{
    zend_long start, stop;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &start, &stop) == FAILURE) {
        RETURN_FALSE;
    }

    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    if (ssd1306_command(SSD1306_G(display), SSD1306_SET_VERTICAL_SCROLL_AREA) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||
        ssd1306_command(SSD1306_G(display), SSD1306_G(display)->height) != 0 ||
        ssd1306_command(SSD1306_G(display), SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||
        ssd1306_command(SSD1306_G(display), start) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x00) != 0 ||
        ssd1306_command(SSD1306_G(display), stop) != 0 ||
        ssd1306_command(SSD1306_G(display), 0x01) != 0 ||
        ssd1306_command(SSD1306_G(display), SSD1306_ACTIVATE_SCROLL) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool ssd1306_stop_scroll()
   Stop scrolling */
PHP_FUNCTION(ssd1306_stop_scroll)
{
    if (!SSD1306_G(display)) {
        php_error_docref(NULL, E_WARNING, "SSD1306 display not initialized");
        RETURN_FALSE;
    }

    if (ssd1306_command(SSD1306_G(display), SSD1306_DEACTIVATE_SCROLL) != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */
