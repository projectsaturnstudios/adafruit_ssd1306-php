#!/bin/bash

# SSD1306 PHP Extension v1.0.0 - Release Verification Script
# Verifies that the extension is properly installed and functional

set -e

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  SSD1306 Extension v1.0.0${NC}"
    echo -e "${BLUE}  Installation Verification${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ $1${NC}"
}

check_php() {
    echo "Checking PHP installation..."
    
    if ! command -v php &> /dev/null; then
        print_error "PHP not found"
        return 1
    fi
    
    PHP_VERSION=$(php -r "echo PHP_VERSION;")
    print_success "PHP $PHP_VERSION found"
    
    return 0
}

check_extension() {
    echo "Checking SSD1306 extension..."
    
    if ! php -m | grep -q ssd1306; then
        print_error "SSD1306 extension not loaded"
        print_info "Make sure extension=ssd1306 is in your php.ini"
        return 1
    fi
    
    print_success "SSD1306 extension loaded"
    
    # Get extension version if available
    php -r "
    if (extension_loaded('ssd1306')) {
        echo 'Extension functions available: ' . count(get_extension_funcs('ssd1306')) . PHP_EOL;
    }
    " 2>/dev/null || true
    
    return 0
}

check_functions() {
    echo "Checking extension functions..."
    
    # Test basic functions exist
    php -r "
    \$functions = [
        'ssd1306_begin',
        'ssd1306_end', 
        'ssd1306_display',
        'ssd1306_clear_display',
        'ssd1306_draw_pixel',
        'ssd1306_print',
        'ssd1306_get_width',
        'ssd1306_get_height'
    ];
    
    \$missing = [];
    foreach (\$functions as \$func) {
        if (!function_exists(\$func)) {
            \$missing[] = \$func;
        }
    }
    
    if (empty(\$missing)) {
        echo 'All core functions available' . PHP_EOL;
        exit(0);
    } else {
        echo 'Missing functions: ' . implode(', ', \$missing) . PHP_EOL;
        exit(1);
    }
    " && print_success "All core functions available" || {
        print_error "Some functions missing"
        return 1
    }
    
    return 0
}

check_constants() {
    echo "Checking extension constants..."
    
    php -r "
    \$constants = [
        'SSD1306_WHITE',
        'SSD1306_BLACK', 
        'SSD1306_INVERSE',
        'SSD1306_I2C_ADDRESS',
        'SSD1306_LCDWIDTH_128',
        'SSD1306_LCDHEIGHT_64'
    ];
    
    \$missing = [];
    foreach (\$constants as \$const) {
        if (!defined(\$const)) {
            \$missing[] = \$const;
        }
    }
    
    if (empty(\$missing)) {
        echo 'All constants defined' . PHP_EOL;
        exit(0);
    } else {
        echo 'Missing constants: ' . implode(', ', \$missing) . PHP_EOL;
        exit(1);
    }
    " && print_success "All constants defined" || {
        print_error "Some constants missing"
        return 1
    }
    
    return 0
}

test_basic_functionality() {
    echo "Testing basic functionality (without hardware)..."
    
    php -r "
    // Test function calls that don't require hardware
    try {
        // These should work without actual hardware
        \$width = ssd1306_get_width();
        \$height = ssd1306_get_height();
        
        if (\$width > 0 && \$height > 0) {
            echo 'Display dimensions: ' . \$width . 'x' . \$height . PHP_EOL;
            echo 'Basic functionality test passed' . PHP_EOL;
        } else {
            echo 'Invalid display dimensions' . PHP_EOL;
            exit(1);
        }
        
    } catch (Exception \$e) {
        echo 'Function test failed: ' . \$e->getMessage() . PHP_EOL;
        exit(1);
    }
    " && print_success "Basic functionality test passed" || {
        print_error "Basic functionality test failed"
        return 1
    }
    
    return 0
}

check_i2c() {
    echo "Checking I2C setup..."
    
    if command -v i2cdetect &> /dev/null; then
        print_success "i2c-tools available"
        
        # Check for common buses
        if i2cdetect -l 2>/dev/null | grep -q "i2c-7"; then
            print_info "I2C bus 7 available (Yahboom CUBE)"
        fi
        if i2cdetect -l 2>/dev/null | grep -q "i2c-1"; then
            print_info "I2C bus 1 available (common default)"
        fi
        
        # Check permissions
        if groups | grep -q "\bi2c\b"; then
            print_success "User in i2c group"
        else
            print_error "User not in i2c group - hardware access may fail"
            print_info "Run: sudo usermod -a -G i2c \$USER && newgrp i2c"
        fi
    else
        print_error "i2c-tools not found"
        return 1
    fi
    
    return 0
}

create_test_script() {
    echo "Creating hardware test script..."
    
    cat > /tmp/quick_test.php << 'EOF'
<?php
echo "SSD1306 Quick Hardware Test\n";
echo "===========================\n";

// Test initialization (will fail if no hardware, but that's OK)
echo "Testing initialization...\n";
$result = @ssd1306_begin(7, 0x3C);  // @ suppresses warnings
if ($result) {
    echo "SUCCESS: Display initialized on bus 7\n";
    ssd1306_clear_display();
    ssd1306_print("Hello v1.0.0!");
    ssd1306_display();
    ssd1306_end();
    echo "Test pattern displayed\n";
} else {
    echo "INFO: No hardware detected on bus 7 (this is normal without SSD1306)\n";
    
    // Try bus 1
    $result = @ssd1306_begin(1, 0x3C);
    if ($result) {
        echo "SUCCESS: Display initialized on bus 1\n";
        ssd1306_clear_display();
        ssd1306_print("Hello v1.0.0!");
        ssd1306_display();
        ssd1306_end();
        echo "Test pattern displayed\n";
    } else {
        echo "INFO: No hardware detected on bus 1 either\n";
        echo "This is normal if no SSD1306 display is connected\n";
    }
}

echo "\nExtension is working correctly!\n";
echo "Connect an SSD1306 display and run: php /tmp/ssd1306_test.php\n";
EOF

    chmod +x /tmp/quick_test.php
    print_success "Test script created: /tmp/quick_test.php"
}

run_verification() {
    print_header
    
    local failed=0
    
    check_php || ((failed++))
    check_extension || ((failed++))
    check_functions || ((failed++))
    check_constants || ((failed++))
    test_basic_functionality || ((failed++))
    check_i2c || ((failed++))
    create_test_script
    
    echo ""
    echo "Running quick test..."
    php /tmp/quick_test.php
    
    echo ""
    if [ $failed -eq 0 ]; then
        print_success "All verification checks passed!"
        echo ""
        echo -e "${GREEN}🎉 SSD1306 Extension v1.0.0 is ready to use!${NC}"
        echo ""
        echo "Next steps:"
        echo "1. Connect your SSD1306 display via I2C"
        echo "2. Run: php /tmp/ssd1306_test.php"
        echo "3. Check the documentation for API reference"
    else
        print_error "$failed verification check(s) failed"
        echo ""
        echo "Please address the issues above before using the extension."
        return 1
    fi
}

# Run verification
run_verification "$@"