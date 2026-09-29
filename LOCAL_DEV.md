# JOCKY Local Development Guide

**No Docker Required** - Run backend, frontend, and CLI compilation locally for fast iteration and cross-platform compilation.

## Quick Start (2 minutes)

```bash
# 1. One-time setup
bash scripts/setup_local_dev.sh

# 2. Start all services (backend + frontend)
bash scripts/start_dev.sh

# 3. In another terminal, compile
bash scripts/compile.sh examples/research_chain_windows_production.jky build/windows --platform windows
bash scripts/compile.sh examples/research_chain_linux_production.jky build/linux --platform linux
```

That's it! No Docker rebuilds needed.

---

## Detailed Setup

### Prerequisites

Your system needs:
- ✅ Python 3.8+
- ✅ Node.js 16+
- ✅ GCC (Linux toolchain)
- ✅ MinGW-w64 (Windows cross-compilation)
- ✅ OpenSSL dev libraries

All checked automatically by setup script.

### Step 1: One-Time Setup

```bash
bash scripts/setup_local_dev.sh
```

This will:
- ✅ Install Python dependencies (FastAPI, Uvicorn, etc.)
- ✅ Install Node.js dependencies (React, Vite, etc.)
- ✅ Verify build tools (GCC, MinGW, OpenSSL)
- ✅ Verify JOCKY toolchain is available
- ✅ Create `.env.local` configuration

**Output:**
```
✓ Setup Complete!

Next steps:
  1. Run: source /path/to/JOCKY/scripts/start_dev.sh
  2. Or run individually:
     - Backend:   cd /path/to/JOCKY/web/backend && source venv/bin/activate && python3 -m uvicorn app:app --reload
     - Frontend:  cd /path/to/JOCKY/web/frontend && npm run dev
     - Compile:   python3 /path/to/JOCKY/scripts/compile_pipeline.py [args]
```

### Step 2: Start Services

**All at once:**
```bash
bash scripts/start_dev.sh
```

**Or individually (in separate terminals):**

**Terminal 1 - Backend API:**
```bash
cd web/backend
source venv/bin/activate
export PYTHONPATH=/path/to/JOCKY/src
python3 -m uvicorn app:app --reload
```

**Terminal 2 - Frontend:**
```bash
cd web/frontend
npm run dev
```

### Step 3: Access Services

| Service | URL | Purpose |
|---------|-----|---------|
| **Frontend** | http://localhost:5173 | JOCKY Web UI |
| **Backend API** | http://localhost:8000 | Compilation REST API |
| **API Docs** | http://localhost:8000/docs | Interactive API docs |
| **Health Check** | http://localhost:8000/api/config | Backend status |

---

## CLI Compilation (No Frontend)

For batch compilation or CI/CD, use the CLI directly:

### Compile Windows PE Binary

```bash
bash scripts/compile.sh \
  examples/research_chain_windows_production.jky \
  build/windows \
  --platform windows \
  --preset standard
```

**Output:**
```
==================================================
JOCKY Local Compilation
==================================================
Source:     examples/research_chain_windows_production.jky
Output:     build/windows
Platform:   windows
Preset:     standard

Toolchain:
  Python:   Python 3.10.12
  Clang:    clang version 22.0.0
  MinGW:    x86_64-w64-mingw32-gcc (GCC) 10.3-win32

Starting compilation...

[PARSE] ...
[CODEGEN] ...
[MLIR] ...
[LLVM-OBF] ...
[COMPILE] ...
[RUNTIME] ...
[LINK] ...

✓ Compilation successful!

Output binary: build/windows/research_chain_windows_production.exe (2.5M)

File type: PE32+ executable (GUI) x86-64, for MS Windows

Next steps:
  - Transfer to Windows system
  - Run: build/windows/research_chain_windows_production.exe
  - Analyze with: Ghidra, IDA Pro, x64dbg
```

### Compile Linux ELF Binary

```bash
bash scripts/compile.sh \
  examples/research_chain_linux_production.jky \
  build/linux \
  --platform linux \
  --preset standard
```

### Obfuscation Presets

| Preset | MLIR Passes | LLVM Passes | Use Case |
|--------|------------|-----------|----------|
| **none** | 0 | 0 | Testing, development |
| **light** | 1 (string-encrypt) | 2 (strip, substitution) | Minimal overhead |
| **standard** | 3 (default) | 6 (default) | Recommended |
| **aggressive** | 6 (all) | 10 (all) | Maximum obfuscation |

```bash
# Aggressive obfuscation (all 16 passes)
bash scripts/compile.sh source.jky output/ --platform linux --preset aggressive

# Minimal obfuscation (faster compile)
bash scripts/compile.sh source.jky output/ --platform windows --preset light
```

---

## Development Workflow

### Local Testing Loop

```bash
# Terminal 1: Start backend + frontend
bash scripts/start_dev.sh

# Terminal 2: Watch compilation
watch -n 1 'ls -lh build/'

# Terminal 3: Make changes and compile
bash scripts/compile.sh examples/research_chain_windows_production.jky build/windows --platform windows
# Observe output, check for errors
# Edit source code
# Compile again - takes ~10-15 seconds, no Docker rebuild!
```

