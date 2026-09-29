# JOCKY Docker Build System

Complete portable LLVM/MLIR toolchain with automatic Windows + Linux cross-compilation support.

## Quick Start

### 1. Build Docker Image (First Time Only)

```bash
make -f Makefile.docker docker-build
```

This builds a Docker image containing:
- Pre-built LLVM/MLIR toolchain (clang-22, opt, mlir-opt, mlir-translate)
- 11 LLVM IR obfuscation passes (boguscf, flattening, substitution, split, linear-mba, opaque-pred, strip-signature, pdata-strip, indirect-call, anti-debug, virtualize)
- 6 MLIR obfuscation passes (string-encrypt, symbol-obfuscate, constant-obfuscate, crypto-hash, scf-obfuscate, import-obfuscate)
- MinGW-w64 cross-compiler for Windows
- All JOCKY Python dependencies

**Time:** ~5-10 minutes (depends on internet speed)

### 2. Compile a JOCKY Script

```bash
# Compile to both Windows (.exe) and Linux (ELF)
make -f Makefile.docker docker-compile-jky SOURCE=examples/research_chain_with_drivers.jky

# With custom obfuscation profile
make -f Makefile.docker docker-compile-jky SOURCE=examples/research_chain_with_drivers.jky PROFILE=aggressive

# Custom output directory
make -f Makefile.docker docker-compile-jky SOURCE=examples/research_chain_with_drivers.jky OUTPUT=/tmp/my_output
```

### 3. Test the Compiled Binary

```bash
# Test Linux binary
make -f Makefile.docker docker-test-linux

# Test Windows binary with Wine
make -f Makefile.docker docker-test-windows
```

## Complete Pipeline: One Command

```bash
# Build image + compile both targets + run tests
make -f Makefile.docker compile
```

## Obfuscation Profiles

| Profile | Passes | Use Case |
|---------|--------|----------|
| `none` | No obfuscation | Debug/testing |
| `light` | 2-3 passes | Minimal overhead |
| `standard` | 5 passes (default) | Production |
| `aggressive` | 8+ passes | High evasion need |
| `paranoid` | All passes, multiple iterations | Max evasion |

Quick-start commands:
```bash
make -f Makefile.docker light      # Light obfuscation
make -f Makefile.docker aggressive # Aggressive obfuscation
make -f Makefile.docker paranoid   # Maximum obfuscation
```

## Full Build Pipeline (Automated)

The Docker container automatically runs this complete pipeline for **each target**:

```
JOCKY Source (.jky)
    ↓
[1] ParseStage
    - Lexical analysis
    - Syntax tree generation
    - Type checking
    ↓
[2] LowerIRStage
    - Lower to MLIR (memref dialect)
    - Generate intermediate representation
    ↓
[3] MLIRObfuscateStage
    - memref.expand_strided_metadata (hide array access patterns)
    - affine-loop-invariant-code-motion (move code out of loops)
    - affine.pipeline (unroll and pipeline loops)
    ↓
[4] IRObfuscateStage (LLVM IR-level obfuscation)
    - boguscf (bogus control flow)
    - flattening (flatten nested control flow)
    - substitution (variable substitution)
    - linear-mba (linear math/boolean algebra obfuscation)
    - opaque-pred (opaque predicates)
    ↓
[5] LinkStage
    - Link with jocky_rt (kernel APIs)
    - Link with jocky_crypto (encryption)
    - Link with jocky_exfil (exfiltration)
    - Link with jocky_evasion (anti-analysis)
    - Link with jocky_linux or jocky_windows runtime
    ↓
[6] PackStage
    - Optional: UPX packing (ELF)
    - Optional: Code encryption (PE)
    ↓
Optimized Executable
    - Linux: ELF 64-bit (fully linked)
    - Windows: PE 64-bit (with import tables)
```

## Docker Volumes

The Docker container mounts:

| Host Path | Container Path | Purpose | Access |
|-----------|----------------|---------|--------|
| `.` (repo) | `/workspace/jocky` | JOCKY source | Read-only |
| `./build` | `/workspace/build` | Build artifacts | Read-write |
| `./dist` | `/workspace/output` | Compiled executables | Read-write |
| `~/.jocky/models` | `/root/.jocky/models` | ML models | Read-only |
| Docker volume | `/root/.cache` | Build cache | Persistent |

## Environment Variables

Control compilation behavior:

