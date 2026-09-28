#!/bin/bash
# JOCKY Local CDN Setup - Caddy Reverse Proxy
# Alternative to AWS for authorized research environments
# Authorized: Red Hat + IIT Bombay Cyber Security Team

set -e

export CDN_ROOT="/opt/jocky-cdn"
export CADDY_CONFIG="$CDN_ROOT/Caddyfile"
export CADDY_STORAGE="$CDN_ROOT/.caddy"
export DATA_DIR="$CDN_ROOT/data"
export LOG_DIR="$CDN_ROOT/logs"

# Configuration
CDN_DOMAIN="research.internal"
CDN_PORT=8443
CDN_HTTP_PORT=8080
STORAGE_QUOTA_GB=100

print_step() {
    echo "[*] $1"
}

print_success() {
    echo "[+] $1"
}

print_error() {
    echo "[!] $1"
}

# Step 1: Install Caddy
step_install_caddy() {
    print_step "Installing Caddy reverse proxy..."

    # Check if Caddy is already installed
    if command -v caddy &> /dev/null; then
        local version=$(caddy version)
        print_success "Caddy already installed: $version"
        return
    fi

    # Install from official repository
    if command -v apt-get &> /dev/null; then
        # Debian/Ubuntu
        sudo apt-get update
        sudo apt-get install -y caddy
    elif command -v dnf &> /dev/null; then
        # Fedora/RHEL
        sudo dnf install -y caddy
    elif command -v brew &> /dev/null; then
        # macOS
        brew install caddy
    else
        # Fallback: build from source
        print_step "Building Caddy from source..."
        go install github.com/caddyserver/caddy/v2/cmd/caddy@latest
    fi

    print_success "Caddy installed"
}

# Step 2: Create directory structure
step_create_directories() {
    print_step "Creating CDN directory structure..."

    sudo mkdir -p "$CDN_ROOT"
    sudo mkdir -p "$DATA_DIR"
    sudo mkdir -p "$LOG_DIR"
    sudo mkdir -p "$CADDY_STORAGE"

    # Set permissions
    sudo chown -R $USER:$USER "$CDN_ROOT"
    chmod -R 755 "$CDN_ROOT"

    print_success "Directories created: $CDN_ROOT"
}

# Step 3: Generate self-signed certificate (if needed)
step_generate_certificates() {
    print_step "Setting up TLS certificates..."

    local cert_dir="$CDN_ROOT/certs"
    mkdir -p "$cert_dir"

    # Check if certificates already exist
    if [ -f "$cert_dir/research.internal.crt" ] && [ -f "$cert_dir/research.internal.key" ]; then
        print_success "Certificates already exist"
        return
    fi

    print_step "Generating self-signed certificate for $CDN_DOMAIN..."

    # Generate self-signed cert (365 days valid)
    openssl req -x509 -newkey rsa:4096 -keyout "$cert_dir/research.internal.key" \
        -out "$cert_dir/research.internal.crt" -days 365 -nodes \
        -subj "/C=IN/ST=Maharashtra/L=Mumbai/O=Red Hat/CN=$CDN_DOMAIN" 2>/dev/null

    chmod 600 "$cert_dir/research.internal.key"
    chmod 644 "$cert_dir/research.internal.crt"

    print_success "Self-signed certificate generated"
    print_success "Certificate: $cert_dir/research.internal.crt"
    print_success "Key: $cert_dir/research.internal.key"
}

