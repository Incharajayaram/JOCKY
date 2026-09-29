# JOCKY Package Manager Support

This directory contains package manager definitions and release infrastructure for distributing JOCKY across multiple platforms.

## Directory Structure

```
packaging/
├── README.md                  # This file
├── release.sh                 # Release script for version bumping
├── version.yaml               # Centralized version configuration
├── homebrew/
│   └── jocky.rb              # Homebrew formula (macOS/Linux)
├── aur/
│   └── PKGBUILD              # AUR package definition (Arch Linux)
└── chocolatey/
    ├── jocky.nuspec          # Chocolatey package metadata
    └── tools/
        ├── chocolateyinstall.ps1    # Installation script
        └── chocolateyuninstall.ps1  # Uninstallation script
```

## Package Managers Supported

### 1. Homebrew (macOS & Linux)

**Repository:** homebrew-jocky (tap)
**Maintainer:** Deval Gupta

**Installation:**
```bash
brew tap devalgupta/homebrew-jocky
brew install jocky
```

**File:** `homebrew/jocky.rb`

**Key Features:**
- Automatic dependency resolution (CMake, LLVM, Python, GCC)
- Binary installation with wrapper script
- Version-specific LLVM compatibility
- Supports both Intel and Apple Silicon Macs

**Maintenance:**
- Updates handled via GitHub Actions
- Homebrew tap auto-updates when releases are pushed
- Formula checksums automatically verified

### 2. AUR (Arch Linux)

**Repository:** https://aur.archlinux.org/jocky.git
**Maintainer:** Deval Gupta <deval@awesomesleep.in>

**Installation:**
```bash
yay -S jocky
# or
git clone https://aur.archlinux.org/jocky.git
cd jocky
makepkg -si
```

**File:** `aur/PKGBUILD`

**Key Features:**
- Full source compilation
- AUR best practices compliance
- Optional dependencies for enhanced features
- Systemwide installation with documentation
- Man page support

**Maintenance:**
- Manual updates to AUR when new releases occur
- Pushes via SSH to AUR infrastructure
- PKGBUILD version auto-updated via CI/CD

### 3. Chocolatey (Windows)

**Repository:** https://community.chocolatey.org/packages/jocky
**Maintainer:** Deval Gupta

**Installation:**
```powershell
choco install jocky
```

**Files:**
- `chocolatey/jocky.nuspec` - Package metadata
- `chocolatey/tools/chocolateyinstall.ps1` - Installation script
- `chocolatey/tools/chocolateyuninstall.ps1` - Uninstallation script

**Key Features:**
- Automatic PATH integration
- Binary distribution for Windows x86/x64
- Wrapper script for Python CLI support
- Automatic version verification

**Maintenance:**
- Automatic push via GitHub Actions on release
- Requires Chocolatey API key in GitHub secrets
- Package moderation by Chocolatey community team

## Release Process

### Manual Release (One-time setup)

1. **Initial Setup (First Time Only):**

   **Homebrew Tap:**
   ```bash
   # Create personal GitHub repository: homebrew-jocky
   # Add to GitHub as deploy key for CI/CD
   gh repo create homebrew-jocky
   git clone https://github.com/devalgupta/homebrew-jocky
   cd homebrew-jocky
   mkdir -p Formula
   # CI/CD will populate formula
   ```

   **AUR Account:**
   ```bash
   # Register at https://aur.archlinux.org
   # Generate SSH key and add to AUR account
   ssh-keygen -f ~/.ssh/aur
   # Configure ~/.ssh/config for aur.archlinux.org
   ```

   **Chocolatey:**
   - Register account at https://community.chocolatey.org
   - Generate API key
   - Add to GitHub secrets as `CHOCO_API_KEY`

2. **Add GitHub Secrets:**
   ```bash
   gh secret set HOMEBREW_TAP_TOKEN --body "$(gh auth token)" -R devalgupta/JOCKY
   gh secret set CHOCO_API_KEY -R devalgupta/JOCKY
   ```

### Automated Release Process

1. **Trigger Release:**
   ```bash
   cd /path/to/JOCKY
   ./packaging/release.sh 0.2.0
   ```

   This script:
   - Updates version in `pyproject.toml`, `packaging/version.yaml`, and all package files
   - Commits changes
   - Creates git tag `v0.2.0`

2. **Push to Repository:**
   ```bash
   git push origin main --tags
   ```

3. **GitHub Actions Automatically:**
   - Builds binaries for Linux, macOS (x86_64 & ARM64), Windows (x86 & x64)
   - Computes SHA256 checksums
   - Creates GitHub Release with all artifacts
   - Updates Homebrew tap formula
   - Publishes to Chocolatey
   - Updates AUR PKGBUILD (if SSH key configured)

