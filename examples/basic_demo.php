<?php
/**
 * SSD1306 Basic Demo
 * 
 * This example demonstrates basic SSD1306 display operations:
 * - Initialize display
 * - Clear screen
 * - Draw pixels
 * - Update display
 */

// Check if extension is loaded
if (!extension_loaded('ssd1306')) {
    die("SSD1306 extension not loaded\n");
}

echo "SSD1306 Basic Demo\n";
echo "==================\n\n";

// Initialize display (I2C bus 1, default address, 128x64)
echo "Initializing display...\n";
if (!ssd1306_begin(1, SSD1306_I2C_ADDRESS, SSD1306_LCDWIDTH_128, SSD1306_LCDHEIGHT_64)) {
    die("Failed to initialize SSD1306 display\n");
}

echo "Display initialized successfully!\n";
echo "Width: " . ssd1306_get_width() . "\n";
echo "Height: " . ssd1306_get_height() . "\n\n";

// Clear display
echo "Clearing display...\n";
ssd1306_clear_display();
ssd1306_display();
sleep(1);

// Draw some pixels
echo "Drawing pixels...\n";
for ($i = 0; $i < 10; $i++) {
    $x = rand(0, ssd1306_get_width() - 1);
    $y = rand(0, ssd1306_get_height() - 1);
    ssd1306_draw_pixel($x, $y, SSD1306_WHITE);
    echo "Pixel at ($x, $y)\n";
}

// Update display
ssd1306_display();
echo "Display updated!\n\n";

sleep(2);

// Test contrast
echo "Testing contrast...\n";
ssd1306_set_contrast(50);
sleep(1);
ssd1306_set_contrast(255);
sleep(1);

// Test invert
echo "Testing invert display...\n";
ssd1306_invert_display(true);
sleep(1);
ssd1306_invert_display(false);
sleep(1);

// Cleanup
echo "Cleaning up...\n";
ssd1306_clear_display();
ssd1306_display();
ssd1306_end();

echo "Demo completed!\n";
?>
