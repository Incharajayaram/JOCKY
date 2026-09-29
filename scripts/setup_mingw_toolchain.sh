#!/bin/bash

###############################################################################
# Setup MinGW in JOCKY Toolchain
#
# Copies MinGW cross-compiler and libraries into toolchain directory
# This makes the toolchain self-contained for Windows PE compilation
#
# Usage: bash scripts/setup_mingw_toolchain.sh
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
echo "Setting up MinGW in JOCKY Toolchain"
echo -e "==================================================${NC}"
echo ""

# Check if MinGW is installed
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo -e "${RED}❌ MinGW not found! Installing...${NC}"
    echo ""
    echo "Run: sudo apt-get install -y mingw-w64 mingw-w64-tools mingw-w64-x86-64-dev"
    echo ""
    exit 1
fi

echo -e "${GREEN}✓ MinGW found${NC}"
echo "  $(x86_64-w64-mingw32-gcc --version | head -1)"
echo ""

# Create MinGW directories in toolchain
echo -e "${BLUE}Creating directory structure...${NC}"
mkdir -p "$TOOLCHAIN/mingw/bin"
mkdir -p "$TOOLCHAIN/mingw/lib"
mkdir -p "$TOOLCHAIN/mingw/include"
mkdir -p "$TOOLCHAIN/mingw/x86_64-w64-mingw32/lib"
mkdir -p "$TOOLCHAIN/mingw/x86_64-w64-mingw32/include"

# Copy MinGW binaries
echo -e "${BLUE}Copying MinGW binaries...${NC}"
for bin in gcc g++ ar as ld ranlib strip objcopy objdump; do
    if [ -f "/usr/bin/x86_64-w64-mingw32-$bin" ]; then
        cp "/usr/bin/x86_64-w64-mingw32-$bin" "$TOOLCHAIN/mingw/bin/"
        echo "  ✓ $bin"
    fi
done

# Copy MinGW libraries and headers
echo -e "${BLUE}Copying MinGW libraries...${NC}"
if [ -d "/usr/x86_64-w64-mingw32/lib" ]; then
    cp -r /usr/x86_64-w64-mingw32/lib/* "$TOOLCHAIN/mingw/x86_64-w64-mingw32/lib/"
    echo "  ✓ Libraries copied"
fi

echo -e "${BLUE}Copying MinGW headers...${NC}"
if [ -d "/usr/x86_64-w64-mingw32/include" ]; then
    cp -r /usr/x86_64-w64-mingw32/include/* "$TOOLCHAIN/mingw/x86_64-w64-mingw32/include/"
    echo "  ✓ Headers copied"
fi

# Create symlinks with simple names for easy access
echo -e "${BLUE}Creating convenience symlinks...${NC}"
ln -sf "$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-gcc" "$TOOLCHAIN/mingw/bin/gcc" 2>/dev/null || true
ln -sf "$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-g++" "$TOOLCHAIN/mingw/bin/g++" 2>/dev/null || true
ln -sf "$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-ar" "$TOOLCHAIN/mingw/bin/ar" 2>/dev/null || true
ln -sf "$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-ld" "$TOOLCHAIN/mingw/bin/ld" 2>/dev/null || true

# Also link in toolchain/bin for consistency
ln -sf "$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-gcc" "$TOOLCHAIN/bin/mingw-gcc" 2>/dev/null || true
ln -sf "$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-g++" "$TOOLCHAIN/bin/mingw-g++" 2>/dev/null || true
echo "  ✓ Symlinks created"

# Verify
echo ""
echo -e "${BLUE}Verifying installation...${NC}"
if [ -f "$TOOLCHAIN/mingw/bin/gcc" ]; then
    SIZE=$(du -sh "$TOOLCHAIN/mingw" | cut -f1)
    echo -e "${GREEN}✓ MinGW successfully installed in toolchain${NC}"
    echo "  Location: $TOOLCHAIN/mingw"
    echo "  Size: $SIZE"
else
    echo -e "${RED}❌ MinGW installation failed${NC}"
    exit 1
fi

echo ""
echo -e "${BLUE}=================================================="
echo -e "${GREEN}✓ MinGW Toolchain Setup Complete!${NC}"
echo -e "==================================================${NC}"
echo ""
echo "Windows PE compilation will now work without system MinGW"
echo ""
echo "Test:"
echo "  bash scripts/compile.sh examples/research_chain_windows_production.jky build/windows --platform windows"
echo ""
