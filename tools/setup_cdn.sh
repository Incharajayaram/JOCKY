#!/bin/bash
# Setup script for JOCKY local CDN (Caddy-based)

set -e

echo "[*] Setting up JOCKY local CDN..."

# Create directories
sudo mkdir -p /opt/jocky-cdn/data/uploads
sudo chmod 755 /opt/jocky-cdn /opt/jocky-cdn/data /opt/jocky-cdn/data/uploads

# Copy Caddyfile
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
sudo cp "$SCRIPT_DIR/Caddyfile" /opt/jocky-cdn/Caddyfile
sudo chmod 644 /opt/jocky-cdn/Caddyfile

echo "[+] CDN directories created"
echo "[+] Caddyfile installed to /opt/jocky-cdn/Caddyfile"
echo ""
echo "To start the CDN server, run:"
echo "  caddy run --config /opt/jocky-cdn/Caddyfile"
echo ""
echo "Or using Docker:"
echo "  docker run -p 8443:8443 -v /opt/jocky-cdn:/config caddy caddy run --config /config/Caddyfile"
echo ""
