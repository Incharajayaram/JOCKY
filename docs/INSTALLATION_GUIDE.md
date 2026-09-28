# JOCKY Installation Guide

This guide covers installation of JOCKY across multiple platforms using package managers and manual installation.

## Table of Contents

- [macOS Installation](#macos-installation)
- [Linux Installation](#linux-installation)
- [Windows Installation](#windows-installation)
- [Manual Installation](#manual-installation)
- [Verification](#verification)
- [Troubleshooting](#troubleshooting)

---

## macOS Installation

### Homebrew

The easiest way to install JOCKY on macOS is via Homebrew:

```bash
# Add the JOCKY tap
brew tap devalgupta/homebrew-jocky

# Install JOCKY
brew install jocky

# Verify installation
jockyc --version
```

### Manual Installation

See [Manual Installation](#manual-installation) section below.

---

## Linux Installation

### Arch Linux (AUR)

For Arch Linux users, JOCKY is available in the AUR:

```bash
# Using yay
yay -S jocky

# Using makepkg directly
git clone https://aur.archlinux.org/jocky.git
cd jocky
makepkg -si

# Verify installation
jockyc --version
```

### Ubuntu/Debian

For Ubuntu and Debian-based systems, use the manual installation or build from source:

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y cmake llvm-14 llvm-14-dev clang-14 python3.10 build-essential

# Clone repository
git clone https://github.com/devalgupta/JOCKY.git
cd JOCKY

# Build and install (see Manual Installation)
```

### Fedora/RHEL

```bash
# Install dependencies
sudo dnf install -y cmake llvm llvm-devel clang python3 gcc-c++ make

# Clone and build (see Manual Installation)
```

---

## Windows Installation

### Chocolatey

For Windows users with Chocolatey package manager:

```powershell
# Install JOCKY
choco install jocky

# Verify installation
jockyc --version
```

### Manual Installation

1. Download the Windows release from [GitHub Releases](https://github.com/devalgupta/JOCKY/releases)
2. Extract the zip file to your desired location (e.g., `C:\Program Files\JOCKY`)
3. Add the binary directory to your PATH:
   - Right-click "This PC" or "My Computer" → Properties
   - Click "Advanced system settings"
   - Click "Environment Variables"
   - Under "System variables", select "Path" and click "Edit"
   - Add `C:\Program Files\JOCKY\bin`
   - Click OK and restart your terminal

4. Verify installation:
```powershell
jockyc --version
```

---

## Manual Installation

### Prerequisites

- **CMake** >= 3.20
- **LLVM** >= 14 (with development libraries)
- **Python** >= 3.10
- **C++ Compiler** (GCC 9+, Clang 10+, or MSVC 2019+)
- **Build tools**:
  - Linux/macOS: `make`, standard build essentials
  - Windows: Visual Studio Build Tools or full Visual Studio

### Installation Steps

1. **Clone the repository:**
```bash
git clone https://github.com/devalgupta/JOCKY.git
cd JOCKY
```

2. **Install dependencies:**

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install -y cmake llvm-14-dev clang-14 python3.10 build-essential
```

**Linux (Fedora/RHEL):**
```bash
sudo dnf install -y cmake llvm llvm-devel clang python3 gcc-c++ make
```

**macOS:**
```bash
brew install cmake llvm@14 python@3.10 gcc
```

**Windows (using vcpkg):**
```powershell
vcpkg install cmake:x64-windows llvm:x64-windows python:x64-windows
```

3. **Build the compiler:**
```bash
mkdir -p compiler/build
cd compiler/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DLLVM_ROOT=/path/to/llvm
cmake --build . -j$(nproc)
cd ../..
```

4. **Install Python dependencies:**
```bash
pip install -e .
```

5. **Add to PATH (optional):**

**Linux/macOS:**
```bash
# Add to ~/.bashrc or ~/.zshrc
export PATH="$PATH:$(pwd)/compiler/build"
export PATH="$PATH:$(python -m site --user-scripts)"
```

**Windows (PowerShell):**
```powershell
$env:PATH += ";$(pwd)\compiler\build"
```

6. **Verify installation:**
```bash
jockyc --version
jocky --version
```

---

## Verification

To verify your JOCKY installation is working correctly:

### Quick Test
```bash
# Check binary version
jockyc --version

# Check Python CLI version
jocky --version

# Get help information
jocky --help
jockyc --help
```

### Compile a Simple Program

Create a test file `hello.c`:
```c
#include <stdio.h>

int main() {
    printf("Hello, JOCKY!\n");
    return 0;
}
```

Compile with JOCKY:
```bash
jockyc hello.c -o hello

# Run the obfuscated binary
./hello
```

### Run Test Suite

If you installed from source:
```bash
cd JOCKY
pytest tests/ -v
```

---

## Troubleshooting

### "command not found: jockyc"

**Solution:** Add the compiler binary directory to your PATH.

**Linux/macOS:**
```bash
export PATH="$PATH:$(pwd)/compiler/build"
source ~/.bashrc  # or ~/.zshrc
```

**Windows:**
1. Search for "Environment Variables" in Settings
2. Click "Edit the system environment variables"
3. Add the JOCKY binary directory to the Path variable
4. Restart your terminal

### LLVM not found

**Error:** `CMake Error: Could not find LLVM`

**Solution:**
```bash
# Specify LLVM location explicitly
cmake .. -DLLVM_ROOT=/usr/lib/llvm-14

# Or set environment variable
export LLVM_ROOT=/usr/lib/llvm-14
```

**macOS (Homebrew):**
```bash
export LLVM_ROOT=$(brew --prefix llvm@14)
```

### Python dependencies missing

**Error:** `ModuleNotFoundError: No module named 'click'`

**Solution:**
```bash
pip install -e .
# or
pip install click rich pyyaml
```

### Build fails on macOS with ARM64

**Solution:** Ensure you have the ARM64-compatible LLVM:
```bash
brew install llvm@14 --HEAD
export LLVM_ROOT=$(brew --prefix llvm@14)
```

### Windows build fails

**Solutions:**
1. Ensure Visual Studio 2019 or later is installed
2. Install LLVM for Windows: https://releases.llvm.org/download.html
3. Set LLVM_ROOT environment variable:
```powershell
$env:LLVM_ROOT = "C:\Program Files\LLVM"
```

### Permission denied on Linux/macOS

**Error:** `Permission denied: './jockyc'`

**Solution:**
```bash
chmod +x compiler/build/jockyc
```

### Chocolatey package download fails

**Solution:** Check your internet connection and ensure GitHub is accessible. Try again:
```powershell
choco install jocky -f  # Force reinstall
```

---

## Uninstallation

### Homebrew
```bash
brew uninstall jocky
brew untap devalgupta/homebrew-jocky
```

### AUR
```bash
yay -R jocky
# or
makepkg -si --clean
```

### Chocolatey
```powershell
choco uninstall jocky
```

### Manual Installation
Simply delete the installation directory and remove it from your PATH.

---

## Getting Help

If you encounter issues:

1. Check the [Troubleshooting](#troubleshooting) section above
2. Review the [Build Documentation](./BUILD.md)
3. Open an issue on [GitHub](https://github.com/devalgupta/JOCKY/issues)
4. Check the [README](../README.md) for additional information

---

## Next Steps

After installation, check out:
- [Getting Started](./GETTING_STARTED.md)
- [CLI Reference](./CLI_REFERENCE.md)
- [Examples](../examples/)
- [Runtime API Documentation](./RUNTIME_API.md)