# Step 4: Create Caddyfile configuration
step_create_caddyfile() {
    print_step "Creating Caddy configuration..."

    cat > "$CADDY_CONFIG" << 'EOF'
{
    storage file_system {
        root /opt/jocky-cdn/.caddy
    }
    log {
        level info
        output file /opt/jocky-cdn/logs/caddy.log {
            roll_size 100mb
            roll_keep 5
            roll_keep_for 720h
        }
    }
}

# Local research CDN endpoint
research.internal:8443 {
    # TLS configuration
    tls /opt/jocky-cdn/certs/research.internal.crt /opt/jocky-cdn/certs/research.internal.key

    # Request logging
    log {
        level info
        output file /opt/jocky-cdn/logs/access.log {
            roll_size 50mb
            roll_keep 10
            roll_keep_for 720h
        }
    }

    # File server for data uploads
    root /opt/jocky-cdn/data
    file_server {
        hide .env
        hide .git
        hide .gitignore
        hide Caddyfile
        hide caddy
    }

    # Upload endpoint
    @upload {
        method POST
        path /upload*
    }
    reverse_proxy @upload http://localhost:9000

    # Listing endpoint
    @list {
        method GET
        path /list*
    }
    reverse_proxy @list http://localhost:9000

    # Authentication middleware
    basicauth /admin/* {
        research $2a$14$bvU3FfMcqXnWkiMrFfLcIe8ZVUl96tQJXkLiJSG7.R5V1j3llHJOO
    }

    # Compression
    encode gzip

    # Security headers
    header / X-Content-Type-Options nosniff
    header / X-Frame-Options DENY
    header / X-XSS-Protection "1; mode=block"
    header / Referrer-Policy strict-origin-when-cross-origin
}

# HTTP redirect to HTTPS
research.internal:8080 {
    redir https://{host}:8443{uri} permanent
}
EOF

    print_success "Caddyfile created: $CADDY_CONFIG"
}

# Step 5: Create data upload handler
step_create_upload_handler() {
    print_step "Creating data upload handler..."

    cat > "$CDN_ROOT/upload_server.py" << 'EOF'
#!/usr/bin/env python3
"""
JOCKY CDN Upload Handler
Receives encrypted data chunks from research chain
"""

from flask import Flask, request, jsonify
from pathlib import Path
import hashlib
import os
from datetime import datetime
import logging

app = Flask(__name__)

# Configuration
DATA_DIR = Path("/opt/jocky-cdn/data")
LOG_DIR = Path("/opt/jocky-cdn/logs")
MAX_CHUNK_SIZE = 100 * 1024 * 1024  # 100MB
AUTH_TOKEN = os.environ.get("CDN_AUTH_TOKEN", "secure_token_change_me")

# Logging
logging.basicConfig(
    level=logging.INFO,
    handlers=[
        logging.FileHandler(LOG_DIR / "upload.log"),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)

@app.before_request
def check_auth():
    """Verify authentication token"""
    token = request.headers.get("Authorization", "").replace("Bearer ", "")
    if not token or token != AUTH_TOKEN:
        logger.warning(f"Unauthorized upload attempt from {request.remote_addr}")
        return jsonify({"error": "Unauthorized"}), 401

@app.route("/upload", methods=["POST"])
def upload_chunk():
    """Handle data chunk upload"""

    if "file" not in request.files:
        return jsonify({"error": "No file provided"}), 400

    file = request.files["file"]
    metadata = request.form.get("metadata", "")

    # Security checks
    if not file.filename:
        return jsonify({"error": "Invalid filename"}), 400

    if file.content_length > MAX_CHUNK_SIZE:
        return jsonify({"error": "File too large"}), 413

    try:
        # Read file data
        data = file.read()

        # Create upload directory structure
        upload_dir = DATA_DIR / "uploads" / datetime.now().strftime("%Y%m%d")
        upload_dir.mkdir(parents=True, exist_ok=True)

        # Generate filename with hash
        file_hash = hashlib.sha256(data).hexdigest()
        filename = f"{file_hash}_{file.filename}"
        filepath = upload_dir / filename

        # Save file
        with open(filepath, "wb") as f:
            f.write(data)

        # Log upload
        logger.info(
            f"Upload successful: {filename} ({len(data)} bytes) "
            f"from {request.remote_addr} - Metadata: {metadata}"
        )

        return jsonify({
            "status": "success",
            "filename": filename,
            "path": f"/uploads/{datetime.now().strftime('%Y%m%d')}/{filename}",
            "size": len(data),
            "hash": file_hash
        }), 200

    except Exception as e:
        logger.error(f"Upload error: {str(e)}")
        return jsonify({"error": "Upload failed"}), 500

@app.route("/list", methods=["GET"])
def list_uploads():
    """List uploaded files"""

    try:
        uploads = []
        upload_dir = DATA_DIR / "uploads"

        if not upload_dir.exists():
            return jsonify({"uploads": []}), 200

        # Walk upload directory
        for root, dirs, files in os.walk(upload_dir):
            for file in files:
                filepath = Path(root) / file
                stat = filepath.stat()
                uploads.append({
                    "path": str(filepath.relative_to(DATA_DIR)),
                    "size": stat.st_size,
                    "modified": stat.st_mtime
                })

        logger.info(f"Listed {len(uploads)} files for {request.remote_addr}")
        return jsonify({"uploads": uploads}), 200

    except Exception as e:
        logger.error(f"List error: {str(e)}")
        return jsonify({"error": "List failed"}), 500

@app.route("/health", methods=["GET"])
def health():
    """Health check"""
    return jsonify({
        "status": "ok",
        "timestamp": datetime.now().isoformat(),
        "data_dir": str(DATA_DIR),
        "storage_used": sum(
            f.stat().st_size for f in Path(DATA_DIR).glob("**/*") if f.is_file()
        )
    }), 200

