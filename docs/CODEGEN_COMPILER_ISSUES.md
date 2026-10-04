# Codegen, Compiler & Obfuscation Issues Tracker

**Last Updated:** 2026-09-30  
**Purpose:** Central documentation of known issues in codegen, LLVM, MLIR, and obfuscation passes to prevent duplicate debugging and provide context for future fixes.

---

## Format for New Issues

When documenting a new issue, use this structure:

```markdown
### Issue: [Title]
- **ID:** ISSUE-NNNN
- **Component:** codegen | llvm | mlir | obfuscation | linker | type-system
- **Severity:** critical | high | medium | low
- **Status:** open | investigating | in-progress | blocked | resolved | workaround-only
- **Date Found:** YYYY-MM-DD
- **Affected File(s):** Path(s) to affected source files
- **Error Message:** Full error output
- **Reproduction Steps:**
  1. Step 1
  2. Step 2
  3. Step 3
- **Root Cause:** (if known)
- **Workaround:** (if available)
- **Priority:** Explain why this matters for backend compilation
- **Related Issues:** Links to similar issues
```

---

## Known Issues

### Issue: Forensic Pipeline Header Include Dependencies UNRESOLVED
- **ID:** ISSUE-0011
- **Component:** Build system, forensics
- **Severity:** critical
- **Status:** open
- **Date Found:** 2026-10-04
- **Affected File(s):**
  - `src/runtime/forensics/store/evidence_store.h` (line 22)
  - `src/runtime/forensics/control/capabilities.h`
  - Multiple forensics subdirectory headers
- **Error Message:** `fatal error: engine/provenance.h: No such file or directory`
- **Reproduction Steps:**
  1. cd src/runtime && cmake .
  2. make
  3. Compiler fails on forensic_engine.c during evidence_store.h inclusion
- **Root Cause:** Forensic pipeline integrated with incorrect header relative paths. Evidence store (in store/) tries to include engine/provenance.h with wrong path syntax. Multiple headers have circular or incorrect include paths.
- **Workaround:** None until paths fixed
- **Priority:** CRITICAL — Blocks full compilation of main pipeline
- **Related Issues:** None yet
- **Notes:** Forensic agents committed files without resolving include paths. Need comprehensive path restructuring in forensics subsystem.

---

### Issue: Undefined Symbol Linker Errors on Linux Runtime APIs
- **ID:** ISSUE-0001
- **Component:** linker
- **Severity:** critical
- **Status:** investigating
- **Date Found:** 2026-09-30
- **Affected File(s):**
  - `src/runtime/linux/linux_syscall.c`
  - `src/runtime/linux/linux_process.c`
  - `src/jocky/language/codegen.py`
- **Error Message:** `undefined reference to 'linux_syscall_wrapper'` and similar
- **Reproduction Steps:**
  1. Compile `research_chain_linux_production.jky` with codegen
  2. Link with `src/runtime/linux/` object files
  3. Observe undefined symbol errors for syscall wrappers
- **Root Cause:** Codegen generates calls to C functions that don't have matching implementations or are not being compiled
- **Workaround:** Verify function names match exactly between header declarations and `.c` implementations
- **Priority:** CRITICAL — Blocks Linux binary generation
- **Related Issues:** None yet

### Issue: Windows Registry API Not Exposed to Codegen
- **ID:** ISSUE-0002
- **Component:** codegen, type-system
- **Severity:** high
- **Status:** open
- **Date Found:** 2026-09-30
- **Affected File(s):**
  - `src/runtime/windows/win_registry.c`
  - `src/jocky/language/codegen.py`
  - `stdlib/jocky.runtime.jky`
- **Error Message:** `NameError: win_registry_query is not defined` (during compilation of research chain)
- **Reproduction Steps:**
  1. Try to use registry APIs in `research_chain_windows_production.jky`
  2. Codegen will fail to find the function binding
- **Root Cause:** Registry APIs exist in C but are not mapped to JOCKY builtin functions
- **Workaround:** Manually call C code through `ffi` (if available)
- **Priority:** HIGH — Required for Windows forensic analysis
- **Related Issues:** ISSUE-0006 (Prelude missing forensic APIs)

### Issue: Obfuscation Pass Ordering Breaks Type Information
- **ID:** ISSUE-0003
- **Component:** obfuscation, pipeline ordering
- **Severity:** medium (likely documentation issue)
- **Status:** resolved
- **Date Found:** 2026-09-30
- **Affected File(s):**
  - `scripts/compile_pipeline.py` (pipeline ordering)
  - `src/jocky/stages/ir_obfuscate.py`
  - `src/jocky/stages/mlir_obfuscate.py`
- **Error Message:** `TypeError: NoneType has no attribute 'type'` (only occurs if type checking called post-obfuscation)
- **Root Cause:** Type checking occurs in Parse stage (before codegen), while obfuscation happens at LLVM/MLIR level (post-codegen). Type metadata is not involved in LLVM-level transforms. If error occurs, it's because type checking is being called twice or in wrong order.
- **Solution:** Confirmed pipeline order is correct:
  1. Parse stage (includes type checking) ✅
  2. CodeGen stage (generates LLVM IR) ✅
  3. MLIR obfuscation (post-codegen, no AST type info needed) ✅
  4. LLVM obfuscation (post-codegen, no AST type info needed) ✅
  5. Compile/Link stages ✅
