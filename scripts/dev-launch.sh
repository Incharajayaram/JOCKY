#!/bin/bash

###############################################################################
# JOCKY Local Development - Auto-Launcher with tmux/screen
#
# Automatically launches backend, frontend, and compilation in separate panes
# Usage: bash scripts/dev-launch.sh
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
echo "JOCKY Local Development Launcher"
echo -e "==================================================${NC}"
echo ""

# Setup environments
echo -e "${CYAN}Setting up environments...${NC}"

# Backend setup
cd "$JOCKY_ROOT/web/backend"
if [ ! -d "venv" ]; then
    python3 -m venv venv
fi
if ! python3 -c "import fastapi" 2>/dev/null; then
    source venv/bin/activate
    pip install -q --upgrade pip
    pip install -q -r requirements.txt
    pip install -q uvicorn[standard] python-multipart websockets
    deactivate
fi

# Frontend setup
cd "$JOCKY_ROOT/web/frontend"
if [ ! -d "node_modules" ]; then
    npm install --silent
fi

echo -e "${GREEN}✓ Environments ready${NC}"
echo ""

# Check if tmux is available
if command -v tmux &> /dev/null; then
    echo -e "${CYAN}Launching with tmux...${NC}"

    SESSION="jocky-dev"

    # Kill existing session if it exists
    tmux kill-session -t $SESSION 2>/dev/null || true

    # Create new session with 3 windows
    tmux new-session -d -s $SESSION -n "backend"

    # Window 1: Backend
    tmux send-keys -t $SESSION:0 "cd '$JOCKY_ROOT/web/backend' && source venv/bin/activate && export PYTHONPATH='$JOCKY_ROOT/src' && export TOOLCHAIN_PATH='$JOCKY_ROOT/toolchain' && echo '' && echo '========================================' && echo 'Backend API' && echo '========================================' && echo '' && echo '📡 http://localhost:8000' && echo '📚 http://localhost:8000/docs' && echo '' && python3 -m uvicorn app:app --reload" Enter

    # Window 2: Frontend
    tmux new-window -t $SESSION -n "frontend"
    tmux send-keys -t $SESSION:1 "cd '$JOCKY_ROOT/web/frontend' && echo '' && echo '========================================' && echo 'Frontend Dev Server' && echo '========================================' && echo '' && echo '🎨 http://localhost:5173' && echo '' && npm run dev" Enter

    # Window 3: Compilation/Info
    tmux new-window -t $SESSION -n "compile"
    tmux send-keys -t $SESSION:2 "cd '$JOCKY_ROOT' && clear && echo '' && echo '========================================' && echo 'JOCKY Development Session Started!' && echo '========================================' && echo '' && echo 'Services:' && echo '  Backend:   http://localhost:8000' && echo '  Frontend:  http://localhost:5173' && echo '  API Docs:  http://localhost:8000/docs' && echo '' && echo 'Tmux commands:' && echo '  tmux attach -t $SESSION         - Attach to session' && echo '  tmux list-windows -t $SESSION   - List windows' && echo '  tmux kill-session -t $SESSION   - Kill session' && echo '' && echo 'To compile (in this window):' && echo '  bash scripts/compile.sh examples/research_chain_linux_production.jky build/linux --platform linux' && echo '' && bash" Enter

    echo -e "${GREEN}✓ Tmux session created: $SESSION${NC}"
    echo ""
    echo -e "${CYAN}Attaching to session...${NC}"
    echo ""
    tmux attach -t $SESSION

elif command -v screen &> /dev/null; then
    echo -e "${CYAN}Launching with screen...${NC}"

    SESSION="jocky-dev"

    # Kill existing session if it exists
    screen -S $SESSION -X quit 2>/dev/null || true

    # Create new session
    screen -d -m -S $SESSION bash -c "
    echo ''
    echo '=========================================='
    echo 'JOCKY Development Session'
    echo '=========================================='
    echo ''
    echo 'Available commands:'
    echo '  screen -r $SESSION              - Reconnect'
    echo '  screen -S $SESSION -X quit      - Stop session'
    echo ''
    echo 'Start services in separate terminals:'
    echo ''
    echo '1. Backend:'
    echo '   cd $JOCKY_ROOT/web/backend'
    echo '   source venv/bin/activate'
    echo '   export PYTHONPATH=$JOCKY_ROOT/src'
    echo '   export TOOLCHAIN_PATH=$JOCKY_ROOT/toolchain'
    echo '   python3 -m uvicorn app:app --reload'
    echo ''
    echo '2. Frontend:'
    echo '   cd $JOCKY_ROOT/web/frontend'
    echo '   npm run dev'
    echo ''
    bash
    "

    echo -e "${GREEN}✓ Screen session created: $SESSION${NC}"
    echo ""
    echo "Reconnect with: ${CYAN}screen -r $SESSION${NC}"
    echo ""

else
    echo -e "${YELLOW}⚠️  tmux/screen not available. Using terminal emulator...${NC}"
    echo ""

    # Fallback to terminal emulators
    TERMINAL=""
    if command -v gnome-terminal &> /dev/null; then
        TERMINAL="gnome-terminal"
    elif command -v xterm &> /dev/null; then
        TERMINAL="xterm"
    elif command -v konsole &> /dev/null; then
        TERMINAL="konsole"
    fi

    if [ -z "$TERMINAL" ]; then
        echo -e "${RED}❌ No suitable terminal found. Run dev-simple.sh instead:${NC}"
        echo "   bash scripts/dev-simple.sh"
        exit 1
    fi

    echo -e "${CYAN}Launching with $TERMINAL...${NC}"
    echo ""

    # Backend terminal
    $TERMINAL -- bash -c "cd '$JOCKY_ROOT/web/backend' && source venv/bin/activate && export PYTHONPATH='$JOCKY_ROOT/src' && export TOOLCHAIN_PATH='$JOCKY_ROOT/toolchain' && python3 -m uvicorn app:app --reload; bash" &
    sleep 2

    # Frontend terminal
    $TERMINAL -- bash -c "cd '$JOCKY_ROOT/web/frontend' && npm run dev; bash" &
    sleep 2

    # Info terminal
    $TERMINAL -- bash -c "cd '$JOCKY_ROOT' && echo '' && echo '========================================' && echo 'JOCKY Development Launcher' && echo '========================================' && echo '' && echo 'Services started:' && echo '  Backend:   http://localhost:8000' && echo '  Frontend:  http://localhost:5173' && echo '  API Docs:  http://localhost:8000/docs' && echo '' && bash" &

    echo -e "${GREEN}✓ Services launched in separate terminals${NC}"
    echo ""
fi

echo ""
echo -e "${BLUE}=================================================="
echo -e "✓ Services should be starting...${NC}"
echo -e "==================================================${NC}"
echo ""
echo -e "${GREEN}Access in browser:${NC}"
echo "  🎨 Frontend:  http://localhost:5173"
echo "  📡 Backend:   http://localhost:8000"
echo "  📚 API Docs:  http://localhost:8000/docs"
echo ""
