#!/bin/bash

###############################################################################
# JOCKY Toolchain Setup - Auto-Download and Configure
#
# Automatically downloads and configures MinGW if not already present
# Usage: bash scripts/setup_toolchain.sh
###############################################################################

set -e

JOCKY_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TOOLCHAIN="$JOCKY_ROOT/toolchain"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo ""
echo -e "${BLUE}=================================================="
echo "JOCKY Toolchain Setup"
echo -e "==================================================${NC}"
echo ""

# Colors
CYAN='\033[0;36m'

# Check if system MinGW is available
echo -e "${CYAN}[1/3] Checking for system MinGW...${NC}"
if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo -e "${GREEN}✓ System MinGW found${NC}"
    echo "  $(x86_64-w64-mingw32-gcc --version | head -1)"
    echo ""
    echo -e "${GREEN}Toolchain ready - using system MinGW${NC}"
    exit 0
fi

echo -e "${YELLOW}⚠️  System MinGW not found${NC}"
echo ""

# Check if bundled MinGW already exists
echo -e "${CYAN}[2/3] Checking for bundled MinGW...${NC}"
if [ -f "$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-gcc" ]; then
    echo -e "${GREEN}✓ Bundled MinGW already installed${NC}"
    SIZE=$(du -sh "$TOOLCHAIN/mingw" | cut -f1)
    echo "  Location: $TOOLCHAIN/mingw"
    echo "  Size: $SIZE"
    echo ""
    echo -e "${GREEN}Toolchain ready${NC}"
    exit 0
fi

# Try to set up bundled MinGW
echo -e "${CYAN}[3/3] Setting up bundled MinGW...${NC}"

if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo -e "${RED}❌ MinGW is required but not installed${NC}"
    echo ""
    echo "Install MinGW with:"
    echo "  Ubuntu/Debian: sudo apt-get install -y mingw-w64 mingw-w64-tools mingw-w64-x86-64-dev"
    echo "  Fedora:        sudo dnf install -y mingw64-toolchain"
    echo "  Arch:          sudo pacman -S mingw-w64-gcc"
    echo ""
    exit 1
fi

# Set up bundled MinGW by copying system MinGW
echo "Copying MinGW to bundled toolchain..."
bash "$JOCKY_ROOT/scripts/setup_mingw_toolchain.sh"

echo ""
echo -e "${GREEN}✓ Toolchain setup complete${NC}"
