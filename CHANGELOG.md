# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-01-10

### Added
- Initial release of SSD1306 PHP extension
- Complete I2C communication support for SSD1306 OLED displays
- Display management functions (begin, end, display, clear, invert, dim, contrast)
- Graphics primitives (pixels, lines, rectangles, circles, filled shapes)
- Text rendering with scalable font sizes and cursor positioning
- Scrolling animations (horizontal and diagonal)
- Support for multiple display sizes (128x64, 128x32, 96x16)
- Hardware-tested on real SSD1306 displays
- PECL package support for easy installation
- Comprehensive documentation and examples
- Full test suite with .phpt tests

### Technical Details
- Native C implementation for optimal performance
- Direct I2C communication via Linux i2c-dev
- Complete ASCII font set (characters 32-126)
- Memory-efficient display buffer management
- Proper error handling and resource cleanup
- Compatible with PHP 8.0+

### Examples Included
- Basic display operations
- Graphics and animation demonstrations
- Text rendering with multiple sizes
- Scrolling effects showcase
