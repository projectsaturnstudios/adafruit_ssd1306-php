#!/bin/bash

# SSD1306 PHP Extension - Installation Script for NVIDIA Jetson Orin Nano
# Ubuntu 22.04.5 LTS with PHP 8.4.x
# Version: 1.0.0
# Author: Project Saturn Studios, LLC

set -e

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print colored output
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  SSD1306 PHP Extension v1.0.0${NC}"
    echo -e "${BLUE}  NVIDIA Jetson Orin Nano Installer${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# Check if running as root
check_root() {
    if [[ $EUID -eq 0 ]]; then
        print_error "This script should not be run as root. Please run as a regular user with sudo privileges."
        exit 1
    fi
}

# Check system compatibility
check_system() {
    print_info "Checking system compatibility..."
    
    # Check if running on Jetson
    if [ ! -f /etc/nv_tegra_release ]; then
        print_warning "This script is designed for NVIDIA Jetson devices. Continuing anyway..."
    else
        print_success "NVIDIA Jetson device detected"
        cat /etc/nv_tegra_release
    fi
    
    # Check Ubuntu version
    . /etc/os-release
    if [[ "$VERSION_ID" != "22.04" ]]; then
        print_warning "This script is optimized for Ubuntu 22.04.5 LTS. Detected: $PRETTY_NAME"
    else
        print_success "Ubuntu 22.04 detected"
    fi
    
    # Check architecture
    ARCH=$(uname -m)
    if [[ "$ARCH" != "aarch64" ]]; then
        print_warning "This script is optimized for ARM64 (aarch64) architecture. Detected: $ARCH"
    else
        print_success "ARM64 architecture detected"
    fi
}

# Check and install PHP 8.4
install_php() {
    print_info "Checking PHP installation..."
    
    # Check if PHP is installed
    if ! command -v php &> /dev/null; then
        print_info "PHP not found. Installing PHP 8.4..."
        
        # Add Ondrej's PPA for PHP 8.4
        sudo apt update
        sudo apt install -y software-properties-common
        sudo add-apt-repository -y ppa:ondrej/php
        sudo apt update
        
        # Install PHP 8.4 and development packages
        sudo apt install -y php8.4 php8.4-dev php8.4-cli php8.4-common \
                           php8.4-curl php8.4-mbstring php8.4-xml php8.4-zip
    else
        PHP_VERSION=$(php -r "echo PHP_VERSION;")
        print_info "PHP found: $PHP_VERSION"
        
        # Check if PHP 8.4
        if [[ "$PHP_VERSION" =~ ^8\.4\. ]]; then
            print_success "PHP 8.4.x detected"
        else
            print_warning "PHP $PHP_VERSION detected. Extension optimized for PHP 8.4.x"
        fi
        
        # Ensure php-dev is installed
        if ! command -v phpize &> /dev/null; then
            print_info "Installing PHP development packages..."
            sudo apt install -y php-dev
        fi
    fi
}

# Install build dependencies
install_dependencies() {
    print_info "Installing build dependencies..."
    
    sudo apt update
    sudo apt install -y \
        build-essential \
        autoconf \
        automake \
        libtool \
        pkg-config \
        i2c-tools \
        libi2c-dev \
        git
        
    print_success "Dependencies installed"
}

# Add user to i2c group
configure_i2c() {
    print_info "Configuring I2C permissions..."
    
    # Check if i2c group exists
    if getent group i2c >/dev/null 2>&1; then
        # Add current user to i2c group
        sudo usermod -a -G i2c $USER
        print_success "User $USER added to i2c group"
        print_warning "You will need to log out and back in for group changes to take effect"
    else
        print_warning "i2c group not found. You may need to run as root or configure permissions manually"
    fi
    
    # Check available I2C buses
    print_info "Available I2C buses:"
    i2cdetect -l || print_warning "Could not list I2C buses. Make sure i2c-tools is installed"
}

