#!/bin/bash

# JOCKY Local Compiler - Cross-compile Windows PE and Linux ELF
# Usage:
#   ./scripts/compile.sh examples/research_chain_windows_production.jky build/windows windows
#   ./scripts/compile.sh examples/research_chain_linux_production.jky build/linux linux
#   ./scripts/compile.sh source.jky output/ --platform windows --preset aggressive

set -e

JOCKY_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export PYTHONPATH="$JOCKY_ROOT/src"
export PATH="$JOCKY_ROOT/toolchain/bin:$PATH"
export LD_LIBRARY_PATH="$JOCKY_ROOT/toolchain/lib:$LD_LIBRARY_PATH"

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Parse arguments
if [ $# -lt 2 ]; then
    echo -e "${RED}Usage: $0 <source.jky> <output_dir> [--platform windows|linux] [--preset none|light|standard|aggressive]${NC}"
    echo ""
    echo "Examples:"
    echo "  $0 examples/research_chain_windows_production.jky build/windows --platform windows"
    echo "  $0 examples/research_chain_linux_production.jky build/linux --platform linux"
    echo "  $0 examples/research_chain_windows_production.jky build/aggressive --preset aggressive"
    echo ""
    exit 1
fi

SOURCE="$1"
OUTPUT_DIR="$2"
PLATFORM="windows"  # default
PRESET="standard"   # default

# Parse optional arguments
shift 2
while [[ $# -gt 0 ]]; do
    case $1 in
        --platform)
            PLATFORM="$2"
            shift 2
            ;;
        --preset)
            PRESET="$2"
            shift 2
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

# Validate inputs
if [ ! -f "$SOURCE" ]; then
    echo -e "${RED}❌ Source file not found: $SOURCE${NC}"
    exit 1
fi

if [ "$PLATFORM" != "windows" ] && [ "$PLATFORM" != "linux" ]; then
    echo -e "${RED}❌ Platform must be 'windows' or 'linux'${NC}"
    exit 1
fi

if [ "$PRESET" != "none" ] && [ "$PRESET" != "light" ] && [ "$PRESET" != "standard" ] && [ "$PRESET" != "aggressive" ]; then
    echo -e "${RED}❌ Preset must be 'none', 'light', 'standard', or 'aggressive'${NC}"
    exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Display header
echo ""
echo "=================================================="
echo -e "${BLUE}JOCKY Local Compilation${NC}"
echo "=================================================="
echo -e "${GREEN}Source:     $SOURCE${NC}"
echo -e "${GREEN}Output:     $OUTPUT_DIR${NC}"
echo -e "${GREEN}Platform:   $PLATFORM${NC}"
echo -e "${GREEN}Preset:     $PRESET${NC}"
echo ""
echo -e "${BLUE}Toolchain:${NC}"
echo "  Python:   $(python3 --version)"
echo "  Clang:    $($JOCKY_ROOT/toolchain/bin/clang --version | head -1)"
if [ "$PLATFORM" = "windows" ]; then
    echo "  MinGW:    $(x86_64-w64-mingw32-gcc --version | head -1)"
fi
echo ""

# Run compilation
echo -e "${BLUE}Starting compilation...${NC}"
echo ""

python3 "$JOCKY_ROOT/scripts/compile_pipeline.py" \
    "$SOURCE" \
    "$OUTPUT_DIR" \
    --platform "$PLATFORM" \
    --preset "$PRESET"

# Check output
if [ "$PLATFORM" = "windows" ]; then
    OUTPUT_FILE="$OUTPUT_DIR/$(basename "${SOURCE%.*}").exe"
else
    OUTPUT_FILE="$OUTPUT_DIR/$(basename "${SOURCE%.*}")"
fi

echo ""
if [ -f "$OUTPUT_FILE" ]; then
    SIZE=$(du -h "$OUTPUT_FILE" | cut -f1)
    echo -e "${GREEN}✓ Compilation successful!${NC}"
    echo ""
    echo "Output binary: $OUTPUT_FILE ($SIZE)"
    echo ""

    # Show file type
    if command -v file &> /dev/null; then
        echo "File type: $(file "$OUTPUT_FILE")"
    fi

    echo ""
    echo -e "${BLUE}Next steps:${NC}"
    if [ "$PLATFORM" = "windows" ]; then
        echo "  - Transfer to Windows system"
        echo "  - Run: $OUTPUT_FILE"
        echo "  - Analyze with: Ghidra, IDA Pro, x64dbg"
    else
        echo "  - Test on Linux: ./$OUTPUT_FILE"
        echo "  - Analyze with: Ghidra, radare2, objdump"
        echo "  - Check: file $OUTPUT_FILE"
    fi
else
    echo -e "${RED}❌ Compilation failed - no output binary found${NC}"
    exit 1
fi

echo ""
echo "=================================================="
echo ""
