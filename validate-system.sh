#!/bin/bash

# SSD1306 PHP Extension - Pre-Installation Validation Script
# Checks system compatibility before installation
# Version: 1.0.0

set -e

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
CHECKS_PASSED=0
CHECKS_FAILED=0
CHECKS_WARNING=0

print_header() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}  SSD1306 PHP Extension v1.0.0${NC}"
    echo -e "${BLUE}  Pre-Installation Validation${NC}"
    echo -e "${BLUE}================================================${NC}"
    echo ""
}

print_check() {
    echo -n "Checking $1... "
}

print_pass() {
    echo -e "${GREEN}PASS${NC}"
    ((CHECKS_PASSED++))
}

print_fail() {
    echo -e "${RED}FAIL${NC}"
    echo -e "${RED}  → $1${NC}"
    ((CHECKS_FAILED++))
}

print_warn() {
    echo -e "${YELLOW}WARN${NC}"
    echo -e "${YELLOW}  → $1${NC}"
    ((CHECKS_WARNING++))
}

print_info() {
    echo -e "${BLUE}  ℹ $1${NC}"
}

check_os() {
    print_check "Operating System"
    
    if [ ! -f /etc/os-release ]; then
        print_fail "Cannot detect OS version"
        return
    fi
    
    . /etc/os-release
    
    if [[ "$ID" == "ubuntu" ]]; then
        if [[ "$VERSION_ID" == "22.04" ]]; then
            print_pass
            print_info "Ubuntu $VERSION_ID detected (optimal)"
        else
            print_warn "Ubuntu $VERSION_ID detected (designed for 22.04)"
        fi
    else
        print_warn "$PRETTY_NAME detected (designed for Ubuntu 22.04)"
    fi
}

check_architecture() {
    print_check "System Architecture"
    
    ARCH=$(uname -m)
    case "$ARCH" in
        "aarch64")
            print_pass
            print_info "ARM64 architecture (optimal for Jetson)"
            ;;
        "x86_64")
            print_warn "x86_64 architecture (will work, but designed for ARM64)"
            ;;
        *)
            print_warn "Unknown architecture: $ARCH"
            ;;
    esac
}

check_jetson() {
    print_check "NVIDIA Jetson Hardware"
    
    if [ -f /etc/nv_tegra_release ]; then
        print_pass
        print_info "$(cat /etc/nv_tegra_release | head -1)"
        
        # Check for Orin specifically
        if grep -q "Orin" /etc/nv_tegra_release 2>/dev/null; then
            print_info "Jetson Orin detected (optimal target)"
        fi
    else
        print_warn "Not running on NVIDIA Jetson (extension will work on other systems)"
    fi
}

check_php() {
    print_check "PHP Installation"
    
    if command -v php &> /dev/null; then
        PHP_VERSION=$(php -r "echo PHP_VERSION;")
        print_pass
        print_info "PHP $PHP_VERSION found"
        
        # Check PHP version compatibility
        if [[ "$PHP_VERSION" =~ ^8\.[4-9]\. ]]; then
            print_info "PHP version optimal (8.4+)"
        elif [[ "$PHP_VERSION" =~ ^8\.[0-3]\. ]]; then
            print_info "PHP version compatible (8.0+)"
        else
            print_warn "PHP version may not be compatible (requires 8.0+)"
        fi
    else
        print_fail "PHP not found - will be installed during setup"
    fi
}

check_phpize() {
    print_check "PHP Development Tools"
    
    if command -v phpize &> /dev/null; then
        print_pass
        print_info "phpize found - ready for extension compilation"
    else
        print_fail "phpize not found - php-dev will be installed"
    fi
}

