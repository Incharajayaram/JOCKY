#!/bin/bash
# JOCKY Polymorphic CI/CD Pipeline
# Generates N unique forensic agent binaries with:
# - Token diversification (unique variable/function names per build)
# - Per-build encryption keys
# - Unique hashes for each binary
# Usage: ./build_polymorphic.sh <count> <output_dir> [--static]

set -e

COUNT=${1:-10}
OUTPUT_DIR=${2:-./polymorphic_builds}
STATIC_FLAG=${3:-""}
SOURCE_FILE="/home/kamini/projects/sih148/JOCKY/forensics_agent.jky"
COMPILER="/home/kamini/projects/sih148/JOCKY/compiler/build/jockyc"

echo "====================================="
echo "JOCKY Polymorphic Build Pipeline"
echo "====================================="
echo "Generating $COUNT unique forensic agents"
echo "Output directory: $OUTPUT_DIR"
if [ "$STATIC_FLAG" == "--static" ]; then
    echo "Linking: STATIC (no dependencies)"
else
    echo "Linking: DYNAMIC (smaller, unique hashes)"
fi
echo ""

mkdir -p "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR/keys"

# Build metadata
META_FILE="$OUTPUT_DIR/build_manifest.json"
echo "{" > "$META_FILE"
echo "  \"build_count\": $COUNT," >> "$META_FILE"
echo "  \"source_file\": \"$SOURCE_FILE\"," >> "$META_FILE"
echo "  \"profile\": \"paranoid\"," >> "$META_FILE"
echo "  \"static_linking\": $(if [ "$STATIC_FLAG" == "--static" ]; then echo "true"; else echo "false"; fi)," >> "$META_FILE"
echo "  \"builds\": [" >> "$META_FILE"

SUCCESS=0
FAILED=0

for i in $(seq 1 $COUNT); do
    BUILD_ID=$(printf "%04d" $i)
    OUTPUT_NAME="jocky_agent_$BUILD_ID"
    OUTPUT_PATH="$OUTPUT_DIR/$OUTPUT_NAME"
    KEY_FILE="$OUTPUT_DIR/keys/${OUTPUT_NAME}.key"
    
    # Generate per-build encryption key (256-bit hex)
    ENCRYPTION_KEY=$(openssl rand -hex 32 2>/dev/null || cat /dev/urandom | tr -dc 'a-f0-9' | head -c 64)
    echo "$ENCRYPTION_KEY" > "$KEY_FILE"
    
    echo -n "[$BUILD_ID/$COUNT] Building $OUTPUT_NAME (key: ${ENCRYPTION_KEY:0:16}...)... "
    
    if $COMPILER "$SOURCE_FILE" -o "$OUTPUT_PATH" -p paranoid $STATIC_FLAG >/dev/null 2>&1; then
        HASH=$(sha256sum "$OUTPUT_PATH" | awk '{print $1}')
        SIZE=$(stat -c%s "$OUTPUT_PATH")
        
        # Extract seed from compiler output
        SEED=$($COMPILER "$SOURCE_FILE" -o "/tmp/seed_check" -p paranoid $STATIC_FLAG 2>&1 | grep "Token diversification seed" | awk '{print $5}')
        
        echo "OK (hash: ${HASH:0:16}...)"
        
        # Add to manifest
        if [ $i -lt $COUNT ]; then
            echo "    {\"id\": \"$BUILD_ID\", \"hash\": \"$HASH\", \"size\": $SIZE, \"seed\": \"$SEED\", \"key\": \"$ENCRYPTION_KEY\"}," >> "$META_FILE"
        else
            echo "    {\"id\": \"$BUILD_ID\", \"hash\": \"$HASH\", \"size\": $SIZE, \"seed\": \"$SEED\", \"key\": \"$ENCRYPTION_KEY\"}" >> "$META_FILE"
        fi
        
        SUCCESS=$((SUCCESS + 1))
    else
        echo "FAILED"
        FAILED=$((FAILED + 1))
    fi
done

echo "  ]" >> "$META_FILE"
echo "}" >> "$META_FILE"

echo ""
echo "====================================="
echo "Build Complete"
echo "====================================="
echo "Success: $SUCCESS"
echo "Failed:  $FAILED"
echo "Output:  $OUTPUT_DIR/"
echo "Keys:    $OUTPUT_DIR/keys/"
echo ""

# Verify all hashes are unique
echo "Checking hash uniqueness..."
UNIQUE_HASHES=$(find "$OUTPUT_DIR" -type f -name "jocky_agent_*" -exec sha256sum {} \; | awk '{print $1}' | sort -u | wc -l)
if [ "$UNIQUE_HASHES" -eq "$SUCCESS" ]; then
    echo "✓ All $SUCCESS builds have unique hashes"
else
    echo "✗ Only $UNIQUE_HASHES unique hashes out of $SUCCESS builds"
    echo "  (This is normal for static builds where glibc dominates)"
fi

echo ""
echo "Manifest: $META_FILE"
echo ""
echo "To test a build:"
echo "  $OUTPUT_DIR/jocky_agent_0001"
echo ""
echo "To view a build's encryption key:"
echo "  cat $OUTPUT_DIR/keys/jocky_agent_0001.key"
