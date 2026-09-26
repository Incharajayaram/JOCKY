# Implementation Plan: Generics + Medium Priority Tasks

**Status:** Starting implementation (84% complete → target 95%+)
**Priority Order:** Generics → Tier 2 APIs → Anti-Analysis → Obfuscation

---

## PHASE 1: GENERICS/TEMPLATES (CRITICAL - Last Language Feature!)

### 1.1 Syntax & Parsing
```jocky
// Generic function
fn max<T>(a: T, b: T) -> T {
    if a > b { a } else { b }
}

// Generic struct
struct Pair<T> {
    first: T,
    second: T
}

// Generic enum
enum Result<T, E> {
    Ok(T),
    Err(E)
}

// Constrained generics (future)
fn sort<T: Comparable>(arr: T[]) { ... }
```

**Implementation Steps:**
1. Add `<T>` parsing to lexer (COLONCOLON vs angle brackets decision)
2. Parse generic parameters in function/struct/enum declarations
3. Parse generic type arguments in expressions: `max::<i32>(1, 2)`
4. Store TypeVar in AST for generic parameters

### 1.2 Type System Changes
- **TypeVar node** – represents `T`, `U`, etc.
- **Monomorphization** – generate concrete versions per usage
  - `max::<i32>` → `max_i32`
  - `max::<i64>` → `max_i64`
- **Type bounds** (future phase) – trait constraints

### 1.3 Type Checking
- Track generic parameter scope during checking
- Validate type constraints in generic calls
- Perform monomorphization during codegen

### 1.4 Code Generation
- Create specialized versions for each concrete type
- Link time optimization for code dedup
- Debug info for generic instances

### 1.5 Testing
- Unit tests for parsing generic declarations
- Type checking tests for type variables
- Codegen tests for monomorphization
- End-to-end tests with generic structs/functions/enums

**Estimated Effort:** 4-5 days

---

## PHASE 2: MEDIUM PRIORITY RUNTIME FEATURES (Tier 2)

### 2.1 Thread Pool API (`jocky_threadpool_*`)
```c
// Create thread pool with N workers
jocky_threadpool_t pool = jocky_threadpool_create(4);

// Submit work (function pointer + context)
jocky_threadpool_submit(pool, callback, context);

// Wait for all work to complete
jocky_threadpool_wait(pool);

// Cleanup
jocky_threadpool_destroy(pool);
```

**Implementation:**
- Windows: ThreadPool API (TP_CALLBACK_ENVIRON)
- Linux: pthreads with work queue
- Cross-platform abstraction layer

**Files to create:**
- `src/runtime/threading/threadpool_windows.c`
- `src/runtime/threading/threadpool_linux.c`
- `src/runtime/include/jocky_threadpool.h`

**Tests:** 10+ tests for queue operations, thread safety

---

### 2.2 Cryptography Library (AES, RSA, ECDH)
```c
// AES encryption
jocky_aes_encrypt(plaintext, key, iv, ciphertext);

// RSA operations
jocky_rsa_encrypt(pubkey, data, ciphertext);

// ECDH key exchange
jocky_ecdh_generate_keypair(privkey, pubkey);
jocky_ecdh_shared_secret(privkey, pubkey, secret);
```

**Implementation:**
- Use OpenSSL/BoringSSL integration
- Or minimal implementations (TinyAES for AES)
- RSA via external lib (can't implement efficiently)
- ECDH via libsodium or similar

**Files to create:**
- `src/runtime/crypto/aes.c`
- `src/runtime/crypto/rsa.c`
- `src/runtime/crypto/ecdh.c`
- `src/runtime/include/jocky_crypto.h`

---

### 2.3 Network Primitives (TCP/UDP Sockets)
```c
// Create socket
jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);

// Connect/Listen
jocky_socket_connect(sock, host, port);
jocky_socket_listen(sock, backlog);

// Send/Receive
jocky_socket_send(sock, data, size);
jocky_socket_recv(sock, buffer, size);

// Cleanup
jocky_socket_close(sock);
```

**Implementation:**
- Windows: Winsock2
- Linux: BSD sockets
- Cross-platform wrapper

---

### 2.4 Compression (zlib)
```c
// Compress data
jocky_zlib_compress(input, input_size, output, output_size);

// Decompress data
jocky_zlib_decompress(input, input_size, output, output_size);
```

**Estimated Effort:** 1-2 days (external lib integration)

---

## PHASE 3: MEDIUM PRIORITY ANTI-ANALYSIS

### 3.1 Anti-IDA Techniques
- String obfuscation with key extraction functions
- Prologue/epilogue recognition evasion
- Dynamic import resolution (GetProcAddress chaining)
- Fake code interleaving

### 3.2 Anti-x64dbg
- TLS callback execution detection
- Debugger-specific register checks (hardware BP detection)
- Timing-based detection

### 3.3 Anti-Frida
- Process environment checks (LD_PRELOAD, etc.)
- Hook detection via function prologue scanning
- Symbol table integrity checks

### 3.4 AMSI Bypass
- AmsiScanBuffer function hooking
- Context parameter modification
- Return value patching

**Estimated Effort:** 3-4 days (requires Windows API deep knowledge)

---

## PHASE 4: MEDIUM PRIORITY OBFUSCATION

### 4.1 Virtualization Obfuscation
- Custom bytecode VM for critical sections
- Register allocation to VM registers
- Indirect branching via VM opcode dispatch

### 4.2 Polymorphic Obfuscation
- Mutate pass selection per compilation
- Random seed injection into build process
- Different obfuscation on each recompile

**Estimated Effort:** 5-7 days (complex LLVM work)

---

## Implementation Order

**Week 1:**
- Generics/Templates (4-5 days) [CRITICAL - completes language]
- Thread Pool (1 day)

**Week 2:**
- Crypto Library (1 day)
- Network Primitives (1-2 days)
- Compression (1 day)

**Week 3:**
- Anti-Analysis suite (3-4 days)

**Week 4:**
- Obfuscation enhancements (5-7 days)

---

## Success Criteria

✅ **Generics:** Can compile `fn max<T>(a: T, b: T) -> T` and call with multiple types
✅ **APIs:** Each API has 10+ tests, cross-platform stubs
✅ **Anti-Analysis:** Detection evasion verified with manual testing
✅ **Obfuscation:** Different bytecode on each recompile

---

## Blockers
- None identified
- All dependencies (OpenSSL, zlib) are standard libraries
- No new AST nodes critical path

---

## Next Steps
1. Design generic parameter representation
2. Update lexer/parser for `<T>` syntax
3. Implement monomorphization strategy
4. Begin Phase 1 implementation

