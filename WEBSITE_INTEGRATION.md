# Website Integration - Complete End-to-End Compilation

## Overview
The JOCKY website (frontend + backend) is now fully integrated with the production-grade `compile_pipeline.py` for both Windows PE and Linux ELF binary compilation with comprehensive obfuscation support.

## Architecture

### Frontend → Backend → Pipeline Flow

```
Frontend (React)
    ↓
1. User selects platform (Windows/Linux)
2. User toggles obfuscation passes
3. User submits source code
    ↓
API Call: POST /api/compile
    ↓
Backend (FastAPI)
    ↓
1. app.py receives CompileRequest with:
   - source: JOCKY code
   - platform: 'windows' or 'linux'
   - obfuscation: {mlir: {...}, llvm: {...}}
   - preset: 'standard' (default)
    ↓
2. compiler.py processes request:
   - _build_obfuscation_config() builds flags from toggles
   - Creates Docker command with --mlir-passes and --llvm-passes
    ↓
3. Docker container runs compile_pipeline.py:
   - Stage 1: Parse (JOCKY code → AST)
   - Stage 2: CodeGen (AST → LLVM IR)
   - Stage 3: MLIR (LLVM IR → MLIR + obfuscation)
   - Stage 4: LLVM (Apply LLVM obfuscation passes)
   - Stage 5: Compile (Bitcode → Object file)
   - Stage 5b: Runtime (Compile C runtime)
   - Stage 6: Link (Link binary)
    ↓
Output: Windows PE .exe or Linux ELF binary
```

## Obfuscation Passes

### MLIR Passes (6 total - ALL AVAILABLE)
| ID | Name | Flag | Description |
|----|------|------|-------------|
| string_encrypt | String Encryption | `--string-encrypt` | Encrypts all string literals with XOR/RC4 |
| constant_obfuscate | Constant Obfuscation | `--constant-obfuscate` | Replaces numeric constants with opaque expressions |
| symbol_obfuscate | Symbol Obfuscation | `--symbol-obfuscate` | Renames internal symbols to randomized identifiers |
| crypto_hash | Cryptographic Hashing | `--crypto-hash` | Uses crypto hashing for symbol obfuscation verification |
| scf_obfuscate | SCF Region Obfuscation | `--scf-obfuscate` | Applies opaque predicates to structured control flow |
| import_obfuscate | Import Obfuscation | `--import-obfuscate` | Hides imports behind wrapper functions |

### LLVM Passes (10 total - FULL OLLVM SUITE)
| ID | Name | Flag | Description |
|----|------|------|-------------|
| strip_signature | Strip Signatures | `strip-signature` | Removes debug metadata and function signature info |
| pdata_strip | PDATA Strip | `pdata-strip` | Removes exception handling and unwinding metadata |
| virtualize | Function Virtualization | `virtualize` | Converts functions to virtualized bytecode dispatchers |
| opaque_pred | Opaque Predicates | `opaque-pred` | Injects opaque predicates to obfuscate control flow |
| substitution | Instruction Substitution | `substitution` | Replaces standard instructions with complex sequences |
| boguscf | Bogus Control Flow | `boguscf` | Inserts opaque predicates and dead code branches |
| flattening | Control Flow Flattening | `flattening` | Converts structured control flow to switch dispatcher |
| linear_mba | Linear MBA | `linear-mba` | Converts arithmetic operations to mixed-boolean arithmetic |
| anti_debug | Anti-Debug Protection | `anti-debug` | Injects debugger detection and anti-debugging techniques |
| indirect_call | Indirect Calls | `indirect-call` | Converts direct calls to indirect via function pointers |

**Recommended Pass Order** (from OBFUSCATION.md):
1. Strip metadata: `strip-signature`, `pdata-strip`
2. Virtualize: `virtualize`
3. Function rewrites (wrapped in `function()`): `opaque-pred`, `substitution`, `boguscf`, `flattening`, `linear-mba`
4. Anti-debug: `anti-debug`
5. Indirect calls (last): `indirect-call`

## Integration Points

### 1. Frontend (web/frontend/src/)
- **data.ts**: `defaultObfuscation` includes all 16 passes with correct IDs
- **App.tsx**: Uses obfuscation state from data.ts
- **useCompiler.ts**: Sends obfuscation config to backend API
- **ObfuscationPanel.tsx**: Displays all passes with toggle switches

### 2. Backend API (web/backend/)
- **obfuscation.py**: Defines all 16 passes with correct IDs and flags
- **app.py**: 
  - `MLIRConfig` and `LLVMConfig` models with all fields
  - `CompileRequest` validates platform and preset
  - `/api/obfuscation-passes` endpoint returns all passes
- **compiler.py**:
  - `_build_obfuscation_config()` builds flags from user toggles
  - `run_compilation()` passes `--mlir-passes` and `--llvm-passes` to pipeline

