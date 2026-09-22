#!/usr/bin/env bash
# Script to compile JOCKY source files with all obfuscation passes enabled for both Linux and Windows target platforms.

set -e

SRC_FILE="${1:-test.jky}"
PROFILE="${2:-paranoid}"

if [ ! -f "$SRC_FILE" ]; then
    echo "Error: Source file '$SRC_FILE' not found."
    echo "Usage: ./compile_dual_target.sh <path-to-jky-file> [profile]"
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export PYTHONPATH="$SCRIPT_DIR/src:/home/incharanew/.cache/uv/archive-v0/GJ4lKSTowvWareTcDs1-J:$PYTHONPATH"
export JOCKY_LLVM_TOOLCHAIN="${JOCKY_LLVM_TOOLCHAIN:-$SCRIPT_DIR/toolchain}"

echo "=================================================="
echo "Compiling $SRC_FILE with profile '$PROFILE' for Linux & Windows..."
echo "=================================================="

python3 -m jocky build "$SRC_FILE" --profile "$PROFILE" --target both --keep-intermediates

echo ""
echo "=================================================="
echo "Build complete!"
echo "Generated binaries in .jocky-build/:"
file "$SCRIPT_DIR/.jocky-build/"* | grep -E "(ELF|PE32)" || true
echo "=================================================="
