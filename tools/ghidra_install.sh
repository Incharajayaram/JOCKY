#!/bin/bash
set -e

GHIDRA_URL="https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_11.3.1_build/ghidra_11.3.1_PUBLIC_20250219.zip"
LOCAL_DIR="/tmp/ghidra_local"
DEST="/opt/ghidra"

if ls "$LOCAL_DIR"/ghidra*.zip 1>/dev/null 2>&1; then
    echo "[ghidra] Using local zip"
    ZIP=$(ls "$LOCAL_DIR"/ghidra*.zip | head -1)
else
    echo "[ghidra] Local zip not found, downloading..."
    ZIP="/tmp/ghidra_download.zip"
    wget -q -O "$ZIP" "$GHIDRA_URL"
fi

unzip -q "$ZIP" -d /opt
mv /opt/ghidra_* "$DEST"
echo "[ghidra] Installed to $DEST"