# Clone and build extension
build_extension() {
    print_info "Building SSD1306 PHP extension..."
    
    # Create temporary directory
    TEMP_DIR=$(mktemp -d)
    cd "$TEMP_DIR"
    
    # Clone repository
    print_info "Cloning repository..."
    git clone https://github.com/projectsaturnstudios/adafruit_ssd1306-php.git
    cd adafruit_ssd1306-php
    
    # Checkout v1.0.0 tag
    git checkout v1.0.0
    
    # Build extension
    print_info "Configuring build environment..."
    phpize
    
    print_info "Configuring extension..."
    ./configure --enable-ssd1306
    
    print_info "Compiling extension..."
    make
    
    print_info "Running tests..."
    make test || print_warning "Some tests may fail without actual hardware"
    
    # Install extension
    print_info "Installing extension..."
    sudo make install
    
    # Get PHP extension directory
    EXT_DIR=$(php-config --extension-dir)
    print_success "Extension installed to: $EXT_DIR"
    
    # Clean up
    cd /
    rm -rf "$TEMP_DIR"
}

# Configure PHP to load extension
configure_php() {
    print_info "Configuring PHP to load SSD1306 extension..."
    
    # Find PHP ini directory
    PHP_INI_DIR="/etc/php/$(php -r 'echo PHP_MAJOR_VERSION.".".PHP_MINOR_VERSION;')/mods-available"
    
    if [ -d "$PHP_INI_DIR" ]; then
        # Create extension configuration file
        echo "extension=ssd1306" | sudo tee "$PHP_INI_DIR/ssd1306.ini"
        
        # Enable extension for CLI
        sudo phpenmod ssd1306
        
        print_success "Extension configuration created"
    else
        # Fallback: add to main php.ini
        PHP_INI=$(php --ini | grep "Loaded Configuration File" | cut -d: -f2 | xargs)
        if [ -f "$PHP_INI" ]; then
            echo "extension=ssd1306" | sudo tee -a "$PHP_INI"
            print_success "Extension added to php.ini"
        else
            print_error "Could not find PHP configuration file"
            return 1
        fi
    fi
}

# Verify installation
verify_installation() {
    print_info "Verifying installation..."
    
    # Check if extension is loaded
    if php -m | grep -q ssd1306; then
        print_success "SSD1306 extension is loaded"
        
        # Show extension info
        php -r "
        if (extension_loaded('ssd1306')) {
            echo 'Extension version: ' . phpversion('ssd1306') . PHP_EOL;
            echo 'Available functions: ' . PHP_EOL;
            \$functions = get_extension_funcs('ssd1306');
            foreach (\$functions as \$func) {
                echo '  - ' . \$func . PHP_EOL;
            }
        }
        "
    else
        print_error "SSD1306 extension is not loaded. Please check your PHP configuration."
        return 1
    fi
}

