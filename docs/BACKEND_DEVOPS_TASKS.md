# Backend & DevOps Tasks for Backend Compilation

**Priority:** CRITICAL  
**Assigned to:** @shreyashsri  
**Date:** 2026-09-30  
**Status:** Ready for implementation  
**Skills Required:** Backend, DevOps, Docker, Python/Services — NOT compiler knowledge

---

## Overview

These tasks enable the backend to support full compilation of Windows and Linux production scripts. They do NOT require compiler, LLVM, or codegen knowledge — only backend architecture, service implementation, and build infrastructure skills.

**Blocking Issue:** Backend cannot currently compile or serve forensic analysis results.

---

## Priority 1: CRITICAL (Blocks All Backend Operations)

### Task 1.1: Backend Forensic Library Loading (ISSUE-0010)
**Priority:** CRITICAL  
**Component:** Backend startup/initialization  
**Effort:** 6-8 hours  

**Problem:**
- Backend doesn't load forensic C libraries on startup
- When compiled scripts call forensic functions, runtime error: `forensic library not loaded`
- Blocks all forensic analysis execution

**What Needs to Happen:**
1. Identify backend startup sequence
2. Add forensic library loading (`dlopen` / Windows equivalent for `.so`/`.dll`)
3. Link forensic functions into backend runtime
4. Test with `./dev_launch.sh` to verify library loads
5. Add error handling for missing forensic libraries

**Success Criteria:**
- ✅ Backend starts without errors
- ✅ Forensic library loads on startup
- ✅ `./dev_launch.sh` can compile scripts with forensic calls
- ✅ No `undefined reference` errors in compiled binaries

**Files to Modify:**
- Backend initialization code (location: TBD, likely `src/backend/main.py` or similar)
- Backend configuration (add forensic library path)
- CMakeLists.txt (ensure forensic `.so`/`.dll` files built)

**Dependencies:**
- Requires Task 2.1 (CMakeLists.txt fixes) to be complete
- Requires Task 2.2 (prelude forensic bindings) to be complete

---

### Task 1.2: Backend Forensic Service Implementation
**Priority:** CRITICAL  
**Component:** Backend service architecture  
**Effort:** 8-10 hours  

**Problem:**
- No backend service for forensic analysis
- Compiled scripts can't call forensic functions through backend
- No endpoints for forensic result retrieval

**What Needs to Happen:**
1. Design forensic analysis service interface
2. Create backend service for forensic operations
3. Implement forensic analysis request handler
4. Create forensic result storage/retrieval system
5. Add request routing for forensic functions
6. Test with `./dev_launch.sh`

**Service Responsibilities:**
- Accept forensic analysis requests from compiled scripts
- Execute forensic functions with proper arguments
- Return results in structured format (JSON/protobuf)
- Handle errors and timeouts
- Log all operations

**Success Criteria:**
- ✅ Backend accepts forensic analysis requests
- ✅ Service executes without errors
- ✅ Results returned in proper format
- ✅ Errors handled gracefully
- ✅ Works with Windows and Linux APIs

**Suggested Endpoints:**
- `POST /forensic/analyze` - Run forensic analysis
- `GET /forensic/results/{id}` - Retrieve results
- `POST /forensic/collect/{type}` - Collect specific evidence (memory, registry, logs)

**Dependencies:**
- Task 1.1 (Library loading)
- Task 2.1 (CMakeLists.txt)
- Task 2.2 (Prelude bindings)

---

## Priority 2: HIGH (Unblocks Development)

### Task 2.1: Fix CMakeLists.txt Build Configuration
**Priority:** HIGH  
**Component:** Build system  
**Effort:** 4-6 hours  
**Skills:** Build systems, CMake, DevOps

**Problem:**
- Linux: 28 of 48 files NOT compiled (missing from CMakeLists.txt)
- CMakeLists.txt has wrong file paths (e.g., lists `linux/syscall.c` but file is `linux/syscalls/syscall.c`)
- Forensic source files (.c) exist but not included in build
- Results: Undefined symbols, stubs override real implementations, binary non-functional

**What Needs to Happen:**
1. Audit CMakeLists.txt against actual file structure
2. Fix incorrect file paths (e.g., `linux/syscall.c` → `linux/syscalls/syscall.c`)
3. Add missing Linux directories to build:
   - `src/runtime/linux/core/` (LKM implementations)
   - `src/runtime/linux/persistence/` (process persistence)
   - `src/runtime/linux/anti_analysis/` (security)
   - `src/runtime/linux/io/` (I/O operations)
   - `src/runtime/linux/exfil/` (data exfiltration)
   - `src/runtime/linux/threading/` (threading)
