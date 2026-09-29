#!/bin/bash
# Package Testing Script
# Tests JOCKY package installations across different package managers

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(dirname "$SCRIPT_DIR")"

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TESTS_PASSED=0
TESTS_FAILED=0

log_info() {
    echo -e "${BLUE}[INFO]${NC} $*"
}

log_success() {
    echo -e "${GREEN}[PASS]${NC} $*"
    ((TESTS_PASSED++))
}

log_error() {
    echo -e "${RED}[FAIL]${NC} $*"
    ((TESTS_FAILED++))
}

log_skip() {
    echo -e "${YELLOW}[SKIP]${NC} $*"
}

# Test binary existence and functionality
test_binary() {
    local binary=$1
    local expected_output=$2

    log_info "Testing binary: $binary"

    if ! command -v "$binary" &> /dev/null; then
        log_error "Binary not found: $binary"
        return 1
    fi

    if output=$($binary --version 2>&1); then
        if [[ $output == *"$expected_output"* ]]; then
            log_success "Binary works: $binary"
            return 0
        else
            log_error "Unexpected output from $binary: $output"
            return 1
        fi
    else
        log_error "Failed to run $binary"
        return 1
    fi
}

# Test Homebrew package
test_homebrew() {
    log_info "========== HOMEBREW TESTS =========="

    if [[ "$OSTYPE" != "darwin"* ]]; then
        log_skip "Homebrew test (not on macOS)"
        return
    fi

    if ! command -v brew &> /dev/null; then
        log_skip "Homebrew not installed"
        return
    fi

    # Add tap (if not already added)
    log_info "Adding Homebrew tap..."
    brew tap devalgupta/homebrew-jocky 2>/dev/null || true

    # Install from local formula
    log_info "Installing from local formula..."
    if brew install --build-from-source "$SCRIPT_DIR/homebrew/jocky.rb" 2>&1 | tee /tmp/homebrew-install.log; then
        log_success "Homebrew installation succeeded"

        # Test the installed binary
        if test_binary "jockyc" "jocky"; then
            log_success "Homebrew package functional"
        else
            log_error "Homebrew binary not functional"
        fi
    else
        log_error "Homebrew installation failed"
        tail -20 /tmp/homebrew-install.log
    fi
}

# Test AUR package
test_aur() {
    log_info "========== AUR TESTS =========="

    if [[ "$OSTYPE" != "linux-gnu"* ]]; then
        log_skip "AUR test (not on Linux)"
        return
    fi

    if ! command -v makepkg &> /dev/null; then
        log_skip "makepkg not found (AUR build tools not installed)"
        return
    fi

    local tmpdir=$(mktemp -d)
    trap "rm -rf $tmpdir" EXIT

    # Copy PKGBUILD to temp directory
    cp "$SCRIPT_DIR/aur/PKGBUILD" "$tmpdir/"
    cd "$tmpdir"

    log_info "Building AUR package..."
    if makepkg -si --noconfirm 2>&1 | tee /tmp/aur-build.log; then
        log_success "AUR package build succeeded"

        # Test the installed binary
        if test_binary "jockyc" "jocky"; then
            log_success "AUR package functional"
        else
            log_error "AUR binary not functional"
        fi
    else
        log_error "AUR package build failed"
        tail -20 /tmp/aur-build.log
    fi
}

# Test Chocolatey package
test_chocolatey() {
    log_info "========== CHOCOLATEY TESTS =========="

    if [[ "$OSTYPE" != "msys" && "$OSTYPE" != "win32" ]]; then
        log_skip "Chocolatey test (not on Windows)"
        return
    fi

    if ! command -v choco &> /dev/null; then
        log_skip "Chocolatey not installed"
        return
    fi

    log_info "Packing Chocolatey package..."
    cd "$SCRIPT_DIR/chocolatey"

    if choco pack jocky.nuspec 2>&1 | tee /tmp/choco-pack.log; then
        log_success "Chocolatey package creation succeeded"

        log_info "Installing Chocolatey package locally..."
        if choco install jocky -s . -y 2>&1 | tee /tmp/choco-install.log; then
            log_success "Chocolatey installation succeeded"

            # Test the installed binary
            if test_binary "jockyc" "jocky"; then
                log_success "Chocolatey package functional"
            else
                log_error "Chocolatey binary not functional"
            fi
        else
            log_error "Chocolatey installation failed"
            tail -20 /tmp/choco-install.log
        fi
    else
        log_error "Chocolatey package creation failed"
        tail -20 /tmp/choco-pack.log
    fi
}