4. **Verification:**
   - Check GitHub Releases page for all artifacts
   - Wait for Homebrew to sync (automatic)
   - Monitor Chocolatey moderation queue

## Version Management

**File:** `packaging/version.yaml`

This YAML file serves as the single source of truth for version information across all package managers:

```yaml
version: "0.2.0"
release_date: "2026-09-29"
github_repo: "devalgupta/JOCKY"

download_urls:
  github_release_base: "https://github.com/devalgupta/JOCKY/releases/download/v${version}"
  linux_x64: "jocky-linux-x64.zip"
  windows_x64: "jocky-windows-x64.zip"
  # ... more platforms

checksums:
  linux_x64: "0d1c95f68cd12d75a3dd3fa5a5b7e4d3a2b8c9f1e2d3c4b5a6f7e8d9c0a1b2c"
  # ... more platforms
```

**Usage:**
- Automatically populated by CI/CD during releases
- Used by package managers for verification
- Reference for documentation

## Building Local Packages (Testing)

### Build Homebrew Locally

```bash
cd packaging/homebrew
brew install --build-from-source ./jocky.rb
```

### Build AUR Locally

```bash
cd packaging/aur
makepkg -si
```

### Build Chocolatey Locally

```powershell
cd packaging\chocolatey
choco pack jocky.nuspec
choco install jocky -s .
```

## Troubleshooting

### Homebrew Formula Issues

**Problem:** Formula not found
```bash
brew tap devalgupta/homebrew-jocky
brew update
brew install jocky
```

**Problem:** Checksum mismatch
- Verify URL is accessible
- Update SHA256 in formula:
```bash
curl -L <url> | sha256sum
```

### AUR Package Issues

**Problem:** Package not found on AUR
- Ensure PKGBUILD is committed to AUR repository
- Try manual installation: `git clone https://aur.archlinux.org/jocky.git`

**Problem:** Build failures
- Check dependencies in PKGBUILD match system packages
- Test locally: `makepkg -si`

### Chocolatey Package Issues

**Problem:** Installation fails
- Ensure Windows dependencies are met
- Download manually from releases and use local package

**Problem:** Permission denied
- Run PowerShell as Administrator
- Check Windows Defender isn't blocking

## CI/CD Integration

**Workflow File:** `.github/workflows/release.yml`

Triggered on:
- Git tags matching `v*` pattern (e.g., `v0.2.0`)

**Jobs:**
1. `build-linux` - Builds Linux x64 binary
2. `build-macos` - Builds macOS ARM64 and x64 binaries
3. `build-windows` - Builds Windows x64 and x86 binaries
4. `github-release` - Creates GitHub Release with all artifacts
5. `update-homebrew` - Updates Homebrew tap formula
6. `update-chocolatey` - Publishes to Chocolatey gallery
7. `create-release-notes` - Generates release documentation

## Installation Verification

After any package manager installation:

```bash
# Check binary version
jockyc --version

# Check CLI version
jocky --version

# Get help
jocky --help

# Test compilation
echo 'int main() { return 0; }' > test.c
jockyc test.c -o test
```

## Maintenance Schedule

| Task | Frequency | Owner |
|------|-----------|-------|
| Version updates | Per release | Automated via release.sh |
| Dependency updates | Monthly | Manual review |
| Security patches | As needed | Manual review + automated push |
| Chocolatey approval | Per release | Community moderation |
| Documentation updates | Per release | Automated via CI/CD |

## Adding a New Package Manager

To add support for a new package manager:

1. Create package definition file in new directory
2. Add build/packaging logic to `.github/workflows/release.yml`
3. Add version reference to `packaging/version.yaml`
4. Update `docs/INSTALLATION_GUIDE.md`
5. Add testing steps to local build process
6. Document in this README

## Security

**Binary Integrity:**
- All binaries signed with SHA256 checksums
- Checksums distributed in GitHub Releases
- Package managers verify checksums before installation

**Source Verification:**
- Git commits GPG-signed (when available)
- Tags signed for releases
- AUR and Homebrew maintain their own signatures

## Support

For issues with:
- **JOCKY Installation/Usage:** [GitHub Issues](https://github.com/devalgupta/JOCKY/issues)
- **Homebrew/AUR/Chocolatey:** Check package manager documentation
- **Release Process:** See CI/CD logs in Actions tab

## License

All packaging files are distributed under the same license as JOCKY (MIT).
