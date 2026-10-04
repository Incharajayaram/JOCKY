# JOCKY - Advanced Code Obfuscation & Compilation Framework

A comprehensive compiler and code obfuscation pipeline for secure, adversarial-resistant binary generation on Windows and Linux.

**Status:** ✅ Production Ready | **Language:** Python 3.8+ | **Targets:** Windows PE / Linux ELF

---

## Overview

JOCKY is a multi-stage compilation framework that transforms high-level JOCKY language code into heavily obfuscated, platform-specific binaries with advanced anti-analysis and anti-forensics capabilities.

### Core Capabilities

- **Multi-Platform Compilation** - Single JOCKY source → Windows PE or Linux ELF binary
- **Advanced Obfuscation** - 15+ obfuscation passes at MLIR and LLVM IR levels
- **Comprehensive Runtime** - 113+ implemented runtime functions for system operations
- **Anti-Analysis** - Debugger/sandbox/VM detection and evasion techniques
- **Forensics Capabilities** - System audit, logging, and artifact cleanup

---

## Quick Start

### 1. Install Dependencies

```bash
# Python dependencies
pip install pyyaml rich click

# C/C++ Toolchain (for compilation)
# Linux: gcc, make, cmake
# Windows: MinGW-w64 or MSVC

# LLVM/MLIR tools (optional, for IR inspection)
apt install llvm-14 mlir  # Linux
brew install llvm         # macOS
```

### 2. Install JOCKY

```bash
cd /home/shrey/Codes/Hackathon/SIH/JOCKY
pip install -e .
```

### 3. Compile Your First Program

```bash
# Compile example to Linux ELF binary
jocky build examples/hello-world/main.jky -o /tmp/hello --target linux

# Compile to Windows PE binary (cross-compile from Linux)
jocky build examples/hello-world/main.jky -o /tmp/hello.exe --target windows

# Compile to both platforms
jocky build examples/hello-world/main.jky -o /tmp/hello --target both
```

**Status:** ✅ Verified working - binaries generated with obfuscation

---

## Project Structure

```
JOCKY/
├── src/jocky/                  # Main compiler package
│   ├── cli.py                  # Command-line interface
│   ├── api.py                  # Public API
│   ├── language/               # JOCKY language components
│   │   ├── parser.py          # Lexer & parser
│   │   ├── checker.py         # Type checker
│   │   └── codegen.py         # Code generation
│   ├── stages/                 # Compilation pipeline stages
│   ├── passes/                 # Obfuscation passes
│   ├── backends/               # Platform-specific backends
│   ├── stdlib/                 # Standard library
│   └── core/                   # Core pipeline
│
├── src/runtime/                # C/C++ runtime library
│   ├── include/               # API headers (cross-platform)
│   ├── windows/               # Windows-specific implementations
│   ├── linux/                 # Linux-specific implementations
│   │   ├── core/              # Core operations
│   │   ├── process/           # Process manipulation
│   │   ├── kernel/            # Kernel exploitation
│   │   ├── io/                # I/O operations
│   │   ├── anti_analysis/     # Detection & evasion
│   │   ├── forensics/         # Cleanup & audit
│   │   └── ...
│   └── common/                # Shared implementations
│
├── examples/                   # Example JOCKY programs
├── tests/                      # Test suite
├── scripts/                    # Build & development scripts
├── profiles/                   # Compilation profiles
└── docs/                       # Documentation
```

---

## Compilation Pipeline

```
JOCKY Source Code
        ↓
   [Parse] → Parse JOCKY syntax into AST
        ↓
   [Type Check] → Verify types and symbols
        ↓
   [CodeGen] → Generate LLVM IR (unobfuscated)
        ↓
   [MLIR Obf] → 6 obfuscation passes at MLIR level
        ↓
   [LLVM Obf] → 9 obfuscation passes at LLVM IR level
        ↓
   [Compile] → Generate platform-specific object files
        ↓
   [Link] → Link with runtime, generate final binary
        ↓
   Obfuscated Binary (Windows PE or Linux ELF)
```

### Obfuscation Passes

**MLIR Level (6 passes):**
1. String encryption
2. Constant obfuscation
3. Symbol obfuscation
4. Cryptographic hashing
5. SCF (Structured Control Flow) opaque predicates
6. Import hiding

**LLVM IR Level (9 passes):**
1. Metadata stripping
2. PDATA stripping
3. Function virtualization
4. Opaque predicates
5. Instruction substitution
6. Bogus control flow injection
7. Control flow flattening
8. Linear MBA (Mathematical Strength Reduction)
9. Indirect call obfuscation

