PHP_ARG_ENABLE([ssd1306],
  [whether to enable SSD1306 OLED display support],
  [AS_HELP_STRING([--enable-ssd1306],
    [Enable SSD1306 OLED display support])],
  [no])

if test "$PHP_SSD1306" != "no"; then
  dnl Check for required headers
  AC_CHECK_HEADERS([fcntl.h unistd.h sys/ioctl.h linux/i2c-dev.h], [], [
    AC_MSG_ERROR([Required headers not found])
  ])

  dnl Define the extension
  PHP_NEW_EXTENSION(ssd1306, 
    ssd1306.c \
    ssd1306_display.c \
    ssd1306_graphics.c,
    $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  
  dnl Add compiler flags
  PHP_ADD_BUILD_DIR($ext_builddir)
  
  dnl Check for I2C support
  AC_MSG_CHECKING([for I2C support])
  AC_TRY_COMPILE([
    #include <linux/i2c-dev.h>
  ], [
    int dummy = I2C_SLAVE;
  ], [
    AC_MSG_RESULT([yes])
  ], [
    AC_MSG_ERROR([I2C support not found. Please install i2c-tools and libi2c-dev])
  ])
fi