- **Fix Applied:** No code change needed. Type checking is already isolated from obfuscation passes. Pipeline ordering is correct.
- **Priority:** RESOLVED — Not a blocker. Type information preserved by correct pipeline ordering.
- **Related Issues:** None

### Issue: MLIR Translation Fails for Complex Control Flow
- **ID:** ISSUE-0004
- **Component:** mlir
- **Severity:** medium
- **Status:** investigating
- **Date Found:** 2026-09-30
- **Affected File(s):** Codegen MLIR translation phase (exact file TBD)
- **Error Message:** `ValueError: Unsupported control flow pattern in MLIR conversion`
- **Reproduction Steps:**
  1. Compile research chain with nested loops and conditionals
  2. MLIR translation encounters complex control flow
  3. Translation fails or produces incorrect code
- **Root Cause:** MLIR translator doesn't handle certain loop + branch combinations
- **Workaround:** Refactor control flow to simpler patterns (avoid nested loops where possible)
- **Priority:** MEDIUM — Affects some analysis scripts
- **Related Issues:** None yet

### Issue: Forensic API Codegen Not Generating Return Type Conversions
- **ID:** ISSUE-0005
- **Component:** codegen
- **Severity:** high
- **Status:** open
- **Date Found:** 2026-09-30
- **Affected File(s):**
  - `src/jocky/language/codegen.py`
  - `src/runtime/forensics/forensic_core.c`
- **Error Message:** `TypeError: Cannot convert C struct to JOCKY record type`
- **Reproduction Steps:**
  1. Call forensic analysis function that returns struct
  2. Codegen generates C call but not return type conversion
  3. Script fails at runtime or compile time
- **Root Cause:** Codegen doesn't have templates for forensic return types (complex structs)
- **Workaround:** Wrap forensic calls in helper functions that return simple types
- **Priority:** HIGH — Blocks forensic pipeline integration
- **Related Issues:** ISSUE-0006 (Prelude)

### Issue: Prelude Missing Forensic API Declarations
- **ID:** ISSUE-0006
- **Component:** type-system, prelude
- **Severity:** high
- **Status:** open
- **Date Found:** 2026-09-30
- **Affected File(s):**
  - `stdlib/jocky.runtime.jky`
  - `src/runtime/forensics/*.c`
- **Error Message:** `NameError: forensic_* functions not found in prelude`
- **Reproduction Steps:**
  1. Try to call forensic function in JOCKY script
  2. Type checker can't find function binding
  3. Compilation fails
- **Root Cause:** Forensic APIs are implemented in C but not declared in prelude
- **Workaround:** Declare forensic functions manually in each script (not scalable)
- **Priority:** CRITICAL — Blocks all forensic analysis
- **Related Issues:** ISSUE-0005 (Codegen return types)

### Issue: Duplicate Symbol in Windows Runtime When Linking AI Module
- **ID:** ISSUE-0007
- **Component:** linker
- **Severity:** medium
- **Status:** investigating
- **Date Found:** 2026-09-30
- **Affected File(s):**
  - `src/runtime/ai/ai_core.c`
  - `src/runtime/windows/win_ml_wrapper.c`
- **Error Message:** `duplicate symbol 'ai_initialize' in ai_core.o and win_ml_wrapper.o`
- **Reproduction Steps:**
  1. Link Windows runtime with both AI and Windows ML modules
  2. Linker encounters duplicate symbol
- **Root Cause:** Function implemented in both `.c` files (copy-paste)
- **Workaround:** Remove one implementation (see which is more complete)
- **Priority:** MEDIUM — Affects Windows AI integration
- **Related Issues:** ISSUE-0001 (Symbol issues)

### Issue: eBPF Program Compilation Fails on Certain Kernel Versions
- **ID:** ISSUE-0008
- **Component:** linker, platform-specific
- **Severity:** medium
- **Status:** open
- **Date Found:** 2026-09-30
- **Affected File(s):**
  - `src/runtime/linux/linux_ebpf.c`
  - eBPF object files (.o)
- **Error Message:** `LLVM: unsupported BPF instruction 0xXX` (kernel < 5.8)
- **Reproduction Steps:**
  1. Compile eBPF programs with modern LLVM
  2. Run on Linux kernel < 5.8
  3. Kernel rejects certain instructions
- **Root Cause:** LLVM generates newer BPF instructions; older kernels don't support
- **Workaround:** Build eBPF with `-O0` or use LLVM flags for older kernels
- **Priority:** LOW — Mostly affects legacy systems; research chains may not need eBPF
- **Related Issues:** None yet

### Issue: Type System Doesn't Support Forensic Analysis Result Types
- **ID:** ISSUE-0009
- **Component:** type-system
- **Severity:** high
- **Status:** investigating
- **Date Found:** 2026-09-30
- **Affected File(s):**
  - `src/jocky/language/type_checker.py` (or equivalent)
  - `src/runtime/forensics/forensic_core.c`
