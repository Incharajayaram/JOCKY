#!/bin/bash

###############################################################################
# JOCKY Local Development - All-in-One Master Script
#
# This script:
# 1. Sets up environment (one-time)
# 2. Starts Backend API in a new terminal
# 3. Starts Frontend in a new terminal
# 4. Provides interactive menu for compilation
#
# Usage: bash scripts/dev.sh
###############################################################################

set -e

JOCKY_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Helper functions
print_banner() {
    echo ""
    echo -e "${BLUE}=================================================="
    echo "JOCKY Local Development - All-in-One"
    echo -e "==================================================${NC}"
    echo ""
}

print_section() {
    echo -e "${CYAN}[*] $1${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

# Check dependencies
check_dependencies() {
    print_section "Checking dependencies..."

    local missing=0

    if ! command -v python3 &> /dev/null; then
        print_error "Python 3 not found"
        missing=1
    else
        print_success "Python $(python3 --version | cut -d' ' -f2)"
    fi

    if ! command -v node &> /dev/null; then
        print_error "Node.js not found"
        missing=1
    else
        print_success "Node $(node --version)"
    fi

    if ! command -v npm &> /dev/null; then
        print_error "npm not found"
        missing=1
    else
        print_success "npm $(npm --version)"
    fi

    if ! command -v gcc &> /dev/null; then
        print_error "GCC not found"
        missing=1
    else
        print_success "GCC $(gcc --version | head -1 | cut -d' ' -f3-)"
    fi

    if [ ! -f "$JOCKY_ROOT/toolchain/bin/clang" ]; then
        print_error "Toolchain not found at $JOCKY_ROOT/toolchain/bin/clang"
        missing=1
    else
        print_success "Toolchain ready"
    fi

    if [ $missing -eq 1 ]; then
        print_error "Missing dependencies. Install manually or use Docker."
        exit 1
    fi
}

# Setup Python environment
setup_backend() {
    print_section "Setting up backend..."

    cd "$JOCKY_ROOT/web/backend"

    if [ ! -d "venv" ]; then
        print_warning "Creating Python virtual environment..."
        python3 -m venv venv
    fi

    if [ ! -f "venv/bin/activate" ]; then
        print_error "Failed to create virtual environment"
        exit 1
    fi

    source venv/bin/activate

    # Check if dependencies are installed
    if ! python3 -c "import fastapi" 2>/dev/null; then
        print_warning "Installing Python dependencies..."
        pip install -q --upgrade pip
        pip install -q -r requirements.txt
        pip install -q uvicorn[standard] python-multipart websockets
    fi

    deactivate
    print_success "Backend ready"
}

# Setup Node environment
setup_frontend() {
    print_section "Setting up frontend..."

    cd "$JOCKY_ROOT/web/frontend"

    if [ ! -d "node_modules" ]; then
        print_warning "Installing npm dependencies (this may take a minute)..."
        npm install --silent
    fi

    print_success "Frontend ready"
}

# Start backend in new terminal
start_backend() {
    print_section "Starting backend in new terminal..."

    local backend_script="
cd '$JOCKY_ROOT/web/backend'
source venv/bin/activate
export PYTHONPATH='$JOCKY_ROOT/src'
export TOOLCHAIN_PATH='$JOCKY_ROOT/toolchain'
echo ''
echo '=========================================='
echo 'JOCKY Backend API'
echo '=========================================='
echo ''
echo '📡 Backend:   http://localhost:8000'
echo '📚 API Docs:  http://localhost:8000/docs'
echo '🏥 Health:    http://localhost:8000/api/config'
echo ''
echo 'Press Ctrl+C to stop backend'
echo '=========================================='
echo ''
python3 -m uvicorn app:app --host 0.0.0.0 --port 8000 --reload
"

    if command -v gnome-terminal &> /dev/null; then
        gnome-terminal -- bash -c "$backend_script"
    elif command -v xterm &> /dev/null; then
        xterm -e bash -c "$backend_script" &
    elif command -v konsole &> /dev/null; then
        konsole -e bash -c "$backend_script" &
    else
        print_warning "Could not find terminal emulator. Starting backend in background..."
        bash -c "$backend_script" &
    fi

    sleep 3
    print_success "Backend started (wait for 'Uvicorn running' message)"
}

# Start frontend in new terminal
start_frontend() {
    print_section "Starting frontend in new terminal..."

    local frontend_script="
cd '$JOCKY_ROOT/web/frontend'
echo ''
echo '=========================================='
echo 'JOCKY Frontend Dev Server'
echo '=========================================='
echo ''
echo '🎨 Frontend:  http://localhost:5173'
echo ''
echo 'Press Ctrl+C to stop frontend'
echo '=========================================='
echo ''
npm run dev
"

    if command -v gnome-terminal &> /dev/null; then
        gnome-terminal -- bash -c "$frontend_script"
    elif command -v xterm &> /dev/null; then
        xterm -e bash -c "$frontend_script" &
    elif command -v konsole &> /dev/null; then
        konsole -e bash -c "$frontend_script" &
    else
        print_warning "Could not find terminal emulator. Starting frontend in background..."
        bash -c "$frontend_script" &
    fi

    sleep 3
    print_success "Frontend started (wait for 'Local: http://localhost:5173' message)"
}

# Interactive menu
show_menu() {
    echo ""
    echo -e "${CYAN}=========================================="
    echo "JOCKY Compilation Menu"
    echo "==========================================${NC}"
    echo ""
    echo "1) Compile Windows PE binary (standard obfuscation)"
    echo "2) Compile Linux ELF binary (standard obfuscation)"
    echo "3) Compile Windows PE (aggressive obfuscation)"
    echo "4) Compile Linux ELF (aggressive obfuscation)"
    echo "5) Custom compilation"
    echo "6) Show quick reference"
    echo "7) Exit"
    echo ""
    echo -n "Select option (1-7): "
}

# Compile Windows
compile_windows() {
    local preset="${1:-standard}"
    echo ""
    print_section "Compiling Windows PE binary..."
    bash "$JOCKY_ROOT/scripts/compile.sh" \
        "$JOCKY_ROOT/examples/research_chain_windows_production.jky" \
        "$JOCKY_ROOT/build/windows_$preset" \
        --platform windows \
        --preset "$preset"
}

# Compile Linux
compile_linux() {
    local preset="${1:-standard}"
    echo ""
    print_section "Compiling Linux ELF binary..."
    bash "$JOCKY_ROOT/scripts/compile.sh" \
        "$JOCKY_ROOT/examples/research_chain_linux_production.jky" \
        "$JOCKY_ROOT/build/linux_$preset" \
        --platform linux \
        --preset "$preset"
}

# Custom compilation
compile_custom() {
    echo ""
    echo -n "Source file (default: examples/research_chain_windows_production.jky): "
    read -r source
    source="${source:-examples/research_chain_windows_production.jky}"

    echo -n "Output directory (default: build/custom): "
    read -r output
    output="${output:-build/custom}"

    echo -n "Platform (windows/linux, default: windows): "
    read -r platform
    platform="${platform:-windows}"

    echo -n "Preset (none/light/standard/aggressive, default: standard): "
    read -r preset
    preset="${preset:-standard}"

    echo ""
    print_section "Compiling custom..."
    bash "$JOCKY_ROOT/scripts/compile.sh" "$source" "$output" --platform "$platform" --preset "$preset"
}

# Quick reference
show_reference() {
    echo ""
    echo -e "${CYAN}=========================================="
    echo "Quick Reference"
    echo "==========================================${NC}"
    echo ""
    echo "🎨 Frontend:  http://localhost:5173"
    echo "📡 Backend:   http://localhost:8000"
    echo "📚 API Docs:  http://localhost:8000/docs"
    echo ""
    echo -e "${YELLOW}CLI Compilation (in this menu or separate terminal):${NC}"
    echo ""
    echo "  Windows PE:"
    echo "    bash scripts/compile.sh examples/research_chain_windows_production.jky build/windows --platform windows"
    echo ""
    echo "  Linux ELF:"
    echo "    bash scripts/compile.sh examples/research_chain_linux_production.jky build/linux --platform linux"
    echo ""
    echo -e "${YELLOW}Presets:${NC}"
    echo "  none       - No obfuscation (fastest)"
    echo "  light      - Minimal obfuscation"
    echo "  standard   - Default obfuscation (recommended)"
    echo "  aggressive - All 16 obfuscation passes"
    echo ""
    echo -e "${YELLOW}Output location:${NC}"
    echo "  build/windows_*    - Windows PE binaries"
    echo "  build/linux_*      - Linux ELF binaries"
    echo ""
    echo -e "${YELLOW}Analyze output:${NC}"
    echo "  file build/windows_standard/*.exe"
    echo "  strings build/linux_standard/research_chain_linux_production"
    echo ""
}

# Main execution
main() {
    print_banner

    print_section "Initializing..."
    check_dependencies
    setup_backend
    setup_frontend

    print_banner
    print_section "Launching services..."
    start_backend
    start_frontend

    print_banner
    echo -e "${GREEN}✓ Services launched!${NC}"
    echo ""
    echo -e "${CYAN}Open in browser:${NC}"
    echo "  🎨 Frontend: http://localhost:5173"
    echo "  📡 Backend:  http://localhost:8000/docs"
    echo ""
    echo -e "${CYAN}Use menu below for compilation, or press Ctrl+C to exit${NC}"

    # Interactive menu loop
    while true; do
        show_menu
        read -r choice

        case $choice in
            1) compile_windows "standard" ;;
            2) compile_linux "standard" ;;
            3) compile_windows "aggressive" ;;
            4) compile_linux "aggressive" ;;
            5) compile_custom ;;
            6) show_reference ;;
            7)
                echo ""
                print_section "Shutting down..."
                killall -q gnome-terminal xterm konsole 2>/dev/null || true
                echo -e "${GREEN}Goodbye!${NC}"
                exit 0
                ;;
            *) print_error "Invalid option" ;;
        esac

        echo ""
        echo -n "Press Enter to continue..."
        read -r
    done
}

# Run main function
main "$@"
