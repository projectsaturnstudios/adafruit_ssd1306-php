--TEST--
SSD1306 Graphics functions test
--SKIPIF--
<?php if (!extension_loaded('ssd1306')) print 'skip'; ?>
--FILE--
<?php
// Test graphics function existence
var_dump(function_exists('ssd1306_draw_line'));
var_dump(function_exists('ssd1306_draw_rect'));
var_dump(function_exists('ssd1306_fill_rect'));
var_dump(function_exists('ssd1306_draw_circle'));
var_dump(function_exists('ssd1306_fill_circle'));

echo "Graphics functions test completed\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Graphics functions test completed
