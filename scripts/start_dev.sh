#!/bin/bash

echo "=================================================="
echo "JOCKY Local Development - Starting Services"
echo "=================================================="
echo ""

JOCKY_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Source environment
if [ -f "$JOCKY_ROOT/.env.local" ]; then
    export $(cat "$JOCKY_ROOT/.env.local" | grep -v '#' | xargs)
else
    export PYTHONPATH="$JOCKY_ROOT/src"
    export JOCKY_ROOT="$JOCKY_ROOT"
    export TOOLCHAIN_PATH="$JOCKY_ROOT/toolchain"
    export API_BASE="http://localhost:8000"
    export FRONTEND_PORT="5173"
    export BACKEND_PORT="8000"
fi

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Environment Variables:${NC}"
echo "  PYTHONPATH: $PYTHONPATH"
echo "  JOCKY_ROOT: $JOCKY_ROOT"
echo "  TOOLCHAIN_PATH: $TOOLCHAIN_PATH"
echo "  API_BASE: $API_BASE"
echo ""

# Function to kill all subprocesses on exit
cleanup() {
    echo ""
    echo -e "${YELLOW}Shutting down services...${NC}"
    kill %1 %2 %3 2>/dev/null || true
    wait %1 %2 %3 2>/dev/null || true
    echo -e "${GREEN}Services stopped${NC}"
}

trap cleanup EXIT

# Start Backend
echo -e "${BLUE}[1/3] Starting Backend API...${NC}"
cd "$JOCKY_ROOT/web/backend"
if [ ! -d "venv" ]; then
    echo -e "${RED}❌ Backend venv not found. Run: bash scripts/setup_local_dev.sh${NC}"
    exit 1
fi
source venv/bin/activate
export PYTHONPATH="$PYTHONPATH"
python3 -m uvicorn app:app --host 0.0.0.0 --port 8000 --reload &
BACKEND_PID=$!
echo -e "${GREEN}✓ Backend started (PID: $BACKEND_PID)${NC}"
echo "  API: http://localhost:8000"
echo "  Docs: http://localhost:8000/docs"
sleep 2

# Start Frontend
echo -e "${BLUE}[2/3] Starting Frontend Dev Server...${NC}"
cd "$JOCKY_ROOT/web/frontend"
if [ ! -d "node_modules" ]; then
    echo -e "${RED}❌ Frontend node_modules not found. Run: bash scripts/setup_local_dev.sh${NC}"
    exit 1
fi
npm run dev &
FRONTEND_PID=$!
echo -e "${GREEN}✓ Frontend started (PID: $FRONTEND_PID)${NC}"
echo "  Frontend: http://localhost:5173"
sleep 2

# Info
echo ""
echo -e "${BLUE}[3/3] Services Ready${NC}"
echo ""
echo -e "${GREEN}✓ All services running!${NC}"
echo ""
echo "=================================================="
echo "📡 Backend API:    http://localhost:8000"
echo "   API Docs:     http://localhost:8000/docs"
echo "   Health:       http://localhost:8000/api/config"
echo ""
echo "🎨 Frontend:       http://localhost:5173"
echo ""
echo "💻 CLI Compilation:"
echo "   Windows: python3 scripts/compile_pipeline.py examples/research_chain_windows_production.jky build/windows --platform windows"
echo "   Linux:   python3 scripts/compile_pipeline.py examples/research_chain_linux_production.jky build/linux --platform linux"
echo ""
echo "Press Ctrl+C to stop all services"
echo "=================================================="
echo ""

# Wait for all processes
wait %1 %2 %3 2>/dev/null || true
