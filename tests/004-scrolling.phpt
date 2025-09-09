--TEST--
SSD1306 Scrolling functions test
--SKIPIF--
<?php if (!extension_loaded('ssd1306')) print 'skip'; ?>
--FILE--
<?php
// Test scrolling function existence
var_dump(function_exists('ssd1306_start_scroll_right'));
var_dump(function_exists('ssd1306_start_scroll_left'));
var_dump(function_exists('ssd1306_start_scroll_diag_right'));
var_dump(function_exists('ssd1306_start_scroll_diag_left'));
var_dump(function_exists('ssd1306_stop_scroll'));

echo "Scrolling functions test completed\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Scrolling functions test completed
