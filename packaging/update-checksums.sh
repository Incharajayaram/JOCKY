#!/bin/bash
# Update Checksums from GitHub Release
# Downloads release artifacts and computes SHA256 checksums
# Usage: ./update-checksums.sh v0.2.0

set -euo pipefail

if [ $# -lt 1 ]; then
    echo "Usage: $0 <tag> [version]"
    echo "Example: $0 v0.2.0 0.2.0"
    echo ""
    echo "This script downloads artifacts from a GitHub release and updates checksums."
    exit 1
fi

TAG=$1
VERSION=${2:-$(echo "$TAG" | sed 's/v//')}
REPO="devalgupta/JOCKY"
RELEASE_URL="https://github.com/$REPO/releases/download/$TAG"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMP_DIR=$(mktemp -d)

echo "=========================================="
echo "GitHub Release Checksum Updater"
echo "=========================================="
echo "Repository: $REPO"
echo "Tag: $TAG"
echo "Version: $VERSION"
echo "Temp Directory: $TEMP_DIR"
echo ""

cleanup() {
    rm -rf "$TEMP_DIR"
}
trap cleanup EXIT

cd "$TEMP_DIR"

# Define expected artifacts
declare -A ARTIFACTS=(
    ["jocky-linux-x64.zip"]="linux_x64"
    ["jocky-windows-x64.zip"]="windows_x64"
    ["jocky-windows-Win32.zip"]="windows_x86"
    ["jocky-macos-arm64.tar.gz"]="macos_arm64"
    ["jocky-macos-x86_64.tar.gz"]="macos_x64"
)

echo "Downloading release artifacts..."
declare -A CHECKSUMS

# Download each artifact and compute checksum
for artifact in "${!ARTIFACTS[@]}"; do
    platform_key="${ARTIFACTS[$artifact]}"
    url="$RELEASE_URL/$artifact"

    echo -n "  $artifact ... "

    # Try to download
    if curl -fsSL -o "$artifact" "$url" 2>/dev/null; then
        # Verify file was downloaded
        if [ -f "$artifact" ]; then
            checksum=$(sha256sum "$artifact" | awk '{print $1}')
            CHECKSUMS[$platform_key]="$checksum"
            echo "✓ ($checksum)"
        else
            echo "✗ (file not created)"
        fi
    else
        echo "✗ (download failed)"
        echo "    URL: $url"
    fi
done

echo ""
echo "Generated checksums:"
echo "===================="
echo ""

# Display checksums in YAML format
echo "checksums:"
for platform_key in "${!CHECKSUMS[@]}"; do
    checksum="${CHECKSUMS[$platform_key]}"
    echo "  $platform_key: \"$checksum\""
done

echo ""
echo "Updating version.yaml..."

# Create temporary file for updates
TEMP_YAML=$(mktemp)
cp "$SCRIPT_DIR/version.yaml" "$TEMP_YAML"

# Update version
sed -i.bak "s/^version: .*/version: \"$VERSION\"/" "$TEMP_YAML"

# Update release date
sed -i.bak "s/^release_date: .*/release_date: \"$(date +%Y-%m-%d)\"/" "$TEMP_YAML"

# Update checksums
for platform_key in "${!CHECKSUMS[@]}"; do
    checksum="${CHECKSUMS[$platform_key]}"
    # Escape special characters for sed
    escaped_checksum=$(echo "$checksum" | sed 's/[\/&]/\\&/g')

    # Update or add the checksum
    if grep -q "  $platform_key:" "$TEMP_YAML"; then
        sed -i.bak "s/  $platform_key: .*/  $platform_key: \"$checksum\"/" "$TEMP_YAML"
    else
        # If checksum key doesn't exist, add it under checksums section
        sed -i.bak "/^checksums:$/a\\  $platform_key: \"$checksum\"" "$TEMP_YAML"
    fi
done

# Remove backup files
rm -f "$TEMP_YAML.bak"

# Copy updated file back
cp "$TEMP_YAML" "$SCRIPT_DIR/version.yaml"
rm "$TEMP_YAML"

echo "✓ version.yaml updated"

# Also update package-specific version files
echo ""
echo "Updating package-specific version files..."

# Update Homebrew formula
if [ -n "${CHECKSUMS[linux_x64]:-}" ]; then
    sed -i.bak "s/sha256 \"[^\"]*\"/sha256 \"${CHECKSUMS[linux_x64]}\"/" "$SCRIPT_DIR/homebrew/jocky.rb"
    echo "✓ homebrew/jocky.rb updated"
    rm -f "$SCRIPT_DIR/homebrew/jocky.rb.bak"
fi

# Update AUR PKGBUILD
if [ -n "${CHECKSUMS[linux_x64]:-}" ]; then
    sed -i.bak "s/sha256sums=('[^']*')/sha256sums=('${CHECKSUMS[linux_x64]}')/" "$SCRIPT_DIR/aur/PKGBUILD"
    echo "✓ aur/PKGBUILD updated"
    rm -f "$SCRIPT_DIR/aur/PKGBUILD.bak"
fi

# Update Chocolatey install script
if [ -n "${CHECKSUMS[windows_x64]:-}" ] && [ -n "${CHECKSUMS[windows_x86]:-}" ]; then
    sed -i.bak "s/\$checksum64 = \"[^\"]*\"/\$checksum64 = \"${CHECKSUMS[windows_x64]}\"/" "$SCRIPT_DIR/chocolatey/tools/chocolateyinstall.ps1"
    sed -i.bak "s/\$checksum32 = \"[^\"]*\"/\$checksum32 = \"${CHECKSUMS[windows_x86]}\"/" "$SCRIPT_DIR/chocolatey/tools/chocolateyinstall.ps1"
    echo "✓ chocolatey/tools/chocolateyinstall.ps1 updated"
    rm -f "$SCRIPT_DIR/chocolatey/tools/chocolateyinstall.ps1.bak"
fi

echo ""
echo "=========================================="
echo "Checksum update complete!"
echo "=========================================="
echo ""
echo "Files updated:"
echo "  - packaging/version.yaml"
echo "  - packaging/homebrew/jocky.rb"
echo "  - packaging/aur/PKGBUILD"
echo "  - packaging/chocolatey/tools/chocolateyinstall.ps1"
echo ""
echo "Next steps:"
echo "1. Review the changes: git diff packaging/"
echo "2. Commit if correct: git add packaging/ && git commit -m 'Update checksums for $TAG'"
echo "3. Push changes: git push origin main"
echo ""
