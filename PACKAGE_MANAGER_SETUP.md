# Package Manager Setup & Release Guide

Complete guide to setting up and managing JOCKY across Homebrew, AUR, and Chocolatey.

## Quick Start

### 1. One-Time Setup (Initial Configuration)

#### Homebrew Tap

```bash
# Create a new repository for the Homebrew tap
gh repo create homebrew-jocky --public --description "Homebrew tap for JOCKY"

# Create the necessary directory structure
git clone https://github.com/devalgupta/homebrew-jocky
cd homebrew-jocky
mkdir -p Formula
echo "# JOCKY Homebrew Tap" > README.md
git add README.md
git commit -m "Initial commit"
git push

# Add GitHub token for CI/CD
gh secret set HOMEBREW_TAP_TOKEN --body "$(gh auth token)" -R devalgupta/JOCKY
```

#### AUR Account

1. Register at https://aur.archlinux.org/register
2. Generate SSH key:
   ```bash
   ssh-keygen -f ~/.ssh/aur_key
   ```
3. Add public key to AUR account
4. Configure SSH:
   ```bash
   cat >> ~/.ssh/config << EOF
   Host aur.archlinux.org
       IdentityFile ~/.ssh/aur_key
       User aur
   EOF
   ```
5. Create AUR repository:
   ```bash
   git clone ssh://aur@aur.archlinux.org/jocky.git
   ```

#### Chocolatey

1. Register account at https://community.chocolatey.org/users/account/Register
2. Generate API key from account settings
3. Add to GitHub secrets:
   ```bash
   gh secret set CHOCO_API_KEY --body "<your-api-key>" -R devalgupta/JOCKY
   ```

### 2. Creating a Release

#### Option A: Using Release Script (Recommended)

```bash
cd /path/to/JOCKY

# Bump version and create tag
./packaging/release.sh 0.2.0

# Push to trigger CI/CD
git push origin main --tags
```

#### Option B: Manual Release

1. Update version in all files:
   ```bash
   # pyproject.toml
   sed -i 's/version = "[^"]*"/version = "0.2.0"/' pyproject.toml
   
   # packaging/version.yaml
   sed -i 's/version: "[^"]*"/version: "0.2.0"/' packaging/version.yaml
   
   # packaging/homebrew/jocky.rb
   sed -i 's/version "[^"]*"/version "0.2.0"/' packaging/homebrew/jocky.rb
   
   # packaging/aur/PKGBUILD
   sed -i 's/pkgver=[^$]*/pkgver=0.2.0/' packaging/aur/PKGBUILD
   
   # packaging/chocolatey/jocky.nuspec
   sed -i 's/<version>[^<]*<\/version>/<version>0.2.0<\/version>/' packaging/chocolatey/jocky.nuspec
   ```

2. Commit and tag:
   ```bash
   git add pyproject.toml packaging/
   git commit -m "Bump version to 0.2.0"
   git tag -a "v0.2.0" -m "Release version 0.2.0"
   git push origin main --tags
   ```

### 3. What Happens Automatically

When you push a tag matching `v*`:

**GitHub Actions (`release.yml` workflow):**
- ✅ Builds Linux binary (x64)
- ✅ Builds macOS binaries (ARM64 and x86_64)
- ✅ Builds Windows binaries (x86 and x64)
- ✅ Computes SHA256 checksums
- ✅ Creates GitHub Release with all artifacts
- ✅ Updates Homebrew tap formula
- ✅ Publishes to Chocolatey gallery
- ✅ Updates AUR PKGBUILD (if SSH key configured)

## Installation Verification After Release

### macOS

```bash
brew tap devalgupta/homebrew-jocky
brew install jocky
jockyc --version
```

### Linux (Arch)

```bash
yay -S jocky
# or
git clone https://aur.archlinux.org/jocky.git
cd jocky
makepkg -si
jockyc --version
```

### Windows

```powershell
choco install jocky
jockyc --version
```

## Testing Before Release

### Test All Package Managers Locally

```bash
cd /path/to/JOCKY

# Run comprehensive test suite
./packaging/test-packages.sh

# Or test specific package manager
./packaging/test-packages.sh --homebrew
./packaging/test-packages.sh --aur
./packaging/test-packages.sh --chocolatey
```

### Test Manual Installation

```bash
./packaging/test-packages.sh --manual
./packaging/test-packages.sh --compile
```

## File Structure Overview

