# Contributing to PHP SSD1306 Extension

Thank you for your interest in contributing to the PHP SSD1306 extension! This document provides guidelines for contributing to the project.

## Development Setup

### Prerequisites
- PHP 8.0+ with development headers
- GCC compiler and build tools
- I2C development libraries
- SSD1306 OLED display for testing

### Building from Source
```bash
git clone https://github.com/projectsaturnstudios/php-ssd1306.git
cd php-ssd1306
phpize
./configure --enable-ssd1306
make
```

### Running Tests
```bash
make test
```

## Code Style

### C Code
- Follow PHP extension coding standards
- Use 4-space indentation
- Include proper error handling
- Add comprehensive comments for complex logic
- Ensure memory safety and proper cleanup

### Documentation
- Update README.md for new features
- Add examples for new functionality
- Include proper API documentation
- Update CHANGELOG.md for all changes

## Submitting Changes

### Pull Request Process
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass
6. Update documentation
7. Commit your changes (`git commit -m 'Add amazing feature'`)
8. Push to the branch (`git push origin feature/amazing-feature`)
9. Open a Pull Request

### Commit Messages
- Use clear, descriptive commit messages
- Start with a verb in present tense
- Keep the first line under 50 characters
- Include detailed description if needed

Example:
```
Add support for 96x16 displays

- Implement display size detection
- Update initialization parameters
- Add tests for new display size
- Update documentation
```

## Testing

### Hardware Testing
- Test on real SSD1306 hardware when possible
- Verify I2C communication works correctly
- Test different display sizes and configurations
- Ensure proper cleanup and resource management

### Unit Tests
- Add .phpt tests for new functions
- Test error conditions and edge cases
- Verify memory management
- Test with different PHP versions

## Reporting Issues

### Bug Reports
Include the following information:
- PHP version and configuration
- Operating system and version
- Hardware setup (display model, I2C bus, etc.)
- Steps to reproduce the issue
- Expected vs actual behavior
- Error messages or logs

### Feature Requests
- Describe the desired functionality
- Explain the use case and benefits
- Consider implementation complexity
- Provide examples if possible

## Code Review

All submissions require code review. We use GitHub pull requests for this purpose. Reviewers will check for:

- Code quality and style
- Test coverage
- Documentation completeness
- Hardware compatibility
- Performance implications
- Security considerations

## License

By contributing to this project, you agree that your contributions will be licensed under the same license as the project (PHP License).

## Questions?

Feel free to open an issue for questions about contributing or reach out to the maintainers.

Thank you for contributing! 🚀
