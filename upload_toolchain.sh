#!/usr/bin/env bash
set -euo pipefail

# Usage: ./upload_toolchain.sh /path/to/gcs-key.json
# Get the key file from the project owner before running this script.

BUCKET="gs://jocky-toolchain"
ARCHIVE="toolchain.zip"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
KEY_FILE="${1:-}"

if [ -z "$KEY_FILE" ]; then
    echo "Usage: $0 /path/to/gcs-key.json"
    exit 1
fi

if [ ! -f "$KEY_FILE" ]; then
    echo "Error: key file not found: $KEY_FILE"
    exit 1
fi

if [ ! -d "$SCRIPT_DIR/toolchain" ]; then
    echo "Error: toolchain/ directory not found in $SCRIPT_DIR"
    exit 1
fi

echo "Authenticating with GCP..."
gcloud auth activate-service-account --key-file="$KEY_FILE"

echo "Compressing toolchain..."
rm -f "$SCRIPT_DIR/$ARCHIVE"
cd "$SCRIPT_DIR" && { zip -ry "$ARCHIVE" toolchain; ZIP_RC=$?; [ $ZIP_RC -le 1 ] || exit $ZIP_RC; }

echo "Uploading to $BUCKET..."
gcloud storage cp "$SCRIPT_DIR/$ARCHIVE" "$BUCKET/$ARCHIVE"

rm "$SCRIPT_DIR/$ARCHIVE"
echo "Done. Toolchain uploaded to $BUCKET/$ARCHIVE"
