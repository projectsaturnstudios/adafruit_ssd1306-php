# SSD1306 PHP Extension - Precompiled Binary

## System Requirements

This precompiled binary is specifically built for:

- **Hardware**: NVIDIA Jetson Orin Nano
- **OS**: Ubuntu 22.04.5 LTS (Jammy) with JetPack
- **Architecture**: aarch64 (ARM64)
- **PHP Version**: 8.4.12 (CLI, NTS)
- **Kernel**: Linux 5.15.148-tegra

## Quick Installation

1. **Copy the extension**:
   ```bash
   sudo cp ssd1306.so /usr/lib/php/20240924/
   ```

2. **Enable the extension**:
   ```bash
   echo "extension=ssd1306.so" | sudo tee -a /etc/php/8.4/cli/php.ini
   ```

3. **Verify installation**:
   ```bash
   php -m | grep ssd1306
   ```

## Usage Example

```php
<?php
// Initialize display (I2C bus 7 for Yahboom CUBE case)
if (!ssd1306_begin(7, 0x3C)) {
    die("Failed to initialize SSD1306 display\n");
}

// Clear and draw
ssd1306_clear_display();
ssd1306_set_text_size(2);
ssd1306_set_cursor(0, 0);
ssd1306_print("Hello PHP!");
ssd1306_display();
?>
```

## Troubleshooting

- **Wrong I2C bus**: Try different bus numbers (0-7) with `ssd1306_begin()`
- **Permission denied**: Add user to i2c group: `sudo usermod -a -G i2c $USER`
- **Extension not loading**: Check PHP version compatibility with `php --version`

## Build Information

- **Compiled on**: Jetson Orin Nano
- **Build date**: January 10, 2025
- **Compiler**: GCC (aarch64-linux-gnu)
- **PHP API**: 20240924
- **Zend Module API**: 20240924

For other systems, please compile from source: https://github.com/projectsaturnstudios/adafruit_ssd1306-php
