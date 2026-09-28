#!/bin/bash
# JOCKY Release Script
# Usage: ./release.sh <version>
# Example: ./release.sh 0.1.0

set -euo pipefail

if [ $# -ne 1 ]; then
    echo "Usage: $0 <version>"
    echo "Example: $0 0.1.0"
    exit 1
fi

VERSION=$1
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo "=========================================="
echo "JOCKY Release Script v$VERSION"
echo "=========================================="

# Validate version format
if ! [[ $VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo "Error: Invalid version format. Use semantic versioning (e.g., 0.1.0)"
    exit 1
fi

# Check git status
cd "$REPO_ROOT"
if ! git diff --quiet; then
    echo "Error: Uncommitted changes detected. Commit or stash changes."
    exit 1
fi

if ! git diff --cached --quiet; then
    echo "Error: Staged changes detected. Commit changes first."
    exit 1
fi

# Update version in files
echo "Updating version in configuration files..."

# Update pyproject.toml
sed -i.bak "s/version = \"[^\"]*\"/version = \"$VERSION\"/" "$REPO_ROOT/pyproject.toml"
rm "$REPO_ROOT/pyproject.toml.bak"

# Update packaging/version.yaml
sed -i.bak "s/version: \"[^\"]*\"/version: \"$VERSION\"/" "$REPO_ROOT/packaging/version.yaml"
rm "$REPO_ROOT/packaging/version.yaml.bak"

# Update Homebrew formula
sed -i.bak "s/version \"[^\"]*\"/version \"$VERSION\"/" "$REPO_ROOT/packaging/homebrew/jocky.rb"
rm "$REPO_ROOT/packaging/homebrew/jocky.rb.bak"

# Update AUR PKGBUILD
sed -i.bak "s/pkgver=[^$]*/pkgver=$VERSION/" "$REPO_ROOT/packaging/aur/PKGBUILD"
rm "$REPO_ROOT/packaging/aur/PKGBUILD.bak"

# Update Chocolatey nuspec
sed -i.bak "s/<version>[^<]*<\/version>/<version>$VERSION<\/version>/" "$REPO_ROOT/packaging/chocolatey/jocky.nuspec"
rm "$REPO_ROOT/packaging/chocolatey/jocky.nuspec.bak"

echo "Version updated to $VERSION"

# Commit version updates
git add pyproject.toml packaging/
git commit -m "Bump version to $VERSION"

# Create git tag
echo "Creating git tag v$VERSION..."
git tag -a "v$VERSION" -m "Release version $VERSION"

echo ""
echo "=========================================="
echo "Release preparation complete!"
echo "=========================================="
echo ""
echo "Next steps:"
echo "1. Review the changes: git log --oneline -5"
echo "2. Push to repository: git push origin main --tags"
echo "3. GitHub Actions will automatically build and publish packages"
echo ""
echo "To undo these changes:"
echo "  git tag -d v$VERSION"
echo "  git reset HEAD~1"
echo ""