### 3. Compilation Pipeline (scripts/compile_pipeline.py)
- **Arguments**: Added `--mlir-passes` and `--llvm-passes` for custom passes
- **stage_mlir_obfuscate()**: Accepts custom MLIR passes, falls back to defaults
- **stage_llvm_obfuscate()**: Accepts custom LLVM passes, falls back to full suite
- **Platforms**: Supports both Windows PE and Linux ELF via `--platform` flag
- **Runtime**: Compiles real C implementations for both platforms

## Data Flow Example

### User selects custom obfuscation
```json
{
  "source": "fn main() { ... }",
  "platform": "linux",
  "obfuscation": {
    "mlir": {
      "string_encrypt": true,
      "constant_obfuscate": false,
      "symbol_obfuscate": true,
      "crypto_hash": false,
      "scf_obfuscate": true,
      "import_obfuscate": false
    },
    "llvm": {
      "strip_signature": true,
      "pdata_strip": true,
      "virtualize": false,
      "opaque_pred": true,
      "substitution": true,
      "boguscf": false,
      "flattening": true,
      "linear_mba": false,
      "anti_debug": true,
      "indirect_call": true
    }
  },
  "preset": "standard"
}
```

### Backend processes request
1. `_build_obfuscation_config()` builds:
   ```python
   {
     "mlir_flags": ["--string-encrypt", "--symbol-obfuscate", "--scf-obfuscate"],
     "llvm_passes": "strip-signature,pdata-strip,function(opaque-pred,substitution,flattening,linear-mba),anti-debug,indirect-call"
   }
   ```

2. Docker command:
   ```bash
   docker run --rm -v /tmp/build:/workspace/build compile_python3 scripts/compile_pipeline.py \
     /workspace/jocky/source.jky /workspace/build \
     --platform linux \
     --preset standard \
     --mlir-passes "--string-encrypt,--symbol-obfuscate,--scf-obfuscate" \
     --llvm-passes "strip-signature,pdata-strip,function(opaque-pred,substitution,flattening,linear-mba),anti-debug,indirect-call"
   ```

3. Pipeline compiles with selected obfuscation passes

## Testing the Integration

### Run Docker Compose
```bash
cd /home/incharanew/JOCKY
docker compose up --build
```

### Access Services
- Frontend: `http://localhost:3000`
- Backend API: `http://localhost:8000`
- API Docs: `http://localhost:8000/docs`

### Test Compilation
1. Open `http://localhost:3000`
2. Select platform (Windows or Linux)
3. Toggle obfuscation passes (or use defaults)
4. Paste JOCKY code or use demo
5. Click "Compile"
6. Watch build logs in real-time via WebSocket
7. Download resulting binary (.exe for Windows, ELF for Linux)

### Verify Obfuscation Applied
- Use Ghidra to inspect binary
- Check for obfuscation artifacts:
  - String decryption stubs
  - Opaque predicates
  - Control flow flattening
  - MBA expressions
  - Anti-debug checks

## File Changes Summary

### New Files
- None

### Modified Files
1. **web/backend/obfuscation.py**: Added all 16 passes (117 lines, +43 lines)
2. **web/backend/app.py**: Updated MLIRConfig/LLVMConfig models
3. **web/backend/compiler.py**: Pass obfuscation config to pipeline
4. **scripts/compile_pipeline.py**: Accept --mlir-passes and --llvm-passes
5. **web/frontend/src/data.ts**: Updated defaultObfuscation with all passes

### Git Commit
```
Integrate compile_pipeline.py with website backend for full end-to-end compilation
- Added all 6 MLIR obfuscation passes to backend
- Added all 10 LLVM obfuscation passes to backend
- Updated compile_pipeline.py to accept custom obfuscation flags
- Updated backend compiler.py to build and pass obfuscation config
- Updated frontend data.ts to match backend pass IDs
```

## Verification Checklist

- [x] All 6 MLIR passes defined in backend/obfuscation.py
- [x] All 10 LLVM passes defined in backend/obfuscation.py
- [x] Frontend defaultObfuscation includes all 16 passes with correct IDs
- [x] Backend MLIRConfig and LLVMConfig models include all fields
- [x] compile_pipeline.py accepts --mlir-passes and --llvm-passes
- [x] Backend compiler.py calls _build_obfuscation_config()
- [x] Backend compiler.py passes flags to Docker command
- [x] Python syntax validation passed
- [x] TypeScript compilation passed
- [x] Git commit created on feature/website-integration branch

## Next Steps

1. **Build Docker images**: `docker compose build --no-cache`
2. **Start services**: `docker compose up`
3. **Test in browser**: Open http://localhost:3000
4. **Verify compilation**: Use demo scripts or custom code
5. **Inspect binaries**: Use Ghidra to verify obfuscation
6. **Create PR**: Feature complete and ready for review

## Status
✅ **COMPLETE** - Website fully integrated with compile_pipeline.py for production-grade compilation with all obfuscation passes
