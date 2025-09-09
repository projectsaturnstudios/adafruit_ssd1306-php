/*
  +----------------------------------------------------------------------+
  | PHP SSD1306 Extension - Display Functions                           |
  +----------------------------------------------------------------------+
  | Copyright (c) Project Saturn Studios, LLC                           |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ssd1306.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <stdlib.h>

/* Initialize SSD1306 display */
int ssd1306_init(ssd1306_t *display, int i2c_bus, int i2c_addr, int width, int height, int vcc_state)
{
    char i2c_device[32];
    
    /* Set display parameters */
    display->width = width;
    display->height = height;
    display->pages = height / 8;
    display->buffer_size = width * display->pages;
    display->i2c_addr = i2c_addr;
    display->vcc_state = vcc_state;
    display->contrast = (vcc_state == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
    display->rotation = 0;
    display->cursor_x = 0;
    display->cursor_y = 0;
    display->text_size = 1;
    display->text_color = SSD1306_WHITE;
    display->text_bg_color = SSD1306_BLACK;
    display->wrap = 1;

    /* Allocate display buffer */
    display->buffer = malloc(display->buffer_size);
    if (!display->buffer) {
        return -1;
    }
    memset(display->buffer, 0, display->buffer_size);

    /* Open I2C device */
    snprintf(i2c_device, sizeof(i2c_device), "/dev/i2c-%d", i2c_bus);
    display->i2c_fd = open(i2c_device, O_RDWR);
    if (display->i2c_fd < 0) {
        free(display->buffer);
        return -1;
    }

    /* Set I2C slave address */
    if (ioctl(display->i2c_fd, I2C_SLAVE, i2c_addr) < 0) {
        close(display->i2c_fd);
        free(display->buffer);
        return -1;
    }

    /* Initialize display with proper sequence */
    if (ssd1306_init_sequence(display) != 0) {
        close(display->i2c_fd);
        free(display->buffer);
        return -1;
    }

    return 0;
}

/* Send initialization sequence to display */
int ssd1306_init_sequence(ssd1306_t *display)
{
    /* Display off */
    if (ssd1306_command(display, SSD1306_DISPLAYOFF) != 0) return -1;

    /* Set display clock divide ratio/oscillator frequency */
    if (ssd1306_command(display, SSD1306_SETDISPLAYCLOCKDIV) != 0) return -1;
    if (ssd1306_command(display, 0x80) != 0) return -1;  /* Default ratio */

    /* Set multiplex ratio */
    if (ssd1306_command(display, SSD1306_SETMULTIPLEX) != 0) return -1;
    if (ssd1306_command(display, display->height - 1) != 0) return -1;

    /* Set display offset */
    if (ssd1306_command(display, SSD1306_SETDISPLAYOFFSET) != 0) return -1;
    if (ssd1306_command(display, 0x0) != 0) return -1;  /* No offset */

    /* Set start line address */
    if (ssd1306_command(display, SSD1306_SETSTARTLINE | 0x0) != 0) return -1;

    /* Charge pump */
    if (ssd1306_command(display, SSD1306_CHARGEPUMP) != 0) return -1;
    if (display->vcc_state == SSD1306_EXTERNALVCC) {
        if (ssd1306_command(display, 0x10) != 0) return -1;
    } else {
        if (ssd1306_command(display, 0x14) != 0) return -1;
    }

    /* Memory mode */
    if (ssd1306_command(display, SSD1306_MEMORYMODE) != 0) return -1;
    if (ssd1306_command(display, 0x00) != 0) return -1;  /* Horizontal addressing mode */

    /* Set segment re-map */
    if (ssd1306_command(display, SSD1306_SEGREMAP | 0x1) != 0) return -1;

    /* Set COM output scan direction */
    if (ssd1306_command(display, SSD1306_COMSCANDEC) != 0) return -1;

    /* Set COM pins hardware configuration */
    if (ssd1306_command(display, SSD1306_SETCOMPINS) != 0) return -1;
    if (display->height == 32) {
        if (ssd1306_command(display, 0x02) != 0) return -1;
    } else if (display->height == 16) {
        if (ssd1306_command(display, 0x02) != 0) return -1;
    } else {
        if (ssd1306_command(display, 0x12) != 0) return -1;
    }

    /* Set contrast control */
    if (ssd1306_command(display, SSD1306_SETCONTRAST) != 0) return -1;
    if (ssd1306_command(display, display->contrast) != 0) return -1;

    /* Set pre-charge period */
    if (ssd1306_command(display, SSD1306_SETPRECHARGE) != 0) return -1;
    if (display->vcc_state == SSD1306_EXTERNALVCC) {
        if (ssd1306_command(display, 0x22) != 0) return -1;
    } else {
        if (ssd1306_command(display, 0xF1) != 0) return -1;
    }

    /* Set VCOMH deselect level */
    if (ssd1306_command(display, SSD1306_SETVCOMDETECT) != 0) return -1;
    if (ssd1306_command(display, 0x40) != 0) return -1;

    /* Entire display on (resume to RAM content display) */
    if (ssd1306_command(display, SSD1306_DISPLAYALLON_RESUME) != 0) return -1;

    /* Set normal display */
    if (ssd1306_command(display, SSD1306_NORMALDISPLAY) != 0) return -1;

    /* Deactivate scroll */
    if (ssd1306_command(display, SSD1306_DEACTIVATE_SCROLL) != 0) return -1;

    /* Display on */
    if (ssd1306_command(display, SSD1306_DISPLAYON) != 0) return -1;

    return 0;
}

/* Send command to display */
int ssd1306_command(ssd1306_t *display, unsigned char cmd)
{
    unsigned char buffer[2];
    buffer[0] = 0x00;  /* Command mode */
    buffer[1] = cmd;

    if (write(display->i2c_fd, buffer, 2) != 2) {
        return -1;
    }

    return 0;
}

/* Send data to display */
int ssd1306_data(ssd1306_t *display, unsigned char *data, int len)
{
    unsigned char *buffer = malloc(len + 1);
    if (!buffer) {
        return -1;
    }

    buffer[0] = 0x40;  /* Data mode */
    memcpy(buffer + 1, data, len);

    int result = (write(display->i2c_fd, buffer, len + 1) == (len + 1)) ? 0 : -1;
    free(buffer);

    return result;
}

/* Update display with buffer contents */
int ssd1306_update_display(ssd1306_t *display)
{
    /* Set column address range */
    if (ssd1306_command(display, SSD1306_COLUMNADDR) != 0) return -1;
    if (ssd1306_command(display, 0) != 0) return -1;  /* Column start address */
    if (ssd1306_command(display, display->width - 1) != 0) return -1;  /* Column end address */

    /* Set page address range */
    if (ssd1306_command(display, SSD1306_PAGEADDR) != 0) return -1;
    if (ssd1306_command(display, 0) != 0) return -1;  /* Page start address */
    if (ssd1306_command(display, display->pages - 1) != 0) return -1;  /* Page end address */

    /* Send buffer data */
    return ssd1306_data(display, display->buffer, display->buffer_size);
}

/* Set pixel in buffer */
void ssd1306_set_pixel_internal(ssd1306_t *display, int x, int y, int color)
{
    if (x < 0 || x >= display->width || y < 0 || y >= display->height) {
        return;
    }

    int page = y / 8;
    int bit = y % 8;
    int index = x + (page * display->width);

    switch (color) {
        case SSD1306_WHITE:
            display->buffer[index] |= (1 << bit);
            break;
        case SSD1306_BLACK:
            display->buffer[index] &= ~(1 << bit);
            break;
        case SSD1306_INVERSE:
            display->buffer[index] ^= (1 << bit);
            break;
    }
}

/* Get pixel from buffer */
int ssd1306_get_pixel_internal(ssd1306_t *display, int x, int y)
{
    if (x < 0 || x >= display->width || y < 0 || y >= display->height) {
        return 0;
    }

    int page = y / 8;
    int bit = y % 8;
    int index = x + (page * display->width);

    return (display->buffer[index] & (1 << bit)) ? SSD1306_WHITE : SSD1306_BLACK;
}

/* Cleanup display resources */
void ssd1306_cleanup(ssd1306_t *display)
{
    if (display) {
        if (display->i2c_fd >= 0) {
            /* Turn off display before closing */
            ssd1306_command(display, SSD1306_DISPLAYOFF);
            close(display->i2c_fd);
        }
        if (display->buffer) {
            free(display->buffer);
        }
    }
}
