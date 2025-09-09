--TEST--
SSD1306 Text functions test
--SKIPIF--
<?php if (!extension_loaded('ssd1306')) print 'skip'; ?>
--FILE--
<?php
// Test text function existence
var_dump(function_exists('ssd1306_print'));
var_dump(function_exists('ssd1306_set_cursor'));
var_dump(function_exists('ssd1306_set_text_size'));
var_dump(function_exists('ssd1306_set_text_color'));

echo "Text functions test completed\n";
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
Text functions test completed
