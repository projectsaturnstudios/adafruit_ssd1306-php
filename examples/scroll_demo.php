<?php
/**
 * SSD1306 Scrolling Demo
 * 
 * This example demonstrates the scrolling capabilities of the SSD1306 OLED display.
 */

// Initialize the display
if (!ssd1306_begin()) {
    die("Failed to initialize SSD1306 display\n");
}

echo "SSD1306 Scrolling Demo\n";

// Clear the display
ssd1306_clear_display();

// Draw some text
ssd1306_draw_char(0, 0, 'H');
ssd1306_draw_char(8, 0, 'e');
ssd1306_draw_char(16, 0, 'l');
ssd1306_draw_char(24, 0, 'l');
ssd1306_draw_char(32, 0, 'o');

// Update the display
ssd1306_display();

echo "Starting horizontal scroll right...\n";
sleep(2);

// Start scrolling right
ssd1306_start_scroll_right(0x00, 0x07);
sleep(3);

// Stop scrolling
ssd1306_stop_scroll();
sleep(1);

echo "Starting horizontal scroll left...\n";
// Start scrolling left
ssd1306_start_scroll_left(0x00, 0x07);
sleep(3);

// Stop scrolling
ssd1306_stop_scroll();
sleep(1);

echo "Starting diagonal scroll right...\n";
// Start diagonal scroll right
ssd1306_start_scroll_diag_right(0x00, 0x07);
sleep(3);

// Stop scrolling
ssd1306_stop_scroll();
sleep(1);

echo "Starting diagonal scroll left...\n";
// Start diagonal scroll left
ssd1306_start_scroll_diag_left(0x00, 0x07);
sleep(3);

// Stop scrolling
ssd1306_stop_scroll();

echo "Demo completed!\n";
?>