# Create test script
create_test_script() {
    print_info "Creating hardware test script..."
    
    cat > /tmp/ssd1306_test.php << 'EOF'
<?php
/**
 * SSD1306 Hardware Test Script
 * Test your SSD1306 display with various patterns
 * 
 * Usage: php ssd1306_test.php [i2c_bus] [i2c_address]
 * Default: php ssd1306_test.php 7 0x3C (for Yahboom CUBE case)
 */

// Get command line arguments
$i2c_bus = isset($argv[1]) ? (int)$argv[1] : 7;  // Default bus 7 for Yahboom CUBE
$i2c_addr = isset($argv[2]) ? hexdec($argv[2]) : 0x3C;  // Default address 0x3C

echo "SSD1306 Hardware Test Script v1.0.0\n";
echo "====================================\n";
echo "I2C Bus: $i2c_bus\n";
echo "I2C Address: 0x" . dechex($i2c_addr) . "\n\n";

// Test 1: Initialize display
echo "Test 1: Initializing display...\n";
if (!ssd1306_begin($i2c_bus, $i2c_addr)) {
    echo "ERROR: Failed to initialize display!\n";
    echo "Check:\n";
    echo "- I2C bus number (try: i2cdetect -l)\n";
    echo "- I2C address (try: i2cdetect -y $i2c_bus)\n";
    echo "- Hardware connections\n";
    echo "- User permissions (member of i2c group)\n";
    exit(1);
}
echo "SUCCESS: Display initialized\n\n";

// Test 2: Clear and basic display
echo "Test 2: Clear screen test...\n";
ssd1306_clear_display();
ssd1306_display();
echo "SUCCESS: Screen cleared\n";
sleep(1);

// Test 3: Pixel test
echo "Test 3: Pixel drawing test...\n";
for ($i = 0; $i < 20; $i++) {
    $x = rand(0, ssd1306_get_width() - 1);
    $y = rand(0, ssd1306_get_height() - 1);
    ssd1306_draw_pixel($x, $y, SSD1306_WHITE);
}
ssd1306_display();
echo "SUCCESS: Random pixels drawn\n";
sleep(2);

// Test 4: Text test
echo "Test 4: Text rendering test...\n";
ssd1306_clear_display();
ssd1306_set_cursor(0, 0);
ssd1306_set_text_size(1);
ssd1306_set_text_color(SSD1306_WHITE);
ssd1306_print("SSD1306 v1.0.0");
ssd1306_set_cursor(0, 16);
ssd1306_set_text_size(2);
ssd1306_print("WORKS!");
ssd1306_display();
echo "SUCCESS: Text rendered\n";
sleep(2);

// Test 5: Graphics test
echo "Test 5: Graphics primitives test...\n";
ssd1306_clear_display();

// Draw rectangles
ssd1306_draw_rect(0, 0, 32, 16, SSD1306_WHITE);
ssd1306_fill_rect(40, 0, 32, 16, SSD1306_WHITE);

// Draw circles
ssd1306_draw_circle(16, 40, 12, SSD1306_WHITE);
ssd1306_fill_circle(56, 40, 12, SSD1306_WHITE);

// Draw lines
for ($i = 80; $i < 128; $i += 4) {
    ssd1306_draw_line(80, 0, $i, 63, SSD1306_WHITE);
}

ssd1306_display();
echo "SUCCESS: Graphics primitives drawn\n";
sleep(3);

// Test 6: Cleanup
echo "Test 6: Cleanup...\n";
ssd1306_clear_display();
ssd1306_display();
ssd1306_end();
echo "SUCCESS: Cleanup complete\n\n";

echo "All tests completed successfully!\n";
echo "Your SSD1306 display is working properly.\n";
EOF

    chmod +x /tmp/ssd1306_test.php
    print_success "Test script created: /tmp/ssd1306_test.php"
}

# Show usage instructions
show_usage() {
    print_info "Installation completed! Here's how to use the extension:"
    echo ""
    echo "1. Test your hardware:"
    echo "   php /tmp/ssd1306_test.php"
    echo ""
    echo "   For custom I2C bus/address:"
    echo "   php /tmp/ssd1306_test.php 1 0x3D"
    echo ""
    echo "2. Basic PHP usage:"
    echo '   <?php'
    echo '   // Initialize (bus 7, address 0x3C for Yahboom CUBE)'
    echo '   ssd1306_begin(7, 0x3C);'
    echo '   ssd1306_clear_display();'
    echo '   ssd1306_print("Hello World!");'
    echo '   ssd1306_display();'
    echo '   ssd1306_end();'
    echo '   ?>'
    echo ""
    echo "3. Find your I2C bus and address:"
    echo "   i2cdetect -l        # List buses"
    echo "   i2cdetect -y 7      # Scan bus 7 (Yahboom CUBE)"
    echo "   i2cdetect -y 1      # Scan bus 1 (common default)"
    echo ""
    echo "4. Documentation and examples:"
    echo "   https://github.com/projectsaturnstudios/adafruit_ssd1306-php"
    echo ""
    print_warning "Note: You may need to log out and back in for I2C group permissions to take effect."
}

# Main installation function
main() {
    print_header
    
    check_root
    check_system
    install_dependencies
    install_php
    configure_i2c
    build_extension
    configure_php
    verify_installation
    create_test_script
    show_usage
    
    print_success "SSD1306 PHP Extension v1.0.0 installation completed!"
}

# Run main function
main "$@"