#!/bin/bash
set -e

echo "=================================================="
echo "JOCKY Local Development Setup"
echo "=================================================="
echo ""

JOCKY_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
echo "[*] JOCKY Root: $JOCKY_ROOT"

# Check Python
echo "[*] Checking Python..."
if ! command -v python3 &> /dev/null; then
    echo "❌ Python 3 not found!"
    exit 1
fi
PYTHON_VERSION=$(python3 --version)
echo "    ✓ $PYTHON_VERSION"

# Check Node.js
echo "[*] Checking Node.js..."
if ! command -v node &> /dev/null; then
    echo "❌ Node.js not found! Installing..."
    curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
    sudo apt-get install -y nodejs
fi
NODE_VERSION=$(node --version)
echo "    ✓ $NODE_VERSION"

# Check npm
echo "[*] Checking npm..."
NPM_VERSION=$(npm --version)
echo "    ✓ npm $NPM_VERSION"

# Check required build tools
echo "[*] Checking build tools..."
if ! command -v gcc &> /dev/null; then
    echo "❌ GCC not found! Installing..."
    sudo apt-get update
    sudo apt-get install -y build-essential
fi

if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "❌ MinGW not found! Installing..."
    sudo apt-get install -y mingw-w64 mingw-w64-tools mingw-w64-x86-64-dev
fi

if ! pkg-config --exists openssl 2>/dev/null; then
    echo "❌ OpenSSL dev not found! Installing..."
    sudo apt-get install -y libssl-dev libcurl4-openssl-dev zlib1g-dev
fi

echo "    ✓ Build tools ready"

# Setup backend
echo "[*] Setting up backend..."
cd "$JOCKY_ROOT/web/backend"

if [ ! -d "venv" ]; then
    echo "    Creating virtual environment..."
    python3 -m venv venv
fi

echo "    Activating virtual environment..."
source venv/bin/activate

echo "    Installing Python dependencies..."
pip install -q --upgrade pip
pip install -q -r requirements.txt
pip install -q uvicorn[standard] python-multipart websockets

deactivate
echo "    ✓ Backend ready"

# Setup frontend
echo "[*] Setting up frontend..."
cd "$JOCKY_ROOT/web/frontend"

if [ ! -d "node_modules" ]; then
    echo "    Installing npm dependencies..."
    npm install --silent
else
    echo "    npm dependencies already installed"
fi
echo "    ✓ Frontend ready"

# Verify toolchain
echo "[*] Verifying toolchain..."
if [ ! -f "$JOCKY_ROOT/toolchain/bin/clang" ]; then
    echo "❌ Toolchain not found at $JOCKY_ROOT/toolchain/bin/clang"
    exit 1
fi
CLANG_VERSION=$("$JOCKY_ROOT/toolchain/bin/clang" --version | head -1)
echo "    ✓ $CLANG_VERSION"

# Create environment file
echo "[*] Creating environment configuration..."
cat > "$JOCKY_ROOT/.env.local" << 'EOF'
# Local Development Environment
PYTHONPATH=/home/incharanew/JOCKY/src
JOCKY_ROOT=/home/incharanew/JOCKY
TOOLCHAIN_PATH=/home/incharanew/JOCKY/toolchain
API_BASE=http://localhost:8000
FRONTEND_PORT=5173
BACKEND_PORT=8000
EOF

echo "    ✓ .env.local created"

echo ""
echo "=================================================="
echo "✓ Setup Complete!"
echo "=================================================="
echo ""
echo "Next steps:"
echo "  1. Run: source $JOCKY_ROOT/scripts/start_dev.sh"
echo "  2. Or run individually:"
echo "     - Backend:   cd $JOCKY_ROOT/web/backend && source venv/bin/activate && python3 -m uvicorn app:app --reload"
echo "     - Frontend:  cd $JOCKY_ROOT/web/frontend && npm run dev"
echo "     - Compile:   python3 $JOCKY_ROOT/scripts/compile_pipeline.py [source.jky] [output_dir] --platform [windows|linux]"
echo ""