```
JOCKY/
├── packaging/                      # Package manager files
│   ├── README.md                  # Detailed packaging guide
│   ├── release.sh                 # Release automation script
│   ├── version.yaml               # Centralized version config
│   ├── test-packages.sh           # Comprehensive testing
│   ├── homebrew/
│   │   └── jocky.rb              # Homebrew formula
│   ├── aur/
│   │   └── PKGBUILD              # AUR package definition
│   └── chocolatey/
│       ├── jocky.nuspec          # Chocolatey metadata
│       └── tools/
│           ├── chocolateyinstall.ps1
│           └── chocolateyuninstall.ps1
│
├── .github/workflows/
│   ├── ci.yml                     # Continuous integration
│   └── release.yml                # Release automation (NEW)
│
├── pyproject.toml                 # Python project config
├── docs/
│   └── INSTALLATION_GUIDE.md      # User installation guide
└── PACKAGE_MANAGER_SETUP.md       # This file
```

## Troubleshooting

### Release workflow fails

Check GitHub Actions logs:
```bash
# View recent workflow runs
gh run list -w release.yml -L 5

# View logs for specific run
gh run view <run-id> --log
```

### Homebrew formula issues

```bash
# Test formula locally
brew install --build-from-source ./packaging/homebrew/jocky.rb

# Update checksum
brew audit --fix ./packaging/homebrew/jocky.rb
```

### AUR package rejected

Check AUR submission guidelines:
- PKGBUILD follows best practices
- No hardcoded paths
- Proper dependencies listed
- Binary permissions correct

### Chocolatey moderation delays

- Verify package compiles locally: `choco pack packaging/chocolatey/jocky.nuspec`
- Check for policy violations on Chocolatey documentation
- Monitor queue at https://community.chocolatey.org/packages

## Version Management

### Update Procedure

1. **Decide on version number** (follow semantic versioning)
   - `MAJOR.MINOR.PATCH`
   - e.g., `0.1.0` → `0.2.0`

2. **Run release script**
   ```bash
   ./packaging/release.sh 0.2.0
   ```

3. **Verify changes**
   ```bash
   git log -1
   git show --stat
   ```

4. **Push to repository**
   ```bash
   git push origin main --tags
   ```

5. **Monitor CI/CD**
   - Check GitHub Actions for success
   - Verify all artifacts appear on Release page
   - Confirm Homebrew tap updated
   - Monitor Chocolatey approval status

### Version File Reference

The `packaging/version.yaml` file contains:
- Current version number
- Release date
- Download URLs for each platform
- SHA256 checksums for verification
- Package manager specific info
- Build requirements per platform
- Changelog entries

Update manually if CI/CD doesn't auto-update:
```bash
# Get actual checksums from release artifacts
for file in jocky-*.{zip,tar.gz}; do
    sha256sum "$file" >> checksums.txt
done

# Update version.yaml with new checksums
```

## Security Considerations

### Binary Verification

Users should verify downloaded binaries:
```bash
# Download binary and checksum from GitHub Releases
curl -L https://github.com/devalgupta/JOCKY/releases/download/v0.2.0/jocky-linux-x64.zip -O
curl -L https://github.com/devalgupta/JOCKY/releases/download/v0.2.0/jocky-linux-x64.zip.sha256 -O

# Verify
sha256sum -c jocky-linux-x64.zip.sha256
```

### Code Signing

To enable GPG signing:
```bash
# Configure git to sign releases
git config --global user.signingkey <your-key-id>
git config --global commit.gpgsign true
git config --global tag.gpgsign true

# Releases will be automatically signed
```

## Platform-Specific Notes

### macOS

- Formula supports both Intel and Apple Silicon
- Homebrew automatically downloads correct binary
- Dependencies managed via Homebrew

### Linux

- AUR provides source-based installation
- Users get latest features
- Compiled locally for their system
- Optional dependencies for enhanced features

### Windows

- Chocolatey provides binary distribution
- PATH automatically configured
- Python CLI wrapper included
- Easy uninstall via `choco uninstall jocky`

## Maintenance Schedule

| Task | Frequency | Command |
|------|-----------|---------|
| Version update | Per release | `./packaging/release.sh <version>` |
| Dependency check | Monthly | Review in each package file |
| Security update | As needed | Follow normal patch process |
| Formula audit | Quarterly | `brew audit jocky` |
| AUR update | Per release | Automatic via CI/CD |
| Chocolatey publish | Per release | Automatic via CI/CD |

## Getting Help

1. **Package Manager Issues:**
   - Homebrew: https://docs.brew.sh
   - AUR: https://wiki.archlinux.org/title/Arch_User_Repository
   - Chocolatey: https://docs.chocolatey.org

2. **JOCKY Issues:**
   - GitHub Issues: https://github.com/devalgupta/JOCKY/issues
   - Documentation: `docs/` directory

3. **Release Questions:**
   - Review `.github/workflows/release.yml`
   - Check `packaging/README.md`
   - Examine example releases on GitHub

## Next Steps

1. Complete one-time setup (see section 1)
2. Create first release with `./packaging/release.sh`
3. Verify packages install correctly
4. Monitor feedback from users
5. Update documentation based on issues

---

**Last Updated:** 2026-09-29
**Status:** Production Ready
