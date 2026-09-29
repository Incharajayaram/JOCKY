#!/bin/bash
# JOCKY Build Validation Script
# Runs all critical tests and verifies the pipeline works end-to-end

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="/tmp/jocky_validate_build"
LOG_FILE="${BUILD_DIR}/validate.log"

mkdir -p "$BUILD_DIR"

log() {
    echo -e "${GREEN}[$(date +'%H:%M:%S')]${NC} $1" | tee -a "$LOG_FILE"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1" | tee -a "$LOG_FILE"
    exit 1
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1" | tee -a "$LOG_FILE"
}

log "========================================="
log "JOCKY Build Validation"
log "========================================="
log "Project: $PROJECT_ROOT"
log "Build Dir: $BUILD_DIR"
log ""

# Step 1: Verify Python modules
log "Step 1: Verifying Python modules..."
python3 -c "
import sys
sys.path.insert(0, '$PROJECT_ROOT/src')
from jocky.language.lexer import Lexer
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker
from jocky.language.codegen import CodeGen
print('✓ All modules load')
" || error "Python modules failed to load"

# Step 2: Verify prelude.jky
log "Step 2: Verifying prelude.jky..."
if python3 << EOF
import sys
sys.path.insert(0, '$PROJECT_ROOT/src')
from jocky.language.lexer import Lexer
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker

with open('$PROJECT_ROOT/src/jocky/stdlib/prelude.jky') as f:
    prelude = f.read()
with open('$PROJECT_ROOT/examples/simple_test.jky') as f:
    src = f.read()

full_src = prelude + '\n' + src
lexer = Lexer(full_src)
tokens = lexer.tokenize()
parser = Parser(tokens)
ast = parser.parse()
checker = TypeChecker()
checker.check(ast)
print('✓ prelude.jky parses and type-checks correctly')
EOF
then
    log "✓ Prelude validation passed"
else
    error "prelude.jky validation failed"
fi

# Step 3: Check Docker image exists
log "Step 3: Checking Docker image..."
if docker image inspect jocky-compiler:latest > /dev/null 2>&1; then
    log "✓ Docker image exists: jocky-compiler:latest"
else
    warn "Docker image not found, building..."
    cd "$PROJECT_ROOT"
    docker build -t jocky-compiler:latest . || error "Docker build failed"
fi

# Step 4: Test compilation pipeline
log "Step 4: Testing compilation pipeline..."
cd "$PROJECT_ROOT"
docker run --rm \
  -v "$PROJECT_ROOT:/workspace/jocky" \
  jocky-compiler:latest \
  python3 /workspace/jocky/scripts/compile_pipeline.py \
  examples/simple_test.jky /workspace/jocky/build 2>&1 | tee -a "$LOG_FILE" | tail -20

if [ -f "$PROJECT_ROOT/build/simple_test.exe" ]; then
    SIZE=$(ls -lh "$PROJECT_ROOT/build/simple_test.exe" | awk '{print $5}')
    log "✓ Compilation successful: simple_test.exe ($SIZE)"
else
    error "Compilation failed - no executable generated"
fi

# Step 5: Verify executable format
log "Step 5: Verifying executable format..."
FILE_TYPE=$(file "$PROJECT_ROOT/build/simple_test.exe")
if echo "$FILE_TYPE" | grep -q "PE32+ executable"; then
    log "✓ Valid PE32+ Windows executable"
else
    error "Invalid executable format: $FILE_TYPE"
fi

# Step 6: Test research_chain_complete
log "Step 6: Testing research_chain_complete.jky..."
docker run --rm \
  -v "$PROJECT_ROOT:/workspace/jocky" \
  jocky-compiler:latest \
  python3 /workspace/jocky/scripts/compile_pipeline.py \
  examples/research_chain_complete.jky /workspace/jocky/build 2>&1 | tee -a "$LOG_FILE" | grep -E "BUILD|OUTPUT|error|FAIL"

if [ -f "$PROJECT_ROOT/build/research_chain_complete.exe" ]; then
    SIZE=$(ls -lh "$PROJECT_ROOT/build/research_chain_complete.exe" | awk '{print $5}')
    log "✓ Research binary compiled: research_chain_complete.exe ($SIZE)"
else
    error "Research binary compilation failed"
fi

# Summary
log ""
log "========================================="
log "✓ ALL VALIDATION TESTS PASSED"
log "========================================="
log "Ready for deployment:"
log "  - simple_test.exe: $PROJECT_ROOT/build/simple_test.exe"
log "  - research_chain_complete.exe: $PROJECT_ROOT/build/research_chain_complete.exe"
log "  - Log: $LOG_FILE"
log ""

exit 0
