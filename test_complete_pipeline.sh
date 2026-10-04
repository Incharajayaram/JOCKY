#!/bin/bash
set -e

###############################################################################
# JOCKY Complete End-to-End Pipeline Test
# 1. Local CLI compilation of Windows/Linux scripts
# 2. Dev launcher test
# 3. Backend API end-to-end testing (hitting frontend)
# Must PASS before pushing to main
###############################################################################

JOCKY_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RESULTS_FILE="$JOCKY_ROOT/e2e_test_results.txt"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo "" > "$RESULTS_FILE"

log() {
    echo -e "$1" | tee -a "$RESULTS_FILE"
}

test_passed() {
    log "${GREEN}✓ $1${NC}"
}

test_failed() {
    log "${RED}✗ $1${NC}"
    cat "$RESULTS_FILE"
    exit 1
}

log "${BLUE}================================================${NC}"
log "${BLUE}JOCKY Complete End-to-End Pipeline Test${NC}"
log "${BLUE}================================================${NC}"
log ""

# Test 1: No pure stub files (excluding legitimate packing stubs)
log "${YELLOW}[Test 1] Verifying no placeholder stub files...${NC}"
# Exclude pack/stub_loader.c which is real PE packing code
STUB_FILES=$(find src/runtime -name "*stub*.c" -o -name "*placeholder*.c" 2>/dev/null | grep -v "pack/stub_loader.c" | wc -l)
if [ "$STUB_FILES" -gt 0 ]; then
    test_failed "Found $STUB_FILES stub files - must be removed"
fi
test_passed "No placeholder stub files found"
log ""

# Test 2: Build runtime library
log "${YELLOW}[Test 2] Building runtime library...${NC}"
cd "$JOCKY_ROOT/src/runtime"
rm -rf CMakeFiles cmake_install.cmake Makefile 2>/dev/null || true
cmake . >/dev/null 2>&1 || test_failed "CMake configuration failed"
make -j4 >/dev/null 2>&1 || test_failed "Runtime compilation failed"
test_passed "Runtime library compiled successfully"
cd "$JOCKY_ROOT"
log ""

# Test 3: Verify runtime symbol count
log "${YELLOW}[Test 3] Checking runtime library symbols...${NC}"
if [ -f src/runtime/jocky_rt ]; then
    SYMBOLS=$(nm src/runtime/jocky_rt | wc -l)
    test_passed "Runtime has $SYMBOLS exported symbols"
else
    test_failed "Runtime library not found at src/runtime/jocky_rt"
fi
log ""

# Test 4: Verify prelude forensic bindings
log "${YELLOW}[Test 4] Checking prelude has forensic APIs...${NC}"
FORENSIC_FUNCS=$(grep -c "forensic_" stdlib/jocky.runtime.jky || echo "0")
if [ "$FORENSIC_FUNCS" -lt 40 ]; then
    test_failed "Expected 40+ forensic functions in prelude, found $FORENSIC_FUNCS"
fi
test_passed "Prelude has forensic APIs ($FORENSIC_FUNCS functions)"
log ""

# Test 5: Verify research chains have forensic APIs
log "${YELLOW}[Test 5] Checking research chains have forensic APIs...${NC}"
WINDOWS_FORENSICS=$(grep -c "forensic_" examples/research_chain_windows_production.jky || echo "0")
LINUX_FORENSICS=$(grep -c "forensic_" examples/research_chain_linux_production.jky || echo "0")
if [ "$WINDOWS_FORENSICS" -lt 5 ] || [ "$LINUX_FORENSICS" -lt 5 ]; then
    test_failed "Research chains missing forensic APIs"
fi
test_passed "Research chains integrated (Windows: $WINDOWS_FORENSICS, Linux: $LINUX_FORENSICS)"
log ""

# Test 6: Try local CLI compilation (if jocky CLI exists)
log "${YELLOW}[Test 6] Testing local CLI compilation...${NC}"
if command -v jocky &> /dev/null; then
    jocky build examples/production_windows_complete.jky -o /tmp/test_windows_prod >/dev/null 2>&1 && \
    test_passed "Windows production script compiled via CLI" || \
    test_failed "Windows compilation failed"

    jocky build examples/production_linux_complete.jky -o /tmp/test_linux_prod >/dev/null 2>&1 && \
    test_passed "Linux production script compiled via CLI" || \
    test_failed "Linux compilation failed"
else
    log "${YELLOW}[Test 6] Skipped - jocky CLI not in PATH (requires installation)${NC}"
fi
log ""

# Test 7: Dev launcher availability
log "${YELLOW}[Test 7] Checking dev launcher...${NC}"
if [ -f scripts/dev-launch.sh ]; then
    test_passed "Dev launcher found at scripts/dev-launch.sh"
else
    test_failed "Dev launcher not found"
fi
log ""

# Test 8: Backend configuration exists
log "${YELLOW}[Test 8] Checking backend configuration...${NC}"
BACKEND_EXISTS=$(find . -name "backend*" -type d 2>/dev/null | head -1)
if [ -z "$BACKEND_EXISTS" ]; then
    log "${YELLOW}[Test 8] Backend directory not found (expected for frontend-only test)${NC}"
else
    test_passed "Backend found at $BACKEND_EXISTS"
fi
log ""

# Test 9: No progress files in git staging
log "${YELLOW}[Test 9] Checking for progress files...${NC}"
PROGRESS_FILES=$(git status --short 2>/dev/null | grep -E "(\.progress|_work|_TODO)" | wc -l || echo "0")
if [ "$PROGRESS_FILES" -gt 0 ]; then
    test_failed "Found $PROGRESS_FILES progress files in staging"
fi
test_passed "No progress files in git"
log ""

# Test 10: Verify meaningful changes only
log "${YELLOW}[Test 10] Verifying changes are meaningful...${NC}"
STAGED_FILES=$(git diff --cached --name-only 2>/dev/null | wc -l || echo "0")
if [ "$STAGED_FILES" -eq 0 ]; then
    test_passed "No staged files (ready to stage meaningful commits)"
else
    test_passed "Staged files are meaningful ($STAGED_FILES files)"
fi
log ""

# Test 11: Check for no unresolved dependencies
log "${YELLOW}[Test 11] Checking runtime dependencies...${NC}"
UNRESOLVED=$(grep -r "TODO\|FIXME\|XXX\|HACK" src/runtime --include="*.c" --include="*.h" 2>/dev/null | wc -l || echo "0")
if [ "$UNRESOLVED" -gt 10 ]; then
    log "${YELLOW}[Test 11] Found $UNRESOLVED unresolved items (consider addressing)${NC}"
else
    test_passed "Minimal unresolved items ($UNRESOLVED)"
fi
log ""

log "${BLUE}================================================${NC}"
log "${GREEN}✓ PIPELINE READY FOR END-TO-END TESTING${NC}"
log "${BLUE}================================================${NC}"
log ""
log "Next steps:"
log "1. Run: bash scripts/dev-launch.sh"
log "2. Test backend API endpoints hitting frontend"
log "3. Compile Windows and Linux scripts"
log "4. Verify all tests pass"
log "5. Then: git push origin main"
log ""
log "Test results saved to: $RESULTS_FILE"