4. Add forensic source files to build:
   - All `.c` files in `src/runtime/forensics/`
5. Test build with `cmake . && make` to verify no missing files
6. Verify no duplicate symbols or conflicts

**Success Criteria:**
- ✅ All 48 Linux files compile without errors
- ✅ All forensic `.c` files included
- ✅ No undefined symbol linker errors
- ✅ Build completes successfully
- ✅ Both Windows and Linux binaries build

**Files to Modify:**
- `src/runtime/CMakeLists.txt` (main runtime configuration)
- Possibly `src/CMakeLists.txt` (project root)

**Reference Documents:**
- Use `docs/LINUX_LINKER_INVESTIGATION.md` for path corrections
- Use `docs/RUNTIME_API_INVENTORY.md` for forensic file list

---

### Task 2.2: Add Forensic Bindings to Prelude
**Priority:** HIGH  
**Component:** Standard library / Language bindings  
**Effort:** 3-4 hours  
**Skills:** Python scripting, API binding

**Problem:**
- 38+ forensic functions implemented in C but NOT exposed to JOCKY code
- Type system missing 11 complex forensic struct definitions
- Research chains can't call forensic APIs because they're not bound
- Blocks all forensic analysis in compiled scripts

**What Needs to Happen:**
1. Read `docs/PRELUDE_BINDINGS_AUDIT.md` (contains suggested signatures)
2. Add 26 Phase 1 forensic function bindings to prelude:
   - `forensic_engine_*` (7 functions)
   - `forensic_artifact_list_*` (6 functions)
   - `forensic_analysis_*` (6 functions)
   - `forensic_provenance_*` (7 functions)
3. Add type definitions for 11 forensic result structs
4. Add documentation/examples for each function
5. Test with `./dev_launch.sh` to verify bindings work

**Success Criteria:**
- ✅ All 26 Phase 1 functions callable from JOCKY code
- ✅ Type system knows about forensic result types
- ✅ No "undefined function" errors in compilation
- ✅ Prelude test scripts run without errors

**File to Modify:**
- `stdlib/jocky.runtime.jky` (add forensic function and type bindings)

**Reference Document:**
- `docs/PRELUDE_BINDINGS_AUDIT.md` contains exact function signatures to copy-paste

---

### Task 2.3: Backend Forensic Analysis Endpoints
**Priority:** HIGH  
**Component:** Backend API  
**Effort:** 6-8 hours  

**Problem:**
- Backend service (Task 1.2) needs actual endpoint implementations
- No handlers for forensic analysis requests
- No way to trigger or retrieve forensic results

**What Needs to Happen:**
1. Implement endpoint handlers for forensic requests
2. Create request validation/parsing
3. Implement result formatting (JSON/protobuf)
4. Add error handling and status codes
5. Add logging for all forensic operations
6. Test with `./dev_launch.sh`

**Endpoints to Implement:**
- `POST /forensic/analyze` — Submit forensic analysis job
- `GET /forensic/results/{id}` — Retrieve analysis results
- `POST /forensic/collect/{type}` — Collect specific evidence type
- `GET /forensic/status/{id}` — Check analysis status

**Success Criteria:**
- ✅ All endpoints respond to requests
- ✅ Proper HTTP status codes returned
- ✅ Results formatted correctly
- ✅ Errors handled gracefully
- ✅ Works with dev launcher testing

**Dependencies:**
- Task 1.2 (Backend service exists)
- Task 2.2 (Prelude bindings exist)

---

## Priority 3: MEDIUM (Improves Research Chains)

### Task 3.1: Fix Research Chain Phase Ordering
**Priority:** MEDIUM  
**Component:** Research chain orchestration  
**Effort:** 2-3 hours  

**Problem:**
- Research chains run anti-forensics (evidence destruction) BEFORE forensic analysis
- Results in destroying evidence before analysis can collect it
- Logical flow is backwards

**What Needs to Happen:**
1. Read both research chain files completely:
   - `examples/research_chain_windows_production.jky`
   - `examples/research_chain_linux_production.jky`
2. Identify phase ordering
3. Move anti-forensics phase to AFTER forensic analysis phase
4. Update phase numbers and documentation
5. Test with `./dev_launch.sh`

**Success Criteria:**
- ✅ Forensic analysis runs BEFORE anti-forensics
- ✅ Evidence collected before cleanup
- ✅ Both chains follow same logical order
- ✅ Compilation succeeds

