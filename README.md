# PHP SSD1306 Extension

## 🎉 **Official Release v1.0.0 Available!**

A comprehensive PHP extension for controlling SSD1306 OLED displays via I2C communication, specifically designed for **NVIDIA Jetson Orin devices** compatible with the [Yahboom Jetson Nano CUBE case](https://www.yahboom.net/study/CUBE_NANO) form factor. This extension provides a complete API for display initialization, pixel manipulation, graphics drawing, text rendering, and scrolling effects.

**✨ Version 1.0.0** includes precompiled binaries and optimized installation for **NVIDIA Jetson Orin Nano** running **Ubuntu 22.04.5 LTS** with **PHP 8.4.12**.

## Features

- **I2C Communication**: Direct hardware communication with SSD1306 displays
- **Multiple Display Sizes**: Support for 128x64, 128x32, and 96x16 displays
- **Graphics Primitives**: Lines, rectangles, circles (filled and outlined)
- **Text Rendering**: Built-in font with scalable text sizes
- **Scrolling Effects**: Horizontal and diagonal scrolling animations
- **Pixel-Level Control**: Individual pixel manipulation
- **Hardware Abstraction**: Easy integration with embedded projects

## Requirements

- **PHP 8.0+** with development headers (`php-dev`)
- **NVIDIA Jetson Orin** or compatible Linux system with I2C support
- **Yahboom CUBE case** or compatible SSD1306 OLED display setup
- **I2C Tools** (`sudo apt install i2c-tools libi2c-dev`)
- **Build Tools** (`sudo apt install build-essential autoconf`)

### ⚠️ Important: Remove Herd Lite (Ubuntu/JetPack)

On fresh Ubuntu/JetPack installations, Composer may install Herd Lite which conflicts with PHP extensions. **Remove it before installation:**

```bash
# Remove Herd Lite completely
rm -rf ~/.config/herd-lite

# Install proper PHP-FPM insteady
sudo apt update
sudo apt install php8.4-fpm php8.4-cli php8.4-dev php8.4-common \
                 php8.4-curl php8.4-mbstring php8.4-xml php8.4-zip \
                 php8.4-sqlite3 php8.4-mysql

# Verify you're using system PHP
which php  # Should show /usr/bin/php, not herd-lite path
php --version  # Should show PHP 8.4.x without warnings
```

## 🚀 Quick Installation (v1.0.0)

### One-Command Installation for Jetson Orin Nano

For **NVIDIA Jetson Orin Nano** with **Ubuntu 22.04.5 LTS**:

```bash
# Check system compatibility first (optional)
curl -fsSL https://github.com/projectsaturnstudios/adafruit_ssd1306-php/raw/v1.0.0/validate-system.sh | bash

# Install SSD1306 extension (recommended)
curl -fsSL https://github.com/projectsaturnstudios/adafruit_ssd1306-php/raw/v1.0.0/install-jetson-orin.sh | bash
```

The installation script will:
- ✅ Install PHP 8.4.x if needed
- ✅ Install all build dependencies
- ✅ Configure I2C permissions
- ✅ Build and install the extension
- ✅ Create hardware test script
- ✅ Verify installation

### Manual Installation

```bash
# Install dependencies
sudo apt update
sudo apt install -y php8.4-dev i2c-tools libi2c-dev build-essential

# Clone and build
git clone https://github.com/projectsaturnstudios/adafruit_ssd1306-php.git
cd adafruit_ssd1306-php
git checkout v1.0.0
phpize
./configure --enable-ssd1306
make && sudo make install

# Configure PHP
echo "extension=ssd1306" | sudo tee /etc/php/8.4/mods-available/ssd1306.ini
sudo phpenmod ssd1306
```

## Installation

### Via PECL (Recommended)

```bash
sudo pecl install ssd1306
```

Add to your php.ini:
```ini
extension=ssd1306
```

### Manual Installation

```bash
git clone https://github.com/projectsaturnstudios/php-ssd1306.git
cd php-ssd1306
phpize
./configure --enable-ssd1306
make
sudo make install
```

## Hardware Setup

### Yahboom CUBE Case (Recommended)

This extension is specifically designed for the [Yahboom Jetson Nano CUBE case](https://www.yahboom.net/study/CUBE_NANO) which includes:
- Built-in SSD1306 OLED display (128x64)
- Pre-wired I2C connections on **bus 7**
- RGB LED strip and cooling fan
- Perfect form factor for Jetson Orin Nano/NX

> **🔧 Developer Note**: The Yahboom CUBE case uses **I2C bus 7**, not the commonly assumed bus 1. This was discovered during development when the original Python examples (which default to bus 1) failed to communicate with the display. Always verify your I2C bus with `i2cdetect -l` and test with `i2cdetect -y 7` to confirm the SSD1306 is detected at address 0x3C.

### Manual SSD1306 Wiring

If using a standalone SSD1306 display:

- **VCC**: 3.3V or 5V (depending on your display)
- **GND**: Ground
- **SCL**: I2C Clock (GPIO 3 on Jetson/Pi)
- **SDA**: I2C Data (GPIO 2 on Jetson/Pi)

Enable I2C on your system:
```bash
# On Raspberry Pi
sudo raspi-config
# Navigate to: Interfacing Options > I2C > Enable

# On Jetson devices (I2C is usually enabled by default)
# Add user to i2c group
sudo usermod -a -G i2c $USER

# Verify I2C is working
# For Yahboom CUBE case (bus 7):
sudo i2cdetect -y 7

# For other setups (usually bus 1):
sudo i2cdetect -y 1
```

## Quick Start

```php
<?php
// Initialize display (I2C bus 7 for Yahboom CUBE, address 0x3C)
if (!ssd1306_begin(7, 0x3C)) {
    die("Failed to initialize display\n");
}

// Clear screen
ssd1306_clear_display();

// Draw some graphics
ssd1306_draw_rect(10, 10, 50, 30, SSD1306_WHITE);
ssd1306_fill_circle(80, 25, 15, SSD1306_WHITE);

// Add text
ssd1306_set_cursor(0, 50);
ssd1306_set_text_size(2);
ssd1306_print("Hello PHP!");

// Update display
ssd1306_display();

// Cleanup
ssd1306_end();
?>
```

## API Reference

### Display Management

```php
// Initialize display
bool ssd1306_begin([int $i2c_bus = 7, int $i2c_addr = 0x3C])

// Cleanup display
void ssd1306_end()

// Update display with buffer contents
bool ssd1306_display()

// Clear display buffer
void ssd1306_clear_display()

// Invert display colors
bool ssd1306_invert_display(bool $invert)

// Dim display
bool ssd1306_dim(bool $dim)

// Set display contrast (0-255)
bool ssd1306_set_contrast(int $contrast)
```

### Graphics Functions

```php
// Draw single pixel
void ssd1306_draw_pixel(int $x, int $y, int $color)

// Draw line
void ssd1306_draw_line(int $x0, int $y0, int $x1, int $y1, int $color)

// Draw rectangle outline
void ssd1306_draw_rect(int $x, int $y, int $w, int $h, int $color)

// Draw filled rectangle
void ssd1306_fill_rect(int $x, int $y, int $w, int $h, int $color)

// Draw circle outline
void ssd1306_draw_circle(int $x0, int $y0, int $r, int $color)

// Draw filled circle
void ssd1306_fill_circle(int $x0, int $y0, int $r, int $color)

// Get pixel color
int ssd1306_get_pixel(int $x, int $y)
```

### Text Functions

```php
// Print text at cursor position
void ssd1306_print(string $text)

// Set text cursor position
void ssd1306_set_cursor(int $x, int $y)

// Set text size multiplier
void ssd1306_set_text_size(int $size)

// Set text color
void ssd1306_set_text_color(int $color)
```

### Display Information

```php
// Get display width
int ssd1306_get_width()

// Get display height
int ssd1306_get_height()
```

### Scrolling Effects

```php
// Start horizontal scroll right
bool ssd1306_start_scroll_right(int $start, int $stop)

// Start horizontal scroll left
bool ssd1306_start_scroll_left(int $start, int $stop)

// Start diagonal scroll right
bool ssd1306_start_scroll_diag_right(int $start, int $stop)

// Start diagonal scroll left
bool ssd1306_start_scroll_diag_left(int $start, int $stop)

// Stop scrolling
bool ssd1306_stop_scroll()
```

## Constants

### I2C Addresses
- `SSD1306_I2C_ADDRESS` (0x3C) - Default I2C address
- `SSD1306_I2C_ADDRESS_ALT` (0x3D) - Alternative I2C address

### Display Sizes
- `SSD1306_LCDWIDTH_128` (128) - 128 pixel width
- `SSD1306_LCDHEIGHT_64` (64) - 64 pixel height
- `SSD1306_LCDHEIGHT_32` (32) - 32 pixel height
- `SSD1306_LCDWIDTH_96` (96) - 96 pixel width
- `SSD1306_LCDHEIGHT_16` (16) - 16 pixel height

### Colors
- `SSD1306_BLACK` (0) - Black/off pixel
- `SSD1306_WHITE` (1) - White/on pixel
- `SSD1306_INVERSE` (2) - Invert pixel

### VCC States
- `SSD1306_EXTERNALVCC` (1) - External VCC supply
- `SSD1306_SWITCHCAPVCC` (2) - Internal charge pump (default)

## Examples

See the `examples/` directory for complete demonstrations:

- `basic_demo.php` - Basic display operations
- `graphics_demo.php` - Graphics and animation
- `text_demo.php` - Text rendering and effects
- `scroll_demo.php` - Scrolling animations

## Testing

### Hardware Test Script

After installation, test your hardware:

```bash
# Test with Yahboom CUBE (bus 7, address 0x3C)
php /tmp/ssd1306_test.php

# Test with custom I2C configuration
php /tmp/ssd1306_test.php 1 0x3D
```

### Development Test Suite

Run the development test suite:

```bash
make test
```

## Troubleshooting

### Display not working
1. **Check I2C bus number first**:
   ```bash
   # List all I2C buses
   i2cdetect -l
   
   # For Yahboom CUBE case, check bus 7:
   sudo i2cdetect -y 7
   
   # For other setups, try bus 1:
   sudo i2cdetect -y 1
   
   # Look for device at 0x3C or 0x3D
   ```

2. **Verify I2C connections and power**
3. **Try alternative I2C address**: Use `0x3D` instead of `0x3C`
4. **Check display power supply** (3.3V or 5V depending on module)

### Wrong I2C Bus Error
If you get "Failed to initialize display", you're likely using the wrong I2C bus:
- **Yahboom CUBE**: Use bus 7 (`ssd1306_begin(7, 0x3C)`)
- **Raspberry Pi**: Usually bus 1 (`ssd1306_begin(1, 0x3C)`)
- **Other Jetson**: Check with `i2cdetect -l`

### 🤖 AI-Assisted I2C Detection
If manual I2C detection is challenging, consider using an AI coding agent like **Cursor** or **Claude Dev** to help:

1. **Install Cursor IDE** or similar AI-powered editor
2. **Share your I2C scan results**:
   ```bash
   i2cdetect -l > i2c_buses.txt
   for i in {0..7}; do echo "=== Bus $i ===" >> i2c_scan.txt; sudo i2cdetect -y $i >> i2c_scan.txt 2>/dev/null; done
   ```
3. **Ask the AI agent** to analyze the output and generate a Python detection script
4. **Use the generated script** to automatically find your SSD1306 display

Example AI prompt: *"Analyze this i2cdetect output and create a Python script to automatically detect which I2C bus has an SSD1306 display at address 0x3C or 0x3D"*

This approach is especially helpful for complex hardware setups or when working with unfamiliar Jetson configurations.

### Permission denied
```bash
# Add user to i2c group
sudo usermod -a -G i2c $USER
# Logout and login again
```

### Extension not loading
1. Check php.ini has `extension=ssd1306`
2. Verify extension is installed: `php -m | grep ssd1306`
3. Check PHP error logs

## License

This extension is licensed under the PHP License. See LICENSE file for details.

## Contributing

Contributions are welcome! Please submit pull requests or issues on GitHub.

## Author

Project Saturn Studios, LLC  
Email: info@projectsaturnstudios.com

## Acknowledgments

Based on the Adafruit SSD1306 library for Arduino. Special thanks to the Adafruit team for their excellent hardware documentation and reference implementation.
