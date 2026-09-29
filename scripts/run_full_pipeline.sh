#!/bin/bash
# JOCKY Full Pipeline - Compile + Analyze
# Run inside Docker container
set -e

echo "============================================"
echo " JOCKY Full Pipeline"
echo " Target: Windows x86_64"
echo "============================================"
echo ""

SOURCE="${1:-examples/research_chain_complete.jky}"
BUILD_DIR="${2:-/workspace/build}"
OUTPUT_DIR="${3:-/workspace/output}"

mkdir -p "$BUILD_DIR" "$OUTPUT_DIR"

echo "[*] Step 1: Compile Pipeline"
echo "-------------------------------------------"
python3 scripts/compile_pipeline.py "$SOURCE" "$BUILD_DIR"
echo ""

STEM=$(basename "$SOURCE" .jky)
EXE="$BUILD_DIR/${STEM}.exe"

if [ -f "$EXE" ]; then
    cp "$EXE" "$OUTPUT_DIR/"
    echo "[*] Step 2: Binary Analysis"
    echo "-------------------------------------------"
    python3 scripts/analyze_binary.py "$EXE"
    echo ""
    echo "[*] Output: $OUTPUT_DIR/${STEM}.exe"
else
    echo "[!] Executable not found at $EXE"
    echo "[*] Checking build artifacts..."
    ls -la "$BUILD_DIR"/
fi

echo ""
echo "============================================"
echo " Pipeline Complete"
echo "============================================"