**Files to Modify:**
- `examples/research_chain_windows_production.jky`
- `examples/research_chain_linux_production.jky`

**Reference Document:**
- `docs/RESEARCH_CHAIN_FORENSIC_GAPS.md` shows current phase structure

---

### Task 3.2: Dev Launcher Testing Validation
**Priority:** MEDIUM  
**Component:** Testing/Infrastructure  
**Effort:** 3-4 hours  

**Problem:**
- Dev launcher (`./dev_launch.sh`) needs to properly test backend compilation with forensic APIs
- Need to verify both Windows and Linux scripts compile end-to-end
- Need clear test results and error reporting

**What Needs to Happen:**
1. Understand dev launcher architecture
2. Add forensic API compilation tests
3. Add Windows script compilation test
4. Add Linux script compilation test
5. Add test output reporting
6. Document test procedure

**Test Coverage:**
- ✅ Main pipeline compiles Windows production script
- ✅ Main pipeline compiles Linux production script
- ✅ Backend accepts compiled binaries
- ✅ Forensic functions callable through backend
- ✅ No linker errors or undefined symbols

**Success Criteria:**
- ✅ Dev launcher reports pass/fail clearly
- ✅ All tests pass
- ✅ Error messages helpful for debugging
- ✅ Tests complete in < 5 minutes

**File to Modify:**
- `dev_launch.sh` or equivalent test runner

---

## Priority 4: LOW (Polish)

### Task 4.1: Docker Setup for Forensic Libraries (Optional)
**Priority:** LOW  
**Component:** Docker/Infrastructure  
**Effort:** 4-6 hours  

**Problem:**
- Backend Docker image may not include forensic libraries
- Tests may fail in Docker without proper library setup

**What Needs to Happen:**
1. Update Dockerfile to include forensic library dependencies
2. Add library linking to Docker build
3. Test Docker build completes without errors
4. Document library requirements

**Note:** User prefers dev launcher over Docker, but Docker setup may be needed for CI/deployment.

---

## Testing Strategy: Use `./dev_launch.sh` Only

Do NOT use Docker for debugging or testing backend compilation. Use dev launcher for all verification.

**Test Command:**
```bash
./dev_launch.sh
```

**Expected Output:**
- ✅ Backend starts
- ✅ Windows script compiles
- ✅ Linux script compiles
- ✅ Forensic functions accessible
- ✅ No linker errors

---

## Implementation Order

1. **Task 2.1** (CMakeLists.txt) — Unblocks everything else
2. **Task 2.2** (Prelude bindings) — Unblocks compilation
3. **Task 1.1** (Library loading) — Unblocks runtime
4. **Task 1.2** (Forensic service) — Implements feature
5. **Task 2.3** (Endpoints) — Implements API
6. **Task 3.1** (Phase ordering) — Improves logic
7. **Task 3.2** (Dev launcher) — Validates all
8. **Task 4.1** (Docker) — Optional polish

---

## Success Criteria: Full Backend Compilation

When all tasks complete:
- ✅ Windows 746KB binary compiles through full pipeline
- ✅ Linux 130KB binary compiles through full pipeline
- ✅ Backend accepts compiled binaries
- ✅ Forensic analysis functions callable and working
- ✅ `./dev_launch.sh` passes all tests
- ✅ No linker errors or undefined symbols
- ✅ Research chains execute forensic analysis phase before anti-forensics

---

## Notes for Shreyash

- **Do NOT edit compiler/codegen files** — That's handled separately
- **Focus on backend architecture and infrastructure** — Your domain
- **Use dev launcher for all testing** — Not Docker
- **Reference documents provided** — Check `docs/` folder
- **Ask questions if unclear** — Architecture is complex, but these tasks are straightforward
- **Coordinate with compiler team on Task 2.1** (CMakeLists.txt) if needed

---

## Related Compiler Tasks (NOT for this team)

These are handled by compiler/toolchain team:
- Prelude forensic bindings (Task 2.2) - might need help from language team
- Codegen return type support (ISSUE-0005)
- Type system extensions (ISSUE-0009)
- Windows registry codegen (ISSUE-0002)

---

## Questions?

See:
- `docs/CODEGEN_COMPILER_ISSUES.md` — Full issue tracker
- `docs/RUNTIME_API_INVENTORY.md` — What's implemented
- `docs/PRELUDE_BINDINGS_AUDIT.md` — Exact functions to bind
- `docs/LINUX_LINKER_INVESTIGATION.md` — Build configuration issues
- `CLAUDE.md` — Development guidelines
