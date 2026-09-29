#!/bin/bash
set -e

SOURCE_FILE="$1"
OUTPUT_DIR="$2"
PLATFORM="${3:-windows}"

if [ "$3" == "--platform" ] && [ -n "$4" ]; then
    PLATFORM="$4"
fi

if [ -z "$SOURCE_FILE" ] || [ -z "$OUTPUT_DIR" ]; then
    echo "Usage: $0 <source.jky> <output_dir> [--platform linux|windows]"
    echo "Example: $0 examples/research_chain_linux_production.jky build/local --platform linux"
    exit 1
fi

echo "=================================================="
echo "JOCKY Local Development Build"
echo "=================================================="
echo ""

if ! docker info > /dev/null 2>&1; then
    echo "❌ Docker is not running!"
    exit 1
fi

echo "[*] Starting Docker services..."
docker-compose -f docker-compose.dev.yml up -d --quiet-pull

echo "[*] Waiting for services..."
sleep 3

echo "[*] Verifying C2 server..."
for i in {1..10}; do
    if curl -s http://localhost:5000/api/status > /dev/null 2>&1; then
        echo "    [+] C2 server is online"
        break
    fi
    if [ $i -eq 10 ]; then
        echo "    [-] C2 server failed!"
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
        echo "    [-] Model repository failed!"
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
        echo "    [-] CDN endpoint failed!"
        exit 1
    fi
    sleep 1
done

echo ""
echo "[*] Setting up environment variables..."

export JOCKY_C2_PRIMARY="http://localhost:5000/api/config"
export JOCKY_C2_FALLBACK="http://localhost:5000/api/config"
export JOCKY_MODEL_REPO="http://localhost:5001/models/"
export JOCKY_BUILD_ID="local_dev_$(date +%s)"

echo "    [+] C2_PRIMARY: $JOCKY_C2_PRIMARY"
echo "    [+] C2_FALLBACK: $JOCKY_C2_FALLBACK"
echo "    [+] MODEL_REPO: $JOCKY_MODEL_REPO"
echo "    [+] BUILD_ID: $JOCKY_BUILD_ID"
echo ""

mkdir -p "$OUTPUT_DIR"

echo "[*] Building with local configuration..."
echo ""

python3 "$(dirname "$0")/compile_pipeline.py" \
    "$SOURCE_FILE" \
    "$OUTPUT_DIR" \
    --platform "$PLATFORM"

echo ""
echo "[+] Build complete!"
echo ""
echo "=================================================="
echo "Local Development Setup"
echo "=================================================="
echo ""
echo "📍 Services:"
echo "   C2 Server:  http://localhost:5000"
echo "   Models:     http://localhost:5001"
echo "   CDN:        http://localhost:5080"
echo ""
echo "🔍 View logs:"
echo "   docker-compose -f docker-compose.dev.yml logs c2-server"
echo ""
echo "🛑 Stop services:"
echo "   docker-compose -f docker-compose.dev.yml down"
echo ""
echo "=================================================="
