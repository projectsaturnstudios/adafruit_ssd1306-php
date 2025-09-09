<?php
/**
 * SSD1306 Text Demo
 * 
 * This example demonstrates text rendering capabilities:
 * - Text printing
 * - Cursor positioning
 * - Text size scaling
 * - Text colors
 */

// Check if extension is loaded
if (!extension_loaded('ssd1306')) {
    die("SSD1306 extension not loaded\n");
}

echo "SSD1306 Text Demo\n";
echo "==================\n\n";

// Initialize display
if (!ssd1306_begin()) {
    die("Failed to initialize SSD1306 display\n");
}

echo "Demonstrating text rendering...\n\n";

// Basic text
echo "Basic text...\n";
ssd1306_clear_display();
ssd1306_set_cursor(0, 0);
ssd1306_set_text_size(1);
ssd1306_set_text_color(SSD1306_WHITE);
ssd1306_print("Hello, World!");
ssd1306_display();
sleep(2);

// Different text sizes
echo "Different text sizes...\n";
ssd1306_clear_display();

ssd1306_set_cursor(0, 0);
ssd1306_set_text_size(1);
ssd1306_print("Size 1");

ssd1306_set_cursor(0, 16);
ssd1306_set_text_size(2);
ssd1306_print("Size 2");

ssd1306_set_cursor(0, 40);
ssd1306_set_text_size(3);
ssd1306_print("Big!");

ssd1306_display();
sleep(3);

// Scrolling text
echo "Scrolling text...\n";
ssd1306_clear_display();
ssd1306_set_text_size(1);

$messages = [
    "PHP SSD1306 Extension",
    "By Project Saturn Studios",
    "Supports I2C displays",
    "Graphics & Text rendering",
    "Scrolling effects",
    "Multiple display sizes"
];

foreach ($messages as $i => $message) {
    ssd1306_set_cursor(0, $i * 10);
    ssd1306_print($message);
}

ssd1306_display();
sleep(2);

// Start horizontal scroll
echo "Starting horizontal scroll...\n";
ssd1306_start_scroll_right(0, 7);
sleep(3);
ssd1306_stop_scroll();

// Typewriter effect
echo "Typewriter effect...\n";
ssd1306_clear_display();
ssd1306_set_cursor(0, 0);
ssd1306_set_text_size(2);

$text = "PHP ROCKS!";
for ($i = 0; $i < strlen($text); $i++) {
    ssd1306_print($text[$i]);
    ssd1306_display();
    usleep(200000); // 200ms delay
}

sleep(2);

// Text positioning demo
echo "Text positioning demo...\n";
ssd1306_clear_display();
ssd1306_set_text_size(1);

// Corner text
ssd1306_set_cursor(0, 0);
ssd1306_print("TL");

ssd1306_set_cursor(ssd1306_get_width() - 12, 0);
ssd1306_print("TR");

ssd1306_set_cursor(0, ssd1306_get_height() - 8);
ssd1306_print("BL");

ssd1306_set_cursor(ssd1306_get_width() - 12, ssd1306_get_height() - 8);
ssd1306_print("BR");

// Center text
$centerX = (ssd1306_get_width() - 6 * 6) / 2; // 6 chars * 6 pixels wide
$centerY = (ssd1306_get_height() - 8) / 2;    // 8 pixels high
ssd1306_set_cursor($centerX, $centerY);
ssd1306_print("CENTER");

ssd1306_display();
sleep(3);

// Cleanup
ssd1306_clear_display();
ssd1306_display();
ssd1306_end();

echo "Text demo completed!\n";
?>
