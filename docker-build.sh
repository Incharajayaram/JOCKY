#!/bin/bash
# JOCKY Automated Docker Build Pipeline
# Orchestrates: parsing, MLIR obfuscation, LLVM obfuscation, compilation, linking, packaging
# Supports: Linux ELF + Windows PE cross-compilation in one command

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
JOCKY_SOURCE="${1:-.}"
JOCKY_OUTPUT="${2:-./dist}"
JOCKY_PROFILE="${JOCKY_PROFILE:-standard}"
JOCKY_TARGETS="${JOCKY_TARGETS:-linux,windows}"
JOCKY_KEEP_INTERMEDIATES="${JOCKY_KEEP_INTERMEDIATES:-false}"

# Derived paths
BUILD_DIR="./build"
CACHE_DIR="${HOME}/.jocky/build-cache"

echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}JOCKY Automated Docker Pipeline${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo ""
echo -e "Source:        ${YELLOW}${JOCKY_SOURCE}${NC}"
echo -e "Output:        ${YELLOW}${JOCKY_OUTPUT}${NC}"
echo -e "Profile:       ${YELLOW}${JOCKY_PROFILE}${NC}"
echo -e "Targets:       ${YELLOW}${JOCKY_TARGETS}${NC}"
echo -e "Keep Int:      ${YELLOW}${JOCKY_KEEP_INTERMEDIATES}${NC}"
echo ""

# Ensure output directory exists
mkdir -p "${JOCKY_OUTPUT}"
mkdir -p "${CACHE_DIR}"

# Function to run a pipeline stage
run_stage() {
    local stage_name="$1"
    local stage_cmd="$2"

    echo -e "${BLUE}▸${NC} ${stage_name}..."
    if eval "${stage_cmd}"; then
        echo -e "${GREEN}✓${NC} ${stage_name} completed"
    else
        echo -e "${RED}✗${NC} ${stage_name} failed"
        exit 1
    fi
    echo ""
}

# Stage 1: Validate JOCKY source
run_stage "Validating JOCKY source" \
    "test -f '${JOCKY_SOURCE}' || (test -d '${JOCKY_SOURCE}' && test -f '${JOCKY_SOURCE}/main.jky')"

# Stage 2: Parse and type-check
run_stage "Parsing JOCKY source" \
    "python3 -m jocky verify '${JOCKY_SOURCE}' 2>&1 | tee '${BUILD_DIR}/parse.log'"

# Stage 3-4: Compile for each target
for TARGET in $(echo ${JOCKY_TARGETS} | tr ',' ' '); do
    echo -e "${BLUE}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║ Building for: ${TARGET}${NC}"
    echo -e "${BLUE}╚════════════════════════════════════════════════════════╝${NC}"
    echo ""

    # Determine output filename and extension
    if [ "$TARGET" = "windows" ]; then
        OUTPUT_EXE="${JOCKY_OUTPUT}/$(basename ${JOCKY_SOURCE%.*}).exe"
    else
        OUTPUT_EXE="${JOCKY_OUTPUT}/$(basename ${JOCKY_SOURCE%.*})"
    fi

    # Run compilation with full pipeline
    run_stage "Building ${TARGET} target" \
        "python3 -m jocky build '${JOCKY_SOURCE}' \
            --target ${TARGET} \
            --output '${OUTPUT_EXE}' \
            --profile '${JOCKY_PROFILE}' \
            --keep-intermediates"

    # Verify output exists
    if [ -f "${OUTPUT_EXE}" ]; then
        echo -e "${GREEN}✓${NC} Output: ${OUTPUT_EXE}"
        ls -lh "${OUTPUT_EXE}"

        # Get binary info
        if [ "$TARGET" = "linux" ]; then
            echo -e "  Type: $(file ${OUTPUT_EXE} | cut -d: -f2-)"
            echo -e "  Size: $(du -h ${OUTPUT_EXE} | cut -f1)"
            echo -e "  Checksum: $(md5sum ${OUTPUT_EXE} | cut -d' ' -f1)"
        fi
    else
        echo -e "${RED}✗${NC} Output file not found: ${OUTPUT_EXE}"
        exit 1
    fi
    echo ""
done

# Stage 5: Generate build report
run_stage "Generating build report" \
    "cat > '${JOCKY_OUTPUT}/BUILD_REPORT.txt' << 'EOF'
JOCKY Automated Build Report
═════════════════════════════════════════════════════════════

Build Time: $(date)
Source: ${JOCKY_SOURCE}
Profile: ${JOCKY_PROFILE}
Targets: ${JOCKY_TARGETS}

Pipeline Stages Completed:
  ✓ ParseStage - Syntax and type validation
  ✓ LowerIRStage - MLIR lowering
  ✓ MLIRObfuscateStage - MLIR-level transforms
    - memref.expand_strided_metadata
    - affine-loop-invariant-code-motion
    - affine.pipeline
  ✓ IRObfuscateStage - LLVM IR obfuscation passes
    - boguscf (bogus control flow)
    - flattening (control flow flattening)
    - substitution (variable substitution)
    - linear-mba (linear math/boolean algebra)
    - opaque-pred (opaque predicates)
  ✓ LinkStage - Runtime library linking
    - jocky_rt (kernel APIs)
    - jocky_crypto (encryption)
    - jocky_exfil (exfiltration)
    - jocky_evasion (anti-analysis)
  ✓ PackStage - Executable packing/optimization

Outputs:
EOF
for target in $(echo ${JOCKY_TARGETS} | tr ',' ' '); do
    if [ \"$target\" = \"windows\" ]; then
        output=\"\${JOCKY_OUTPUT}/\$(basename ${JOCKY_SOURCE%.*}).exe\"
    else
        output=\"\${JOCKY_OUTPUT}/\$(basename ${JOCKY_SOURCE%.*})\"
    fi
    if [ -f \"\${output}\" ]; then
        echo \"  ${target}: \$(ls -lh \${output} | awk '{print \$5}') - \$(file \${output} | cut -d: -f2-)\" >> '${JOCKY_OUTPUT}/BUILD_REPORT.txt'
    fi
done"

echo -e "${GREEN}═══════════════════════════════════════════════════════════${NC}"
echo -e "${GREEN}Build Complete!${NC}"
echo -e "${GREEN}═══════════════════════════════════════════════════════════${NC}"
echo ""
echo -e "Outputs in: ${YELLOW}${JOCKY_OUTPUT}${NC}"
echo ""
echo "Next steps:"
echo "  1. Test Linux binary:   ./${JOCKY_OUTPUT}/$(basename ${JOCKY_SOURCE%.*})"
echo "  2. Test Windows binary: docker-compose run jocky-wine wine $(basename ${JOCKY_SOURCE%.*}).exe"
echo "  3. View report:         cat ${JOCKY_OUTPUT}/BUILD_REPORT.txt"
echo ""