check_build_tools() {
    print_check "Build Tools"
    
    missing_tools=()
    
    for tool in gcc make autoconf; do
        if ! command -v "$tool" &> /dev/null; then
            missing_tools+=("$tool")
        fi
    done
    
    if [ ${#missing_tools[@]} -eq 0 ]; then
        print_pass
        print_info "All build tools available"
    else
        print_fail "Missing tools: ${missing_tools[*]} - will be installed"
    fi
}

check_i2c_tools() {
    print_check "I2C Tools"
    
    if command -v i2cdetect &> /dev/null; then
        print_pass
        print_info "i2c-tools installed"
    else
        print_fail "i2c-tools not found - will be installed"
    fi
}

check_i2c_dev() {
    print_check "I2C Development Headers"
    
    if [ -f /usr/include/linux/i2c-dev.h ]; then
        print_pass
        print_info "libi2c-dev headers found"
    else
        print_fail "libi2c-dev not found - will be installed"
    fi
}

check_i2c_permissions() {
    print_check "I2C Permissions"
    
    if groups | grep -q "\bi2c\b"; then
        print_pass
        print_info "User is in i2c group"
    else
        print_warn "User not in i2c group - will be added during installation"
        print_info "You'll need to log out and back in after installation"
    fi
}

check_i2c_buses() {
    print_check "I2C Bus Availability"
    
    if command -v i2cdetect &> /dev/null; then
        buses=$(i2cdetect -l 2>/dev/null | wc -l)
        if [ "$buses" -gt 0 ]; then
            print_pass
            print_info "$buses I2C bus(es) detected"
            
            # Check for common buses
            if i2cdetect -l 2>/dev/null | grep -q "i2c-7"; then
                print_info "Bus 7 found (used by Yahboom CUBE)"
            fi
            if i2cdetect -l 2>/dev/null | grep -q "i2c-1"; then
                print_info "Bus 1 found (common default)"
            fi
        else
            print_warn "No I2C buses detected - hardware may not be available"
        fi
    else
        print_warn "Cannot check I2C buses - i2c-tools not installed"
    fi
}

check_sudo() {
    print_check "Sudo Privileges"
    
    if sudo -n true 2>/dev/null; then
        print_pass
        print_info "Passwordless sudo available"
    elif sudo -v 2>/dev/null; then
        print_pass
        print_info "Sudo privileges available"
    else
        print_fail "Sudo privileges required for installation"
    fi
}

check_internet() {
    print_check "Internet Connection"
    
    if ping -c 1 google.com &> /dev/null; then
        print_pass
        print_info "Internet connection available"
    else
        print_fail "Internet connection required for package installation"
    fi
}

check_disk_space() {
    print_check "Disk Space"
    
    available=$(df / | awk 'NR==2 {print $4}')
    required=512000  # 500MB in KB
    
    if [ "$available" -gt "$required" ]; then
        print_pass
        print_info "$(($available / 1024))MB available (need ~100MB)"
    else
        print_warn "Low disk space: $(($available / 1024))MB available"
    fi
}

# Hardware detection for Yahboom CUBE
check_yahboom_cube() {
    print_check "Yahboom CUBE Detection"
    
    if [ -f /etc/nv_tegra_release ] && command -v i2cdetect &> /dev/null; then
        # Check if SSD1306 is on bus 7 at address 0x3C
        if i2cdetect -y 7 2>/dev/null | grep -q "3c"; then
            print_pass
            print_info "SSD1306 display detected on bus 7 (Yahboom CUBE)"
        elif i2cdetect -y 1 2>/dev/null | grep -q "3c"; then
            print_warn "SSD1306 found on bus 1 (not Yahboom CUBE setup)"
        else
            print_warn "SSD1306 display not detected (may need different bus/address)"
        fi
    else
        print_warn "Cannot detect Yahboom CUBE setup"
    fi
}

print_summary() {
    echo ""
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}  Validation Summary${NC}"
    echo -e "${BLUE}================================================${NC}"
    
    echo -e "${GREEN}Passed: $CHECKS_PASSED${NC}"
    echo -e "${YELLOW}Warnings: $CHECKS_WARNING${NC}"
    echo -e "${RED}Failed: $CHECKS_FAILED${NC}"
    echo ""
    
    if [ $CHECKS_FAILED -eq 0 ]; then
        echo -e "${GREEN}✓ System ready for SSD1306 extension installation!${NC}"
        echo ""
        echo "To install, run:"
        echo -e "${BLUE}curl -fsSL https://github.com/projectsaturnstudios/adafruit_ssd1306-php/raw/v1.0.0/install-jetson-orin.sh | bash${NC}"
    elif [ $CHECKS_FAILED -le 3 ]; then
        echo -e "${YELLOW}⚠ System mostly ready - installation script will fix missing dependencies${NC}"
        echo ""
        echo "To install (dependencies will be installed automatically):"
        echo -e "${BLUE}curl -fsSL https://github.com/projectsaturnstudios/adafruit_ssd1306-php/raw/v1.0.0/install-jetson-orin.sh | bash${NC}"
    else
        echo -e "${RED}✗ System needs preparation before installation${NC}"
        echo ""
        echo "Please address the failed checks above, then run this script again."
    fi
    
    if [ $CHECKS_WARNING -gt 0 ]; then
        echo ""
        echo -e "${YELLOW}Note: Warnings indicate non-optimal conditions but won't prevent installation.${NC}"
    fi
}

main() {
    print_header
    
    check_os
    check_architecture
    check_jetson
    check_php
    check_phpize
    check_build_tools
    check_i2c_tools
    check_i2c_dev
    check_i2c_permissions
    check_i2c_buses
    check_sudo
    check_internet
    check_disk_space
    check_yahboom_cube
    
    print_summary
}

# Run validation
main "$@"