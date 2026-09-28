# JOCKY Architecture & Design

This document describes the high-level architecture of the JOCKY compiler system, its components, interaction patterns, and compilation pipeline.

---

## Table of Contents

1. [System Overview](#system-overview)
2. [Component Architecture](#component-architecture)
3. [Compilation Pipeline](#compilation-pipeline)
4. [Data Flow](#data-flow)
5. [Obfuscation Framework](#obfuscation-framework)
6. [Runtime Architecture](#runtime-architecture)

---

## System Overview

JOCKY is a **modular, multi-stage compiler** that transforms JOCKY source code into obfuscated native executables. The system is organized into three primary layers:

### Layer 1: Language Frontend
- **Lexer** – Tokenizes source code
- **Parser** – Builds abstract syntax tree (AST)
- **Type Checker** – Validates types and semantic constraints
- **Module System** – Manages cross-file dependencies

### Layer 2: Intermediate Representation (IR) & Optimization
- **LLVM IR Codegen** – Emits LLVM intermediate representation
- **MLIR Layer** – Higher-level IR for cross-cutting concerns
- **Canonicalization** – Normalizes IR for obfuscation
- **MLIR Obfuscation Passes** – Semantic-preserving transformations

### Layer 3: Lowering & Backend
- **LLVM Obfuscation** – opt-based IR-level passes
- **Binary Codegen** – Assembly generation via LLVM backend
- **Linking** – Object file linking to executable
- **Packing** – Optional encryption and hardening

### Runtime Layer
- **Anti-Analysis** – Detection of debuggers, VMs, sandboxes
- **Evasion Techniques** – Syscall spoofing, module hiding
- **Driver Operations** – BYOVD, kernel memory access
- **Cleanup** – Log clearing and artifact removal

---

## Component Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        JOCKY CLI (Python)                        │
│                    src/jocky/cli.py & api.py                     │
└──────────────────────────┬──────────────────────────────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
   ┌────▼────┐      ┌─────▼──────┐    ┌─────▼────┐
   │  Lexer   │      │   Parser   │    │ Checker  │
   │  (lexer) │─────▶│  (parser)  │───▶│(checker) │
   └──────────┘      └────────────┘    └─────────▶┘
                                              │
                              ┌───────────────┼────────────────┐
                              │               │                │
                        ┌─────▼─────┐   ┌────▼──────┐  ┌──────▼──────┐
                        │ Generics  │   │ Closures  │  │ Attributes  │
                        │ Inference │   │ Analysis  │  │ Resolution  │
                        └─────┬─────┘   └────┬──────┘  └──────┬──────┘
                              │              │                │
                        ┌─────▼──────────────▼────────────────▼────────┐
                        │          LLVM IR Codegen (codegen.py)        │
                        │         Emits LLVM IR Module                  │
                        └─────────────────┬──────────────────────────┘
                                          │
                        ┌─────────────────▼──────────────┐
                        │  Canonicalization Pass         │
                        │  Normalize IR for Obfuscation  │
                        └─────────────────┬──────────────┘
                                          │
                        ┌─────────────────▼──────────────────────┐
                        │    MLIR Obfuscation Passes             │
                        │ - const-encrypt                         │
                        │ - control-flow                          │
                        │ - call-site-obfuscation                 │
                        │ - instruction-duplication               │
                        │ - polymorphic-obfuscation (NEW)         │
                        │ - bytecode-vm (NEW)                     │
                        └─────────────────┬──────────────────────┘
                                          │
                        ┌─────────────────▼──────────────────────┐
                        │    LLVM Obfuscation (opt passes)        │
                        │ - boguscf                               │
                        │ - flattening                            │
                        │ - substitution                          │
                        │ - linear-mba                            │
                        │ - opaque-pred                           │
                        │ - string-encrypt                        │
                        │ - anti-disasm                           │
                        │ - anti-tamper                           │
                        └─────────────────┬──────────────────────┘
                                          │
                        ┌─────────────────▼──────────────┐
                        │   LLVM Backend                 │
                        │   - Target Triple Selection    │
                        │   - Code Generation            │
                        │   - Object File Emission       │
                        └─────────────────┬──────────────┘
                                          │
                        ┌─────────────────▼──────────────┐
                        │   Linker (ld/lld)              │
                        │   Link Object + Runtime        │
                        └─────────────────┬──────────────┘
                                          │
                        ┌─────────────────▼──────────────┐
                        │   Packing (Optional)           │
                        │   - PE Stub Creation           │
                        │   - RC4 Encryption             │
                        │   - Integrity Embedding        │
                        └─────────────────┬──────────────┘
                                          │
                        ┌─────────────────▼──────────────┐
                        │   Final Executable             │
                        │   (PE/ELF with Runtime)        │
                        └────────────────────────────────┘
```

---

## Compilation Pipeline

The compilation process follows nine distinct stages, each with clear input/output contracts:

### Stage 1: Lexical Analysis (Lex)
**Input:** JOCKY source code (.jky file)  
**Output:** List of tokens with position information  
**Key Components:**
- Pattern-based tokenization
- Keyword/identifier differentiation
- String and numeric literal parsing
- Comment skipping

```python
# Example token stream
[Token(KEYWORD, 'fn'), Token(IDENT, 'add'), Token(LPAREN, '('),
 Token(IDENT, 'a'), Token(COLON, ':'), Token(KEYWORD, 'i32'), ...]
```

### Stage 2: Parsing (Parse)
**Input:** Token stream  
**Output:** Abstract Syntax Tree (AST)  
**Key Components:**
- Recursive descent parser
- Precedence-based expression parsing
- Error recovery and reporting
- Scope tracking for modules

```python
# Example AST structure
FuncDecl(name='add', params=[Param(name='a', type=i32), Param(name='b', type=i32)],
         return_type=i32, body=[...statements...])
```

### Stage 3: Type Checking (Check)
**Input:** AST  
**Output:** Annotated AST with type information  
**Key Components:**
- Type inference for let bindings
- Type unification for generics
- Monomorphization (specialized generic instances)
- Pattern matching validation
- Attribute resolution

```python
# Type environment after checking
{
  'add': FuncType(params=[i32, i32], return_type=i32),
  'max<T>': GenericFuncType(type_params=['T'], ...),
  'map<T, U>': GenericFuncType(type_params=['T', 'U'], ...),
}
```

### Stage 4: Code Generation (Codegen)
**Input:** Annotated AST  
**Output:** LLVM IR module  
**Key Components:**
- LLVM IR builder
- Function lowering to LLVM declarations
- Closure capture handling
- String literal embedding and encryption
- Runtime function registration

```llvm
; Generated LLVM IR example
define i32 @add(i32 %a, i32 %b) {
  %1 = add i32 %a, %b
  ret i32 %1
}
```

### Stage 5: Canonicalization
**Input:** LLVM IR module  
**Output:** Normalized LLVM IR  
**Key Components:**
- Control flow simplification
- Branch canonicalization
- Function inlining markers
- Obfuscation hint insertion

### Stage 6: MLIR Obfuscation
**Input:** Canonicalized LLVM IR  
**Output:** Obfuscated MLIR + LLVM IR  
**Key Components:**
- Constant encryption transformations
- Control flow mutation
- Call site obfuscation
- Instruction duplication
- Polymorphic code generation
- Bytecode VM virtualization

### Stage 7: LLVM Obfuscation
**Input:** Obfuscated IR  
**Output:** Final optimized IR  
**Key Components:**
- opt-based pass pipeline
- Bogus control flow injection
- Instruction substitution
- Mixed boolean arithmetic
- String encryption
- Anti-disassembly hardening

### Stage 8: Linking
**Input:** Object files (user code + runtime)  
**Output:** Executable  
**Key Components:**
- Symbol resolution
- Section layout
- Runtime initialization glue
- ASLR support

### Stage 9: Packing (Optional)
**Input:** Executable  
**Output:** Packed + hardened executable  
**Key Components:**
- PE header analysis
- Stub generation
- Section encryption
- CRC32 checksum embedding

---

## Data Flow

The compiler maintains several key data structures through the pipeline:

### Type System
```
        JType (base)
       /     |     \
    Scalar  Pointer  Struct
    (i32)   (T*)     {fields}
     |              /  |  \
     +-- Generic   /   |   \
        <T>       Enum Array Closure
                  {variants}
```

### Symbol Tables
```
Global Scope
├── Functions
│   ├── user_func: FuncType
│   └── runtime_funcs: [jocky_*, builtin_*]
├── Types
│   ├── i32, i64, bool, ...
│   └── user_structs, enums
├── Modules
│   └── mod_name: ModuleScope
└── Generics
    └── generic_func<T>: GenericInstance[]
```

### Module System
```
MainModule
├── declarations (fn, struct, enum)
├── imports (use statements)
└── sub_modules
    ├── crypto
    │   └── rc4_module
    └── evasion
        └── anti_debug_module
```

---

## Obfuscation Framework

JOCKY implements a **profile-based obfuscation system** where each profile defines a specific set and order of obfuscation passes.

### Profile Architecture

```
profiles.yaml
├── none
│   └── passes: []
├── light
│   └── passes: [const-encrypt, instruction-duplication]
├── standard
│   └── passes: [boguscf, flattening, substitution, linear-mba, opaque-pred]
├── aggressive
│   └── passes: [LLVM passes + MLIR passes + polymorphic-obf + bytecode-vm]
└── paranoid
    └── passes: [aggressive + anti-disasm + anti-tamper + anti-analysis]
```

### Pass Pipeline Execution

```
Input IR
├─ Pass 1: const-encrypt
│  └─ Output: encrypted constants
├─ Pass 2: control-flow-mutation
│  └─ Output: flattened branches
├─ Pass 3: instruction-duplication
│  └─ Output: redundant instructions
└─ Pass N: polymorphic-obfuscation
   └─ Output: multiple code variants with runtime selection
```

### Obfuscation Categories

**Semantic-Preserving (Safe):**
- Control flow flattening
- Instruction substitution
- String encryption
- Constant obfuscation

**Heuristic-Based (Risky):**
- Anti-disassembly (may break genuine disassemblers)
- Opaque predicates (detectable by constraint solving)
- Function pointer obfuscation (may confuse static analysis)

**Runtime-Aware (Adaptive):**
- Polymorphic obfuscation (mutates at runtime)
- Bytecode VM (custom instruction set per build)
- Anti-analysis hooks (responds to environment)

---

## Runtime Architecture

The runtime layer is embedded in the final executable and provides two primary functions:

### 1. Initialization & Integrity
```
jocky_runtime_init()
├─ Integrity verification (CRC32 check)
├─ Analysis environment detection
└─ Anti-analysis subsystem initialization
```

### 2. Capability Subsystems
```
Capabilities
├── Anti-Analysis
│   ├── Debugger detection
│   ├── VM/hypervisor detection
│   ├── Sandbox detection
│   └── Anti-disassembly checks
├── Evasion
│   ├── Syscall spoofing
│   ├── API hooking evasion
│   └── Call stack manipulation
├── Kernel Operations
│   ├── Driver loading (BYOVD)
│   ├── Physical memory access
│   └── Kernel exploitation
└── Cleanup
    ├── Log clearing
    ├── Artifact removal
    └── Anti-forensics
```

### Runtime Initialization Chain

```
_start
└─ jocky_runtime_init()
   ├─ 1. Load embedded driver (.jdrv section)
   ├─ 2. Verify binary integrity
   ├─ 3. Check debugger/VM/sandbox
   ├─ 4. Initialize anti-disasm defenses
   ├─ 5. Set up exception handlers
   └─ 6. Jump to user main()
```

### Memory Layout
```
+─────────────────────────────────────+
│  .text (encrypted, anti-tamper)     │
│  .data (encrypted constants)        │
│  .rdata (embedded strings)          │
│  .reloc (relocation info)           │
+─────────────────────────────────────+
│  .jdrv (embedded driver)            │
│  .jmani (manifest)                  │
│  .jtamp (integrity checksum)        │
+─────────────────────────────────────+
│  Runtime heap (anti-forensics)      │
│  Stack (canary-protected)           │
+─────────────────────────────────────+
```

---

## Thread Safety & Concurrency

JOCKY's runtime is designed for single-threaded execution by default. For concurrent scenarios:

1. **Thread-local storage** – Anti-analysis state isolated per thread
2. **Atomic operations** – CAS loops for lock-free data structures
3. **Memory barriers** – Volatile accesses for shared state
4. **Future:** Thread pool API (planned)

---

## Error Handling

Errors flow through the system with rich context:

```
ErrorFormatter
├── Error Code (E0001-E0010)
├── SourceLocation (file:line:column)
├── SourceRange (visual context)
├── Message (human-readable)
└── Suggestion (fix hint)
```

Example error output:
```
Error E0003: Type mismatch in function call
  at examples/test.jky:15:10

    14 | let result = add(5, "hello");
    15 |             ^^^^^^^^^^^^^^^^
       |             expected i32, got string

Suggestion: convert argument to i32 first
```

---

## Caching & Incremental Compilation

The build system supports caching to accelerate development:

```
CacheManager
├── Lexer cache (tokenization)
├── Parser cache (AST)
├── Checker cache (type info)
├── Codegen cache (LLVM IR)
└── Linking cache (object files)

Cache invalidation on:
├── Source file modification
├── Profile change
├── Compiler version change
└── Dependency updates
```

---

## Future Directions

### Platform Expansion
- **macOS (Mach-O)** – Full runtime support planned
- **Android NDK** – Cross-compilation for mobile
- **WASM** – Browser execution target

### Optimization
- **Parallel compilation** – Multi-threaded stage execution
- **LTO integration** – Link-time optimization for smaller binaries
- **SIMD obfuscation** – Vectorized transformation passes

### Language Features
- **Trait system** – Compile-time polymorphism
- **Macros** – Compile-time meta-programming
- **Associated types** – Generic constraints

---

**Document Status:** Complete as of 2026-09-29  
**Architecture Version:** 1.0