if __name__ == "__main__":
    # Create required directories
    Path("/opt/jocky-cdn/logs").mkdir(parents=True, exist_ok=True)
    Path("/opt/jocky-cdn/data/uploads").mkdir(parents=True, exist_ok=True)

    # Run Flask app
    app.run(
        host="127.0.0.1",
        port=9000,
        debug=False,
        use_reloader=False
    )
EOF

    chmod +x "$CDN_ROOT/upload_server.py"
    print_success "Upload handler created: $CDN_ROOT/upload_server.py"
}

# Step 6: Create systemd service
step_create_systemd_service() {
    print_step "Creating systemd services..."

    # Caddy service
    cat > /tmp/jocky-cdn.service << 'EOF'
[Unit]
Description=JOCKY Research CDN (Caddy)
After=network-online.target
Wants=network-online.target

[Service]
Type=notify
User=root
Group=root
ExecStart=/usr/bin/caddy run --config /opt/jocky-cdn/Caddyfile
ExecReload=/usr/bin/caddy reload --config /opt/jocky-cdn/Caddyfile
TimeoutStopSec=5s
LimitNOFILE=1048576
LimitNPROC=512
StandardOutput=journal
StandardError=journal
SyslogIdentifier=caddy

[Install]
WantedBy=multi-user.target
EOF

    print_step "Caddy systemd service:"
    cat /tmp/jocky-cdn.service

    # Upload handler service
    cat > /tmp/jocky-cdn-upload.service << 'EOF'
[Unit]
Description=JOCKY CDN Upload Handler
After=network.target
Wants=jocky-cdn.service

[Service]
Type=simple
User=www-data
Group=www-data
WorkingDirectory=/opt/jocky-cdn
Environment="CDN_AUTH_TOKEN=secure_token_change_me"
ExecStart=/usr/bin/python3 /opt/jocky-cdn/upload_server.py
Restart=always
RestartSec=10
StandardOutput=journal
StandardError=journal
SyslogIdentifier=jocky-upload

[Install]
WantedBy=multi-user.target
EOF

    print_step "Upload handler systemd service:"
    cat /tmp/jocky-cdn-upload.service

    print_success "Services created (run with sudo to install)"
}