### Testing Frontend + Backend Together

1. Open http://localhost:5173
2. Select platform (Windows/Linux)
3. Toggle obfuscation passes
4. Paste or edit JOCKY code
5. Click "Compile"
6. Watch build logs in real-time
7. Download binary

### Debugging Compilation Issues

```bash
# See full pipeline output
bash scripts/compile.sh source.jky output/ --platform windows 2>&1 | tee compile.log

# Check intermediate files
ls -la output/
  output.ll        # LLVM IR
  output.mlir      # MLIR
  output_obf.mlir  # After MLIR obfuscation
  output.o         # Object file
  output.exe       # Final PE binary (Windows)
```

---

## Environment Variables

Created in `.env.local` after setup:

```bash
# Source manually if needed
source .env.local

# Or set manually
export PYTHONPATH=/path/to/JOCKY/src
export JOCKY_ROOT=/path/to/JOCKY
export TOOLCHAIN_PATH=/path/to/JOCKY/toolchain
export API_BASE=http://localhost:8000
```

---

## Troubleshooting

### Backend Won't Start

```bash
# Check Python environment
python3 --version  # Should be 3.8+

# Verify dependencies
cd web/backend && source venv/bin/activate
pip list | grep -E "fastapi|uvicorn"

# Check port availability
lsof -i :8000

# Start with debug output
python3 -m uvicorn app:app --reload --log-level debug
```

### Frontend Won't Start

```bash
# Check Node version
node --version  # Should be 16+
npm --version

# Verify dependencies
cd web/frontend && npm list react react-dom

# Clear cache and reinstall
rm -rf node_modules package-lock.json
npm install
npm run dev
```

### Compilation Fails

```bash
# Verify toolchain
file toolchain/bin/clang
toolchain/bin/clang --version

# Check PYTHONPATH
python3 -c "import sys; print('\n'.join(sys.path))"
# Should include /path/to/JOCKY/src

# Verify build tools
gcc --version
x86_64-w64-mingw32-gcc --version
pkg-config --cflags openssl

# Run with verbose output
bash -x scripts/compile.sh source.jky output/ --platform windows
```

### API Connection Issues

```bash
# Check backend is running
curl http://localhost:8000/api/config

# Check frontend API base
cat web/frontend/src/utils/env.ts

# Check network
netstat -tlnp | grep 8000  # Backend
netstat -tlnp | grep 5173  # Frontend
```

---

## Performance Tips

✅ **Incremental builds** - Use `--preset light` for fast iteration  
✅ **Parallel compilation** - Run multiple compile.sh in parallel  
✅ **Frontend HMR** - Vite hot-reload makes UI changes instant  
✅ **API hot-reload** - Uvicorn auto-reloads on Python changes  

---

## Next Steps

- ✅ **Local compilation working?** → Edit `examples/research_chain_*.jky` and recompile
- ✅ **Frontend working?** → Try different platforms, obfuscation presets
- ✅ **Both working?** → Analyze output binaries with Ghidra
- ✅ **Ready to commit?** → Push to feature branch and create PR

---

## File Structure

```
JOCKY/
├── scripts/
│   ├── setup_local_dev.sh      ← One-time setup
│   ├── start_dev.sh            ← Start all services
│   ├── compile.sh              ← CLI compilation
│   └── compile_pipeline.py     ← Core pipeline (called by compile.sh)
├── web/
│   ├── backend/
│   │   ├── app.py              ← FastAPI server
│   │   ├── compiler.py         ← Compilation handler
│   │   └── venv/               ← Python virtual env
│   └── frontend/
│       ├── src/                ← React source
│       ├── package.json
│       └── node_modules/       ← npm packages
├── src/
│   └── jocky/                  ← JOCKY compiler source
├── toolchain/
│   ├── bin/                    ← clang, mlir-opt, etc.
│   └── lib/                    ← MLIR/LLVM plugins
├── examples/
│   ├── research_chain_windows_production.jky
│   └── research_chain_linux_production.jky
└── .env.local                  ← Created by setup script
```

---

## Tips & Tricks

### Batch Compilation

```bash
#!/bin/bash
for jky in examples/*.jky; do
  echo "Compiling $jky..."
  bash scripts/compile.sh "$jky" "build/batch" --platform windows --preset aggressive
  bash scripts/compile.sh "$jky" "build/batch" --platform linux --preset aggressive
done
```

### Watch Mode (Recompile on Changes)

```bash
# Install entr: apt-get install -y entr
find examples -name "*.jky" | entr bash scripts/compile.sh examples/research_chain_windows_production.jky build/watch --platform windows
```

### Direct Pipeline Invocation

```bash
export PYTHONPATH=/path/to/JOCKY/src
python3 scripts/compile_pipeline.py \
  examples/research_chain_linux_production.jky \
  build/direct \
  --platform linux \
  --preset aggressive \
  --mlir-passes "--string-encrypt,--symbol-obfuscate" \
  --llvm-passes "strip-signature,flattening,anti-debug"
```

---

**Questions?** Check the main README.md or run `--help` on any script!
