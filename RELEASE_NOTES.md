# SSD1306 PHP Extension - Release Notes v1.0.0

## 🎉 First Official Release!

We're excited to announce the **first official stable release** of the SSD1306 PHP Extension v1.0.0! This release provides a comprehensive PHP interface for controlling SSD1306 OLED displays via I2C communication, specifically optimized for **NVIDIA Jetson Orin Nano** devices running Ubuntu 22.04.5 LTS with PHP 8.4.x.

## 🎯 Target Platform

- **Hardware**: NVIDIA Jetson Orin Nano (ARM64/aarch64)
- **Operating System**: Ubuntu 22.04.5 LTS
- **PHP Version**: 8.4.12 (compatible with PHP 8.0+)
- **Recommended Setup**: [Yahboom Jetson Nano CUBE case](https://www.yahboom.net/study/CUBE_NANO)

## ✨ What's New in v1.0.0

### Complete SSD1306 Support
- **Hardware Communication**: Direct I2C communication with SSD1306 displays
- **Multiple Display Sizes**: Support for 128x64, 128x32, and 96x16 displays
- **Hardware-Tested**: Verified on real hardware with Yahboom CUBE case

### Graphics & Display Features
- **Pixel Operations**: Individual pixel control with `ssd1306_draw_pixel()`
- **Graphics Primitives**: Lines, rectangles, circles (both outlined and filled)
- **Text Rendering**: Built-in ASCII font with scalable text sizes (1x to 8x)
- **Display Management**: Clear, invert, dim, contrast control
- **Buffer Management**: Efficient display buffer with explicit update control

### Advanced Features
- **Scrolling Effects**: Horizontal and diagonal scrolling animations
- **Color Support**: BLACK, WHITE, and INVERSE pixel modes
- **I2C Flexibility**: Configurable I2C bus and device address
- **Error Handling**: Comprehensive error checking and resource cleanup

### Developer Experience
- **Easy Installation**: One-command installation script for Jetson Orin
- **Comprehensive Documentation**: Complete API reference and examples
- **Test Suite**: 4 comprehensive test cases covering all functionality
- **Hardware Validation**: Built-in hardware test script

## 🚀 Quick Installation

### For NVIDIA Jetson Orin Nano (Recommended)

```bash
# Download and run the installation script
curl -fsSL https://github.com/projectsaturnstudios/adafruit_ssd1306-php/raw/v1.0.0/install-jetson-orin.sh | bash
```

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

## 📝 Quick Start Example

```php
<?php
// Initialize display (I2C bus 7, address 0x3C for Yahboom CUBE)
if (!ssd1306_begin(7, 0x3C)) {
    die("Failed to initialize display\n");
}

// Clear screen and draw some graphics
ssd1306_clear_display();
ssd1306_draw_rect(10, 10, 50, 30, SSD1306_WHITE);
ssd1306_fill_circle(80, 25, 15, SSD1306_WHITE);

// Add text
ssd1306_set_cursor(0, 50);
ssd1306_set_text_size(2);
ssd1306_print("Hello PHP!");

// Update display and cleanup
ssd1306_display();
ssd1306_end();
?>
```

## 🔧 Hardware Configuration

### Yahboom CUBE Case (Recommended)
The extension is optimized for the Yahboom Jetson Nano CUBE case, which includes:
- Built-in 128x64 SSD1306 OLED display
- Pre-wired I2C connections on **bus 7** (not the common bus 1!)
- Plug-and-play setup with Jetson Orin Nano

### Manual SSD1306 Wiring
For standalone displays:
- **VCC**: 3.3V or 5V (depending on display)
- **GND**: Ground
- **SCL**: I2C Clock (GPIO 3)
- **SDA**: I2C Data (GPIO 2)

### I2C Bus Detection
```bash
# List available I2C buses
i2cdetect -l

# Scan for devices (Yahboom CUBE uses bus 7)
i2cdetect -y 7

# Common alternative (many tutorials use bus 1)
i2cdetect -y 1
```

## 📚 Complete API Reference

### Display Management
| Function | Description |
|----------|-------------|
| `ssd1306_begin(bus, addr)` | Initialize display |
| `ssd1306_end()` | Cleanup and release resources |
| `ssd1306_display()` | Update physical display with buffer |
| `ssd1306_clear_display()` | Clear display buffer |
| `ssd1306_invert_display(bool)` | Invert display colors |
| `ssd1306_dim(bool)` | Dim display brightness |
| `ssd1306_set_contrast(0-255)` | Set display contrast |

### Graphics Functions
| Function | Description |
|----------|-------------|
| `ssd1306_draw_pixel(x, y, color)` | Draw single pixel |
| `ssd1306_draw_line(x0, y0, x1, y1, color)` | Draw line |
| `ssd1306_draw_rect(x, y, w, h, color)` | Draw rectangle outline |
| `ssd1306_fill_rect(x, y, w, h, color)` | Draw filled rectangle |
| `ssd1306_draw_circle(x, y, r, color)` | Draw circle outline |
| `ssd1306_fill_circle(x, y, r, color)` | Draw filled circle |
| `ssd1306_get_pixel(x, y)` | Get pixel color |

### Text Functions
| Function | Description |
|----------|-------------|
| `ssd1306_print(text)` | Print text at cursor |
| `ssd1306_set_cursor(x, y)` | Set text cursor position |
| `ssd1306_set_text_size(1-8)` | Set text size multiplier |
| `ssd1306_set_text_color(color)` | Set text color |

### Display Information
| Function | Description |
|----------|-------------|
| `ssd1306_get_width()` | Get display width in pixels |
| `ssd1306_get_height()` | Get display height in pixels |

### Scrolling Effects
| Function | Description |
|----------|-------------|
| `ssd1306_start_scroll_right(start, stop)` | Start horizontal scroll right |
| `ssd1306_start_scroll_left(start, stop)` | Start horizontal scroll left |
| `ssd1306_start_scroll_diag_right(start, stop)` | Start diagonal scroll right |
| `ssd1306_start_scroll_diag_left(start, stop)` | Start diagonal scroll left |
| `ssd1306_stop_scroll()` | Stop scrolling |

## 🧪 Testing Your Installation

The installation script creates a hardware test script at `/tmp/ssd1306_test.php`:

```bash
# Test with Yahboom CUBE (bus 7, address 0x3C)
php /tmp/ssd1306_test.php

# Test with custom I2C configuration
php /tmp/ssd1306_test.php 1 0x3D
```

## 🔍 Troubleshooting

### Display Not Working
1. **Wrong I2C bus**: Yahboom CUBE uses bus 7, not bus 1
2. **Check hardware**: `i2cdetect -y 7` should show device at 0x3C
3. **Permissions**: Add user to i2c group: `sudo usermod -a -G i2c $USER`
4. **Alternative address**: Try 0x3D instead of 0x3C

### Extension Not Loading
1. **Check installation**: `php -m | grep ssd1306`
2. **Verify configuration**: Look for `extension=ssd1306` in PHP config
3. **Check PHP version**: Extension optimized for PHP 8.4.x

### Permission Denied
```bash
# Add user to i2c group and restart session
sudo usermod -a -G i2c $USER
# Log out and back in, or run: newgrp i2c
```

## 📈 Performance Notes

- **Memory Efficient**: 1KB display buffer for 128x64 displays
- **Fast Updates**: Direct I2C communication without overhead
- **Optimized Graphics**: Hardware-accelerated primitives where possible
- **Resource Safe**: Automatic cleanup prevents memory leaks

## 🛠️ Development & Testing

### Test Suite Results
All 4 test cases pass:
- ✅ Basic functionality test
- ✅ Graphics functions test  
- ✅ Text functions test
- ✅ Scrolling functions test

### Build Environment
- **Compiler**: GCC with C11 support
- **PHP API**: 20230831 (PHP 8.3/8.4 compatible)
- **Architecture**: ARM64 (aarch64) optimized
- **Dependencies**: i2c-tools, libi2c-dev

## 📦 Package Contents

This release includes:
- **Source Code**: Complete C implementation
- **Installation Script**: One-command setup for Jetson Orin
- **Documentation**: Comprehensive API reference and examples
- **Test Suite**: 4 comprehensive test cases
- **Examples**: 4 demo scripts showing different features
- **Hardware Test**: Validation script for real hardware

## 🔗 Resources

- **GitHub Repository**: https://github.com/projectsaturnstudios/adafruit_ssd1306-php
- **Documentation**: See README.md for complete guide
- **Hardware Guide**: Yahboom CUBE case setup instructions
- **Support**: Create issues on GitHub for bug reports

## 🙏 Acknowledgments

- **Adafruit**: Based on their excellent SSD1306 Arduino library
- **Community**: Thanks to early testers and contributors
- **Yahboom**: For the excellent CUBE case that makes hardware setup easy

## 📄 License

This extension is licensed under the PHP License. See LICENSE file for details.

---

**Project Saturn Studios, LLC**  
Email: info@projectsaturnstudios.com  
Website: https://github.com/projectsaturnstudios

*Happy coding with your SSD1306 displays! 🚀*