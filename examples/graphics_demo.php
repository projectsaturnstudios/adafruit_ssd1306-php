<?php
/**
 * SSD1306 Graphics Demo
 * 
 * This example demonstrates graphics drawing functions:
 * - Lines, rectangles, circles
 * - Filled shapes
 * - Animation effects
 */

// Check if extension is loaded
if (!extension_loaded('ssd1306')) {
    die("SSD1306 extension not loaded\n");
}

echo "SSD1306 Graphics Demo\n";
echo "=====================\n\n";

// Initialize display
if (!ssd1306_begin()) {
    die("Failed to initialize SSD1306 display\n");
}

$width = ssd1306_get_width();
$height = ssd1306_get_height();

echo "Drawing graphics on {$width}x{$height} display...\n\n";

// Draw lines
echo "Drawing lines...\n";
ssd1306_clear_display();
for ($i = 0; $i < $width; $i += 10) {
    ssd1306_draw_line(0, 0, $i, $height - 1, SSD1306_WHITE);
    ssd1306_draw_line($width - 1, 0, $width - 1 - $i, $height - 1, SSD1306_WHITE);
}
ssd1306_display();
sleep(2);

// Draw rectangles
echo "Drawing rectangles...\n";
ssd1306_clear_display();
for ($i = 0; $i < min($width, $height) / 2; $i += 8) {
    ssd1306_draw_rect($i, $i, $width - 2 * $i, $height - 2 * $i, SSD1306_WHITE);
}
ssd1306_display();
sleep(2);

// Draw filled rectangles
echo "Drawing filled rectangles...\n";
ssd1306_clear_display();
ssd1306_fill_rect(10, 10, 20, 15, SSD1306_WHITE);
ssd1306_fill_rect(40, 10, 20, 15, SSD1306_WHITE);
ssd1306_fill_rect(70, 10, 20, 15, SSD1306_WHITE);
ssd1306_display();
sleep(2);

// Draw circles
echo "Drawing circles...\n";
ssd1306_clear_display();
$centerX = $width / 2;
$centerY = $height / 2;
for ($r = 5; $r < min($width, $height) / 2; $r += 5) {
    ssd1306_draw_circle($centerX, $centerY, $r, SSD1306_WHITE);
}
ssd1306_display();
sleep(2);

// Draw filled circle
echo "Drawing filled circle...\n";
ssd1306_clear_display();
ssd1306_fill_circle($centerX, $centerY, 20, SSD1306_WHITE);
ssd1306_display();
sleep(2);

// Animation: bouncing ball
echo "Bouncing ball animation...\n";
$ballX = 10;
$ballY = 10;
$ballDX = 2;
$ballDY = 1;
$ballR = 3;

for ($frame = 0; $frame < 100; $frame++) {
    ssd1306_clear_display();
    
    // Update ball position
    $ballX += $ballDX;
    $ballY += $ballDY;
    
    // Bounce off walls
    if ($ballX - $ballR <= 0 || $ballX + $ballR >= $width) {
        $ballDX = -$ballDX;
    }
    if ($ballY - $ballR <= 0 || $ballY + $ballR >= $height) {
        $ballDY = -$ballDY;
    }
    
    // Draw ball
    ssd1306_fill_circle($ballX, $ballY, $ballR, SSD1306_WHITE);
    
    // Draw borders
    ssd1306_draw_rect(0, 0, $width, $height, SSD1306_WHITE);
    
    ssd1306_display();
    usleep(50000); // 50ms delay
}

// Cleanup
ssd1306_clear_display();
ssd1306_display();
ssd1306_end();

echo "Graphics demo completed!\n";
?>