- **Error Message:** `TypeError: struct forensic_analysis_result has no JOCKY type binding`
- **Reproduction Steps:**
  1. Try to return forensic analysis result from function
  2. Type checker can't map C struct to JOCKY type
  3. Compilation fails
- **Root Cause:** Type system doesn't have bindings for forensic analysis result types
- **Workaround:** Convert to simpler types or JSON strings before returning
- **Priority:** HIGH — Affects forensic API usability
- **Related Issues:** ISSUE-0005, ISSUE-0006

### Issue: Backend Doesn't Load Forensic Libraries on Startup
- **ID:** ISSUE-0010
- **Component:** backend, linker
- **Severity:** critical
- **Status:** open
- **Date Found:** 2026-09-30
- **Affected File(s):** Backend initialization (location TBD)
- **Error Message:** `RuntimeError: forensic library not loaded` (when forensic functions called)
- **Reproduction Steps:**
  1. Start backend service
  2. Compile script with forensic APIs
  3. Try to execute forensic function
  4. Runtime error: library not loaded
- **Root Cause:** Backend startup doesn't link or load forensic `.so` / `.dll` files
- **Workaround:** Manual dlopen of forensic library (not production-ready)
- **Priority:** CRITICAL — Blocks backend forensic execution
- **Related Issues:** ISSUE-0001, ISSUE-0002

---

### Issue: LLVM IR Generation Invalid Pointer Array Initialization
- **ID:** ISSUE-0012
- **Component:** codegen, LLVM
- **Severity:** critical
- **Status:** open
- **Date Found:** 2026-10-04
- **Affected File(s):**
  - `src/jocky/language/codegen.py` (array initialization codegen)
  - Generated LLVM IR files during compilation
- **Error Message:** `error: integer constant must have integer type` when importing LLVM IR to MLIR
- **Reproduction Steps:**
  1. Compile any JOCKY file with global array of pointers
  2. Check generated `examples/.jocky-build/lower_ir/output.ll`
  3. Look for `@DATA_PATHS = global [6 x i8*] [i8* 0, i8* 0, ...]`
  4. Run `mlir-translate --import-llvm output.ll -o input.mlir`
  5. MLIR translation fails on invalid pointer initialization
- **Root Cause:** Codegen outputs `i8* 0` for pointer array initialization instead of `i8* null`. LLVM requires `null` keyword for pointer nullification, not `0`.
- **Workaround:** None - requires codegen fix
- **Priority:** CRITICAL — Blocks all research chain compilation (Windows/Linux)
- **Related Issues:** None
- **Example Bad Output:** `@DATA_PATHS = global [6 x i8*] [i8* 0, i8* 0, i8* 0, i8* 0, i8* 0, i8* 0]`
- **Expected Output:** `@DATA_PATHS = global [6 x i8*] [i8* null, i8* null, i8* null, i8* null, i8* null, i8* null]`
- **Fix Location:** In codegen array initialization, replace `0` with `null` for pointer array elements
- **Testing After Fix:** Must recompile research_chain_windows_production.jky and research_chain_linux_production.jky, verify MLIR translation succeeds

---

## Issue Statistics

- **Total Known Issues:** 12
- **Critical:** 4 (ISSUE-0001, ISSUE-0006, ISSUE-0010, ISSUE-0012)
- **High:** 5 (ISSUE-0002, ISSUE-0003, ISSUE-0005, ISSUE-0009, ISSUE-0004)
- **Medium:** 2 (ISSUE-0007, ISSUE-0008)
- **Resolved:** 1 (ISSUE-0003)

---

## Components Breakdown

| Component | Issues | Status |
|-----------|--------|--------|
| Codegen | 3 | 2 Open, 1 Investigating |
| Linker | 3 | 1 Critical, 2 Investigating |
| MLIR | 1 | Investigating |
| Obfuscation | 1 | Investigating |
| Type-System | 2 | 2 Open |
| Backend | 1 | Critical |
| Platform-Specific | 1 | Open |

---

## Dependency Graph

```
ISSUE-0006 (Prelude forensic APIs)
  ↓ blocks
ISSUE-0005 (Codegen return types)
  ↓ blocks
ISSUE-0009 (Type system support)
  ↓ blocks
ISSUE-0010 (Backend library loading)
```

```
ISSUE-0002 (Registry API exposure)
  ↓ blocks
Research chain Windows compilation
```

---

## Next Steps

1. **Immediate:** Triage CRITICAL issues (0001, 0006, 0010)
2. **Short-term:** Fix linker and type-system issues
3. **Medium-term:** Improve codegen for forensic results
4. **Long-term:** Optimize MLIR and obfuscation passes

---

## Adding New Issues

When you encounter a compilation or codegen error:

1. **Search this file first** — Is it already documented?
2. **If not found:**
   - Create new issue with format above
   - Assign next ID number
   - Add reproduction steps
   - Link related issues
3. **Update statistics** at top of file
4. **Update component table** if new component
5. **Note in CLAUDE.md** if it blocks backend compilation
