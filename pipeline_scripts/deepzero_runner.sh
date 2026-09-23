#!/bin/bash
set -e

# Configuration
JOCKY_DIR="/home/incharanew/JOCKY"
GHIDRA_URL="https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_11.1.2_build/ghidra_11.1.2_PUBLIC_20240709.zip"
GHIDRA_ZIP="ghidra.zip"
GHIDRA_DIR_NAME="ghidra_11.1.2_PUBLIC"

echo "=========================================="
echo "    DeepZero LOLDrivers Orchestrator      "
echo "=========================================="

# 1. Dependency Check (Java)
if ! command -v java &> /dev/null; then
    echo "[!] Java is not installed. Please install it using:"
    echo "    sudo apt update && sudo apt install default-jre unzip -y"
    exit 1
fi

# 2. Ghidra Setup (Local)
cd "$JOCKY_DIR"
if [ ! -d "$GHIDRA_DIR_NAME" ]; then
    echo "[*] Downloading Ghidra locally to JOCKY..."
    wget -q --show-progress -O "$GHIDRA_ZIP" "$GHIDRA_URL"
    echo "[*] Extracting Ghidra..."
    unzip -q "$GHIDRA_ZIP"
    rm "$GHIDRA_ZIP"
fi
export GHIDRA_INSTALL_DIR="$JOCKY_DIR/$GHIDRA_DIR_NAME"
echo "[+] Ghidra configured at $GHIDRA_INSTALL_DIR"

# 3. DeepZero Setup
if [ ! -d "DeepZero" ]; then
    echo "[*] Cloning DeepZero..."
    git clone https://github.com/416rehman/DeepZero.git
fi

cd DeepZero
if [ ! -d ".venv" ]; then
    echo "[*] Setting up Python virtual environment with Python 3.11..."
    python3.11 -m venv .venv
fi

echo "[*] Activating venv and installing requirements..."
source .venv/bin/activate
pip install -q -e .[full]
pip install -q google-generativeai pandas semgrep # For Gemini, aggregation, and semgrep scanner

# 4. Configure API Keys
if [ -z "$GEMINI_API_KEY" ]; then
    echo "[!] GEMINI_API_KEY environment variable is not set."
    echo "    DeepZero and the REPL need this to analyze exploitability via Google Gemini."
    echo "    Run: export GEMINI_API_KEY='your-key-here'"
    exit 1
fi

export LITELLM_MODEL="gemini/gemini-3.5-flash"

# 5. Check for Downloaded Corpus
CORPUS_DIR="/home/incharanew/Downloads/drivers_out"
if [ ! -d "$CORPUS_DIR" ] || [ -z "$(ls -A "$CORPUS_DIR" 2>/dev/null)" ]; then
    echo "[!] Corpus directory empty or not found at $CORPUS_DIR."
    echo "    Please run download_unblocked.py first to fetch the drivers."
    exit 1
fi

echo "[+] Pointing DeepZero at corpus: $CORPUS_DIR"

# 6. Execute DeepZero Pipeline
echo "[*] Running DeepZero AI analysis pipeline..."
deepzero run -p loldrivers "$CORPUS_DIR"

echo "[+] Analysis complete! State directory populated."
echo "[+] To aggregate results and start the interactive LLM REPL, run:"
echo "    python3 ../aggregator.py"

cd "$JOCKY_DIR"
echo "=========================================="
echo "          Orchestration Finished          "
echo "=========================================="