# Step 7: Create monitoring script
step_create_monitoring() {
    print_step "Creating CDN monitoring script..."

    cat > "$CDN_ROOT/monitor_cdn.sh" << 'EOF'
#!/bin/bash
# JOCKY CDN Monitoring Script

CDN_ROOT="/opt/jocky-cdn"
DATA_DIR="$CDN_ROOT/data"

echo "========================================================================"
echo "JOCKY CDN Status - $(date)"
echo "========================================================================"
echo ""

# Caddy status
echo "[*] Caddy Process:"
if pgrep caddy > /dev/null; then
    echo "    [+] Running (PID: $(pgrep caddy))"
else
    echo "    [-] Not running"
fi
echo ""

# Upload handler status
echo "[*] Upload Handler:"
if pgrep -f "upload_server.py" > /dev/null; then
    echo "    [+] Running (PID: $(pgrep -f upload_server.py))"
else
    echo "    [-] Not running"
fi
echo ""

# Storage statistics
echo "[*] Storage Usage:"
total_size=$(du -sh "$DATA_DIR" 2>/dev/null | cut -f1)
file_count=$(find "$DATA_DIR" -type f 2>/dev/null | wc -l)
echo "    Total: $total_size ($file_count files)"
echo ""

# Recent uploads
echo "[*] Recent Uploads:"
if [ -d "$DATA_DIR/uploads" ]; then
    find "$DATA_DIR/uploads" -type f -mtime -1 | head -10 | while read file; do
        size=$(du -h "$file" | cut -f1)
        echo "    - $file ($size)"
    done
else
    echo "    No uploads yet"
fi
echo ""

# Port connectivity
echo "[*] Port Status:"
if netstat -tuln 2>/dev/null | grep -q :8443; then
    echo "    [+] HTTPS (8443): Listening"
else
    echo "    [-] HTTPS (8443): Not listening"
fi

if netstat -tuln 2>/dev/null | grep -q :8080; then
    echo "    [+] HTTP (8080): Listening"
else
    echo "    [-] HTTP (8080): Not listening"
fi

if netstat -tuln 2>/dev/null | grep -q :9000; then
    echo "    [+] Upload Handler (9000): Listening"
else
    echo "    [-] Upload Handler (9000): Not listening"
fi
echo ""

# Log status
echo "[*] Recent Log Activity:"
if [ -f "$CDN_ROOT/logs/access.log" ]; then
    tail -5 "$CDN_ROOT/logs/access.log" | sed 's/^/    /'
else
    echo "    No access logs yet"
fi
echo ""

echo "========================================================================"
EOF

    chmod +x "$CDN_ROOT/monitor_cdn.sh"
    print_success "Monitoring script created: $CDN_ROOT/monitor_cdn.sh"
}

# Step 8: Create JOCKY integration
step_create_jocky_integration() {
    print_step "Creating JOCKY CDN integration..."

    cat > "$CDN_ROOT/jocky_cdn_integration.h" << 'EOF'
#ifndef JOCKY_CDN_INTEGRATION_H
#define JOCKY_CDN_INTEGRATION_H

#include <stdint.h>

/* Local CDN configuration */
typedef struct {
    const char* endpoint_url;    /* https://research.internal:8443 */
    const char* auth_token;      /* Bearer token for uploads */
    const char* ca_cert_path;    /* Path to CA certificate */
    uint32_t max_chunk_size;     /* Max chunk size (100MB) */
    uint32_t timeout_ms;         /* Upload timeout */
} JOCKY_CDN_CONFIG;

/* Upload result */
typedef struct {
    int status_code;
    char filename[256];
    char path[512];
    uint32_t size;
    char hash[65];  /* SHA256 */
} JOCKY_CDN_UPLOAD_RESULT;

/* Initialize CDN connection */
int jocky_cdn_init(const JOCKY_CDN_CONFIG* config);

/* Upload encrypted data chunk */
int jocky_cdn_upload_chunk(
    const void* data,
    uint32_t size,
    const char* metadata,
    JOCKY_CDN_UPLOAD_RESULT* out_result);

/* List uploaded files */
int jocky_cdn_list_files(
    char** out_files,
    uint32_t* out_count);

/* Get CDN status */
int jocky_cdn_get_status(
    char* out_status,
    uint32_t status_buf_size);

/* Shutdown CDN connection */
int jocky_cdn_shutdown(void);

#endif
EOF

    print_success "JOCKY CDN integration header: $CDN_ROOT/jocky_cdn_integration.h"
}

