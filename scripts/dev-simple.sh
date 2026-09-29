#!/bin/bash

###############################################################################
# JOCKY Local Development - Simple Version
#
# Start all services with clear instructions
# Usage: bash scripts/dev-simple.sh
###############################################################################

set -e

JOCKY_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

echo ""
echo -e "${BLUE}=================================================="
echo "JOCKY Local Development Setup"
echo -e "==================================================${NC}"
echo ""

# Check dependencies
echo -e "${CYAN}[1/3] Checking dependencies...${NC}"
if ! command -v python3 &> /dev/null; then
    echo -e "${RED}❌ Python 3 not found${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Python $(python3 --version | cut -d' ' -f2)${NC}"

if ! command -v node &> /dev/null; then
    echo -e "${RED}❌ Node.js not found${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Node $(node --version)${NC}"

if [ ! -f "$JOCKY_ROOT/toolchain/bin/clang" ]; then
    echo -e "${RED}❌ Toolchain not found${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Toolchain ready${NC}"

# Setup backend
echo ""
echo -e "${CYAN}[2/3] Setting up backend...${NC}"
cd "$JOCKY_ROOT/web/backend"

if [ ! -d "venv" ]; then
    echo -e "${YELLOW}Creating Python virtual environment...${NC}"
    python3 -m venv venv
fi

source venv/bin/activate
if ! python3 -c "import fastapi" 2>/dev/null; then
    echo -e "${YELLOW}Installing Python dependencies...${NC}"
    pip install -q --upgrade pip
    pip install -q -r requirements.txt
    pip install -q uvicorn[standard] python-multipart websockets
fi
deactivate
echo -e "${GREEN}✓ Backend ready${NC}"

# Setup frontend
echo ""
echo -e "${CYAN}[3/3] Setting up frontend...${NC}"
cd "$JOCKY_ROOT/web/frontend"

if [ ! -d "node_modules" ]; then
    echo -e "${YELLOW}Installing npm dependencies (this may take a minute)...${NC}"
    npm install --silent
fi
echo -e "${GREEN}✓ Frontend ready${NC}"

echo ""
echo -e "${BLUE}=================================================="
echo "Setup Complete! Ready to Start Services"
echo -e "==================================================${NC}"
echo ""

echo -e "${YELLOW}📋 Instructions:${NC}"
echo ""
echo "Open THREE separate terminals and run these commands:"
echo ""
echo -e "${CYAN}Terminal 1 - Backend API:${NC}"
echo "  cd $JOCKY_ROOT/web/backend"
echo "  source venv/bin/activate"
echo "  export PYTHONPATH=$JOCKY_ROOT/src"
echo "  export TOOLCHAIN_PATH=$JOCKY_ROOT/toolchain"
echo "  python3 -m uvicorn app:app --reload"
echo ""
echo -e "${CYAN}Terminal 2 - Frontend Dev Server:${NC}"
echo "  cd $JOCKY_ROOT/web/frontend"
echo "  npm run dev"
echo ""
echo -e "${CYAN}Terminal 3 - Compilation (optional):${NC}"
echo "  cd $JOCKY_ROOT"
echo "  export PYTHONPATH=$JOCKY_ROOT/src"
echo "  bash scripts/compile.sh examples/research_chain_linux_production.jky build/linux --platform linux"
echo ""
echo -e "${GREEN}Once services are running:${NC}"
echo "  🎨 Frontend:  http://localhost:5173"
echo "  📡 Backend:   http://localhost:8000"
echo "  📚 API Docs:  http://localhost:8000/docs"
echo ""
echo -e "${YELLOW}Quick test (after services start):${NC}"
echo "  curl http://localhost:8000/api/config"
echo ""
echo -e "${BLUE}=================================================="
echo -e "Happy coding! 🚀${NC}"
echo ""