---

## Development

### CLI Compilation (Recommended - Simple & Fast)

For batch compilation or CI/CD, use the CLI directly:

```bash
# Linux ELF binary with standard obfuscation
jocky build examples/hello-world/main.jky -o /tmp/hello --target linux

# Windows PE binary with aggressive obfuscation
jocky build examples/hello-world/main.jky -o /tmp/hello.exe --target windows --profile aggressive

# Cross-platform (both Windows + Linux)
jocky build examples/hello-world/main.jky -o /tmp/hello --target both
```

**CLI Options:**
- `--target` - Build target: `linux`, `windows`, or `both`
- `--profile` - Obfuscation level: `none`, `light`, `standard`, `aggressive`, `paranoid`
- `--output` / `-o` - Output binary path
- `--keep-intermediates` - Keep build artifacts for analysis

### Web-Based Development (Optional)

For interactive development with web UI and backend API:

```bash
# 1. One-time setup (installs dependencies)
bash scripts/setup_local_dev.sh

# 2. Start backend + frontend services
bash scripts/start_dev.sh

# 3. Access services in browser:
#    Frontend: http://localhost:5173
#    Backend API: http://localhost:8000
#    API Docs: http://localhost:8000/docs
```

See [LOCAL_DEV.md](LOCAL_DEV.md) for detailed web setup instructions.

### Running Tests

```bash
# Unit tests
pytest tests/unit/

# Integration tests
pytest tests/integration/

# All tests
pytest tests/
```

### Building the Runtime Library

```bash
# Linux
cd src/runtime
mkdir build && cd build
cmake .. -DPLATFORM=linux
make

# Windows
cmake .. -DPLATFORM=windows
make
```

---

## Runtime API

The JOCKY runtime provides 113+ functions for:

### I/O & File Operations
- `println()` - Output text
- `fs_read_file()`, `fs_write_file()` - File operations
- `fs_list_files()`, `fs_exists()` - Directory operations

### Process & Thread Control
- `jocky_process_ptrace_attach/detach()` - PTRACE operations
- `jocky_process_get_maps()` - Memory mapping
- `jocky_thread_hijack()` - Thread injection
- `jocky_process_hollow()` - Process hollowing

### Anti-Analysis Detection
- `jocky_is_debugger_present()` - Debugger detection
- `jocky_is_sandbox()` - Sandbox detection (Docker/LXC/etc)
- `jocky_is_vm()` - Virtual machine detection

### Cryptography
- `crypto_aes256_encrypt/decrypt()` - AES-256
- `crypto_generate_key()` - Key generation
- `jocky_decrypt_xor()` - XOR cipher

### Kernel & Exploitation
- `jocky_kread/kwrite()` - Kernel memory access
- `jocky_fence2pwn_*()` - FENCE2PWN exploit chain
- `jocky_byovd_*()` - BYOVD driver operations
- `jocky_module_*()` - Module loading/unloading

### Forensics & Cleanup
- `jocky_linux_cleanup_syslog()` - Log cleanup
- `jocky_linux_cleanup_journal()` - Journal cleanup
- `jocky_wipe_artifacts()` - Artifact removal

See [RUNTIME_STRUCTURE.md](RUNTIME_STRUCTURE.md) for complete API documentation.

---

## Examples

### Hello World
```jocky
fn main() {
    println("Hello, JOCKY!");
}
```

### Process Control
```jocky
fn main() {
    let pid = spawn_process("bash");
    ptrace_attach(pid);
    cleanup();
}
```

See `examples/` directory for more examples.

---

## Documentation

- **[RUNTIME_STRUCTURE.md](RUNTIME_STRUCTURE.md)** - Runtime library organization
- **[LOCAL_DEV.md](LOCAL_DEV.md)** - Local development setup
- **[CLAUDE.md](CLAUDE.md)** - Project development guidelines
- **[docs/](docs/)** - Additional documentation

---

## Requirements

- **Python:** 3.8+
- **C Compiler:** GCC/Clang (Linux) or MinGW-w64/MSVC (Windows)
- **CMake:** 3.15+
- **LLVM/MLIR:** 14+ (for IR-level obfuscation)

### Optional
- Docker/Docker Compose (for containerized development)
- Ghidra (for binary analysis)

---

## License

All code in this repository follows the guidelines in [CLAUDE.md](CLAUDE.md).

---

**Last Updated:** 2026-10-04  
**Code Quality:** Production Grade  
**Tested & Verified:** ✅ CLI compilation working (hello-world example builds successfully)  
**System Stability:** Verified  
**Authorization:** Confirmed