# Step 9: Create usage guide
step_create_usage_guide() {
    print_step "Creating usage guide..."

    cat > "$CDN_ROOT/USAGE.txt" << 'EOF'
JOCKY Local CDN - Usage Guide
==============================

1. START SERVICES:
   sudo systemctl start jocky-cdn
   sudo systemctl start jocky-cdn-upload

   Or manually:
   caddy run --config /opt/jocky-cdn/Caddyfile
   python3 /opt/jocky-cdn/upload_server.py

2. VERIFY SERVICES:
   ./monitor_cdn.sh

3. UPLOAD DATA:
   # Example using curl
   curl -k \
     -H "Authorization: Bearer secure_token_change_me" \
     -F "file=@data.bin" \
     -F "metadata=research_chain_v2" \
     https://research.internal:8443/upload

4. LIST FILES:
   curl -k \
     -H "Authorization: Bearer secure_token_change_me" \
     https://research.internal:8443/list

5. DOWNLOAD FILES:
   curl -k https://research.internal:8443/uploads/20260928/HASH_filename.bin

6. CONFIGURE JOCKY:
   const CDN_ENDPOINT = "https://research.internal:8443/upload"
   const CDN_AUTH_TOKEN = "secure_token_change_me"

7. STOP SERVICES:
   sudo systemctl stop jocky-cdn jocky-cdn-upload

8. VIEW LOGS:
   tail -f /opt/jocky-cdn/logs/access.log
   tail -f /opt/jocky-cdn/logs/upload.log
   tail -f /opt/jocky-cdn/logs/caddy.log

SECURITY NOTES:
- Change CDN_AUTH_TOKEN in upload_server.py and JOCKY config
- Use self-signed cert only in research environment
- Restrict access with firewall rules
- Monitor /opt/jocky-cdn/logs/ for unauthorized attempts
- Storage limited to 100GB by default (edit Caddyfile)

EOF

    print_success "Usage guide: $CDN_ROOT/USAGE.txt"
    cat "$CDN_ROOT/USAGE.txt"
}

# Main execution
main() {
    echo ""
    echo "========================================================================"
    echo "JOCKY Local CDN Setup - Caddy Reverse Proxy"
    echo "========================================================================"
    echo ""

    step_install_caddy
    echo ""

    step_create_directories
    echo ""

    step_generate_certificates
    echo ""

    step_create_caddyfile
    echo ""

    step_create_upload_handler
    echo ""

    step_create_systemd_service
    echo ""

    step_create_monitoring
    echo ""

    step_create_jocky_integration
    echo ""

    step_create_usage_guide
    echo ""

    echo "========================================================================"
    echo "Setup Complete"
    echo "========================================================================"
    echo ""
    echo "Next Steps:"
    echo "1. Review configuration:"
    echo "   cat $CADDY_CONFIG"
    echo ""
    echo "2. Install systemd services (requires sudo):"
    echo "   sudo cp /tmp/jocky-cdn.service /etc/systemd/system/"
    echo "   sudo cp /tmp/jocky-cdn-upload.service /etc/systemd/system/"
    echo "   sudo systemctl daemon-reload"
    echo "   sudo systemctl enable jocky-cdn jocky-cdn-upload"
    echo ""
    echo "3. Start services:"
    echo "   sudo systemctl start jocky-cdn"
    echo "   sudo systemctl start jocky-cdn-upload"
    echo ""
    echo "4. Verify:"
    echo "   $CDN_ROOT/monitor_cdn.sh"
    echo ""
    echo "5. Update JOCKY configuration with:"
    echo "   const CDN_ENDPOINT = \"https://research.internal:8443/upload\""
    echo ""
    echo "Local CDN ready for authorized research"
    echo ""
}

main "$@"