```bash
# Inside docker compose run
export JOCKY_PROFILE=aggressive       # Obfuscation profile
export JOCKY_TARGETS=linux,windows    # Targets to build
export JOCKY_OPT_LEVEL=2              # LLVM optimization level (0-3)
export JOCKY_LTO=thin                 # Link-time optimization (thin/full/off)
export JOCKY_KEEP_INTERMEDIATES=true  # Keep build artifacts
export JOCKY_ENABLE_OBFUSCATION=true  # Enable MLIR/LLVM obfuscation
export JOCKY_ENABLE_PACKING=true      # Enable UPX/encryption packing
```

## Advanced Usage

### Interactive Development

```bash
# Open shell inside container
make -f Makefile.docker docker-shell

# Inside container:
cd /workspace/jocky
python3 -m jocky build examples/test.jky --target linux --profile standard
```

### View Build Logs

```bash
make -f Makefile.docker docker-logs
```

### Clean Everything

```bash
make -f Makefile.docker docker-clean
```

This removes:
- Docker containers
- Docker images
- Local build artifacts (`./build`, `./dist`)

## Standalone Docker Commands

If not using Makefile:

```bash
# Build image
docker compose build jocky-compiler

# Compile a JOCKY script
docker compose run --rm \
  -v $(pwd)/examples/test.jky:/tmp/source.jky:ro \
  jocky-compiler \
  python3 -m jocky build /tmp/source.jky --target linux

# Interactive shell
docker compose run --rm jocky-compiler /bin/bash

# View logs
docker compose logs -f jocky-compiler
```

## Testing Compiled Binaries

### Linux Binary (Native)

```bash
./dist/research_chain_with_drivers
```

### Windows Binary (via Wine)

```bash
docker compose run --rm \
  --profile test-windows \
  -v $(pwd)/dist:/workspace/output:ro \
  jocky-wine \
  wine /workspace/output/research_chain_with_drivers.exe
```

Requires Wine installed. Install with:
```bash
sudo apt-get install wine wine32 wine64
```

## System Requirements

| Component | Minimum | Recommended |
|-----------|---------|------------|
| Docker | 20.10+ | Latest |
| Docker Compose | 1.29+ | 2.0+ |
| RAM | 4 GB | 8+ GB |
| Disk | 5 GB | 15+ GB |
| CPU | 2 cores | 4+ cores |

## Troubleshooting

### Docker image build fails

```bash
# Clear cache and rebuild
docker system prune -a
make -f Makefile.docker docker-build
```

### "LLVM plugin not found" error

The LLVM obfuscation plugin is loaded from the pre-built toolchain at `/workspace/jocky/toolchain/lib/`. If missing:

```bash
# Inside container
ls -la /workspace/jocky/toolchain/lib/LLVMObfuscation*
ls -la /workspace/jocky/toolchain/lib/MLIRObfuscation*
```

### Windows binary won't run in Wine

```bash
# Ensure Wine is configured
docker compose run --rm jocky-wine winecfg

# Test with simple binary
docker compose run --rm jocky-wine wine cmd /c echo "Wine works"
```

### Out of memory during compilation

Increase Docker memory limit in docker compose.yml:

```yaml
deploy:
  resources:
    limits:
      memory: 16G    # Increase from 8G
```

## Performance Tips

1. **Use build cache:** Docker caches layers, so rebuilds are fast
2. **Parallel compilation:** `JOCKY_OPT_LEVEL=0` for faster debug builds
3. **LTO optimization:** `JOCKY_LTO=thin` for balanced speed/size

## Integration with CI/CD

Use in GitHub Actions / GitLab CI:

```yaml
# Example GitHub Actions workflow
build-with-jocky:
  runs-on: ubuntu-latest
  steps:
    - uses: actions/checkout@v3
    - name: Build JOCKY
      run: |
        docker compose build jocky-compiler
        make -f Makefile.docker docker-compile-jky SOURCE=examples/test.jky
    - name: Upload artifacts
      uses: actions/upload-artifact@v3
      with:
        name: jocky-binaries
        path: dist/
```

## Documentation

- **Full API Reference:** See `src/runtime/RUNTIME_API.md`
- **JOCKY Language Syntax:** See `docs/` directory
- **Obfuscation Passes:** See `src/jocky/passes/`
- **Build Configuration:** See `CMakeLists.txt`

## Support

For issues:
1. Check Docker logs: `make -f Makefile.docker docker-logs`
2. Rebuild from scratch: `make -f Makefile.docker docker-clean && make -f Makefile.docker docker-build`
3. Report issues with: `docker version` and `docker compose version` output

## License

See LICENSE file in repository root.