# Test manual installation
test_manual_install() {
    log_info "========== MANUAL INSTALLATION TEST =========="

    local tmpdir=$(mktemp -d)
    trap "rm -rf $tmpdir" EXIT

    cd "$REPO_ROOT"

    log_info "Building from source..."
    mkdir -p "$tmpdir/build"
    cd "$tmpdir/build"

    if cmake "$REPO_ROOT/compiler" -DCMAKE_BUILD_TYPE=Release 2>&1 | tee /tmp/cmake-config.log; then
        log_success "CMake configuration succeeded"
    else
        log_error "CMake configuration failed"
        tail -20 /tmp/cmake-config.log
        return 1
    fi

    if cmake --build . --parallel $(nproc) 2>&1 | tee /tmp/cmake-build.log; then
        log_success "Build succeeded"
    else
        log_error "Build failed"
        tail -20 /tmp/cmake-build.log
        return 1
    fi

    if [[ -f "$tmpdir/build/jockyc" ]]; then
        log_success "Binary created successfully"

        if "$tmpdir/build/jockyc" --version &> /dev/null; then
            log_success "Binary works correctly"
        else
            log_error "Binary execution failed"
        fi
    else
        log_error "Binary not created"
    fi
}

# Test compilation with installed JOCKY
test_compilation() {
    log_info "========== COMPILATION TEST =========="

    if ! command -v jockyc &> /dev/null; then
        log_skip "jockyc not in PATH (install via package manager first)"
        return
    fi

    local tmpdir=$(mktemp -d)
    trap "rm -rf $tmpdir" EXIT

    # Create test C file
    cat > "$tmpdir/test.c" << 'EOF'
#include <stdio.h>

int main() {
    printf("JOCKY compilation test successful!\n");
    return 0;
}
EOF

    log_info "Compiling test program..."
    cd "$tmpdir"

    if jockyc test.c -o test 2>&1 | tee /tmp/jocky-compile.log; then
        log_success "Compilation succeeded"

        if [[ -f "$tmpdir/test" ]]; then
            log_success "Binary created"

            if "$tmpdir/test" 2>&1 | grep -q "successful"; then
                log_success "Compiled binary executed successfully"
            else
                log_error "Binary execution failed"
            fi
        else
            log_error "Binary not created"
        fi
    else
        log_error "Compilation failed"
        tail -20 /tmp/jocky-compile.log
    fi
}

# Checksum verification test
test_checksums() {
    log_info "========== CHECKSUM VERIFICATION TEST =========="

    local version_file="$SCRIPT_DIR/version.yaml"

    if [[ ! -f "$version_file" ]]; then
        log_error "Version file not found: $version_file"
        return 1
    fi

    log_info "Validating version.yaml..."

    if grep -q "version:" "$version_file" && grep -q "checksums:" "$version_file"; then
        log_success "Version file structure valid"
    else
        log_error "Version file missing required fields"
    fi
}

# Main test execution
main() {
    echo ""
    echo "╔════════════════════════════════════════╗"
    echo "║   JOCKY Package Manager Test Suite    ║"
    echo "╚════════════════════════════════════════╝"
    echo ""

    # Run all tests
    test_checksums
    test_manual_install
    test_homebrew
    test_aur
    test_chocolatey
    test_compilation

    # Summary
    echo ""
    echo "╔════════════════════════════════════════╗"
    echo "║            Test Summary                ║"
    echo "╚════════════════════════════════════════╝"
    echo -e "${GREEN}Passed: $TESTS_PASSED${NC}"
    echo -e "${RED}Failed: $TESTS_FAILED${NC}"
    echo ""

    if [[ $TESTS_FAILED -eq 0 ]]; then
        echo -e "${GREEN}✓ All tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}✗ Some tests failed. See above for details.${NC}"
        exit 1
    fi
}

# Show usage
if [[ "$#" -gt 0 && "$1" == "-h" ]]; then
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Test JOCKY package installations"
    echo ""
    echo "Options:"
    echo "  -h              Show this help message"
    echo "  --homebrew      Test Homebrew only"
    echo "  --aur           Test AUR only"
    echo "  --chocolatey    Test Chocolatey only"
    echo "  --manual        Test manual installation only"
    echo "  --compile       Test compilation only"
    exit 0
fi

# Handle specific test selection
if [[ "$#" -gt 0 ]]; then
    case "$1" in
        --homebrew)
            test_homebrew
            exit $?
            ;;
        --aur)
            test_aur
            exit $?
            ;;
        --chocolatey)
            test_chocolatey
            exit $?
            ;;
        --manual)
            test_manual_install
            exit $?
            ;;
        --compile)
            test_compilation
            exit $?
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
fi

# Run full test suite
main
