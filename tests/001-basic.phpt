--TEST--
SSD1306 Basic functionality test
--SKIPIF--
<?php if (!extension_loaded('ssd1306')) print 'skip'; ?>
--FILE--
<?php
// Test constants
var_dump(defined('SSD1306_I2C_ADDRESS'));
var_dump(defined('SSD1306_BLACK'));
var_dump(defined('SSD1306_WHITE'));
var_dump(defined('SSD1306_LCDWIDTH_128'));
var_dump(defined('SSD1306_LCDHEIGHT_64'));

// Test function existence
var_dump(function_exists('ssd1306_begin'));
var_dump(function_exists('ssd1306_end'));
var_dump(function_exists('ssd1306_display'));
var_dump(function_exists('ssd1306_clear_display'));
var_dump(function_exists('ssd1306_draw_pixel'));
var_dump(function_exists('ssd1306_get_width'));
var_dump(function_exists('ssd1306_get_height'));

echo "Basic functionality test completed\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Basic functionality test completed
