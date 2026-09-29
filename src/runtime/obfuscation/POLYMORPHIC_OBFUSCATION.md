# Polymorphic Obfuscation System

Advanced runtime mutation and randomization for JOCKY, enabling code to change its obfuscation strategy on each execution.

## Architecture Overview

### Three-Layer Mutation System

1. **Instruction-Level Mutations** — Substitute individual instructions while preserving semantics
2. **Control Flow Mutations** — Randomize basic block ordering and dispatch mechanisms
3. **Data Mutations** — Randomize constants and data values with compensation

### Pass-Based Execution

The mutation engine supports a registry of specialized mutation passes, each focusing on a specific transformation technique. Passes are:
- **Selected** based on randomized probability weighted by intensity
- **Sorted** by priority for optimal effectiveness
- **Shuffled** for each execution to vary the ordering
- **Applied** in sequence to the binary code

## Core APIs

### Initialization

```c
// Initialize with optional seed (0 = random) and intensity (1-5)
void jocky_mutation_init(MutationEngine *engine, uint32_t seed, int intensity);

// Apply random mutations to code
void jocky_apply_polymorphic_mutations(uint8_t *code, size_t len);

// Apply mutations with specific intensity
void jocky_apply_mutations_intensity(uint8_t *code, size_t len, int intensity);
```

### Instruction Mutations

```c
// Substitute equivalent instructions (ADD → LEA, MOV sequences, etc.)
void jocky_mutate_instructions(uint8_t *code, size_t len);

// Randomize immediates with compensation
void jocky_mutate_constants(uint8_t *code, size_t len);

// Inject semantically-irrelevant junk code
void jocky_inject_junk_code(uint8_t *code, size_t len);

// Apply De Morgan's law substitutions
void jocky_mutate_bitwise_ops(uint8_t *code, size_t len);

// XOR-obfuscate data values
void jocky_mutate_data_values(uint8_t *code, size_t len);
```

### Control Flow Mutations

```c
// Extract basic blocks from binary code
BasicBlock *jocky_extract_basic_blocks(uint8_t *code, size_t len, int *out_count);

// Randomize basic block execution order
void jocky_randomize_cfg(uint8_t *code, size_t len);

// Create polymorphic dispatch tables with XOR-obfuscated targets
void jocky_create_polymorphic_dispatch(uint8_t *code, size_t len);

// Reorder switch cases
void jocky_randomize_switch_cases(uint8_t *code, size_t len);
```

### Self-Modifying Code

```c
// Make code sections writable for runtime mutation
void jocky_enable_code_mutation(uintptr_t code_addr, size_t code_len);

// Apply mutations to writable code segment
void jocky_apply_self_mutations(SelfModifyingSegment *segment);

// Verify code integrity after mutations
bool jocky_verify_code_integrity(SelfModifyingSegment *segment);

// Periodically re-mutate code sections
void jocky_enable_periodic_mutation(int interval_seconds);
```

### Randomization Utilities

```c
// Seed RNG (0 = use entropy from clock/PID)
void jocky_seed_rng(uint32_t seed);

// Random in range [min, max)
uint32_t jocky_random_range(uint32_t min, uint32_t max);

// Random u32/u64
uint32_t jocky_random_u32(void);
uint64_t jocky_random_u64(void);

// In-place Fisher-Yates shuffle
void jocky_shuffle_array(void *array, size_t count, size_t elem_size);
```

## Mutation Strategies

### Instruction Substitution (Priority 2)

Replaces semantically equivalent x86-64 instruction sequences:

- `ADD r1, r2` → `LEA r1, [r1+r2]`
- `MOV r1, r2` → sequence of `PUSH` + `POP`
- `XOR r1, r1` → `SUB r1, r1`

**Effectiveness:** 40% of ADD instructions, 30% of XOR instructions
**Cost:** ~5-10% runtime overhead per transformation

### Constant Randomization (Priority 1)

For each immediate constant in the binary:

1. Add random offset K to the constant value
2. Inject `ADD/SUB` instructions to compensate in the calculation

Example:
```
Original:   MOV rax, 0x12345678
Mutated:    MOV rax, 0x12345678 ^ random_key
            ; Later: ADD rax, -random_key
```

**Effectiveness:** 50% of constants
**Cost:** ~2-3% overhead

### Junk Code Injection (Priority 1)

Inserts semantically-irrelevant instruction sequences:
- `PUSH rax; POP rax` (register shuffle)
- `XOR r8, r8; XOR r8, r8` (no-op double XOR)
- Multi-byte NOP sequences

**Coverage:** ~0.5% of code buffer
**Cost:** ~1% size increase

### Bitwise Operation Mutation (Priority 2)

Applies De Morgan's laws to bitwise operations:
- `AND r1, r2` ↔ `OR r1, r2` (with negation)
- `OR r1, r2` ↔ `XOR r1, r2` (with masking)

Requires surrounding compensation instructions.

**Coverage:** ~33% of bitwise ops
**Cost:** ~3-5% overhead

### Control Flow Randomization (Priority 3, Highest)

1. **Basic Block Reordering:** Extract CFG, shuffle blocks, update jump targets
2. **Polymorphic Dispatch:** XOR-obfuscate jump targets:
   ```
   Normal:       JMP rax
   Polymorphic:  MOV r8, random_key
                 MOV r9, target XOR random_key
                 XOR r8, r9
                 JMP r8
   ```
3. **Switch Case Reordering:** Shuffle jump table entries

**Effectiveness:** High impact on control flow analysis
**Cost:** ~5-15% runtime overhead

## Intensity Levels

### Intensity 1 (Minimal)
- 20% pass selection rate
- Few instruction mutations
- No control flow changes
- **Overhead:** <5%

### Intensity 2 (Moderate)
- 40% pass selection rate
- Moderate constant randomization
- Basic control flow randomization
- **Overhead:** 5-10%

### Intensity 3 (Balanced) — Default
- 60% pass selection rate
- All major mutations enabled
- Full control flow randomization
- **Overhead:** 10-20%

### Intensity 4 (Aggressive)
- 80% pass selection rate
- Frequent instruction substitution
- Multiple rounds of mutation
- **Overhead:** 20-30%

### Intensity 5 (Maximum)
- 100% pass selection
- All mutations applied multiple times
- Comprehensive junk injection
- **Overhead:** 30-50%

## Usage Examples

### Basic Runtime Mutation

```c
#include "mutation.h"

int main() {
    uint8_t code[] = { /* binary code */ };
    size_t code_len = sizeof(code);

    // Initialize with seed 0 (random) and intensity 3
    MutationEngine engine;
    jocky_mutation_init(&engine, 0, 3);

    // Apply mutations
    jocky_apply_polymorphic_mutations(code, code_len);

    // Code is now mutated in-place
    return 0;
}
```

### Self-Modifying Code with Verification

```c
SelfModifyingSegment segment = {
    .address = (uintptr_t)my_function,
    .mutation_payload = mutated_copy,
    .payload_len = function_size,
    .checksum = jocky_compute_code_checksum(mutated_copy, function_size)
};

// Replace code in place
jocky_apply_self_mutations(&segment);

// Later: verify integrity
if (!jocky_verify_code_integrity(&segment)) {
    // Code was tampered with
    exit(1);
}
```

### Periodic Mutation

```c
jocky_enable_periodic_mutation(30);  // Re-mutate every 30 seconds

// Signal handler will trigger mutation
signal(SIGALRM, on_mutation_timer);
```

### Mutation Control

```c
// Disable mutations temporarily
jocky_set_mutation_enabled(false);

// Do critical work...

// Re-enable
jocky_set_mutation_enabled(true);

// Get statistics
MutationStats stats = jocky_get_mutation_stats();
printf("Applied: %d mutations, modified %d instructions\n",
       stats.mutations_applied, stats.instructions_modified);
```

## Performance Characteristics

### Runtime Overhead by Intensity

| Intensity | Overhead | Use Case |
|-----------|----------|----------|
| 1         | <5%      | Minimal evasion, performance-critical |
| 2         | 5-10%    | Light obfuscation |
| 3         | 10-20%   | Balanced (default) |
| 4         | 20-30%   | Aggressive evasion |
| 5         | 30-50%   | Maximum anti-analysis |

### Memory Usage

- RNG state: 4 bytes
- Pass registry: ~200 bytes
- Statistics: ~32 bytes
- Per-segment overhead: ~40 bytes

### Cache Impact

Mutation may reduce instruction cache efficiency:
- Junk code adds 2-5% cache pollution
- Instruction substitution can improve locality
- Control flow randomization may increase branch mispredicts by 3-8%

## Evasion Techniques

### Against Static Analysis

- **Polymorphic dispatch** defeats jump table analysis
- **Instruction substitution** breaks pattern matching
- **Junk code** confuses IDA disassemblers
- **Constant randomization** prevents string matching

### Against Dynamic Analysis

- **Self-modifying code** defeats tracing
- **Control flow randomization** breaks execution monitoring
- **Periodic mutation** defeats memory snapshots
- **Periodic checksums** detect code tampering

### Against Reverse Engineering

- **No two executions identical** prevents reproducible disassembly
- **Polymorphic prologues** confuse frame pointer analysis
- **Bitwise law substitutions** make data flow analysis harder
- **XOR-obfuscated targets** prevent jump target recovery

## Integration with JOCKY Pipeline

### Python API

```python
from jocky.passes import registry

# Register polymorphic pass
registry.PASS_REGISTRY['polymorphic_mutation'] = 'polymorphic'
registry.RUNTIME_PASSES.add('polymorphic')

# Use in compilation pipeline
compiler = JockyCompiler()
compiler.apply_passes(['polymorphic_mutation'], intensity=3)
```

### Compile-Time Configuration

```python
# In profile or CLI
obfuscate(
    polymorphic_mutation={
        'intensity': 3,        # 1-5
        'seed': 0,             # 0 = random
        'self_modifying': True,
        'periodic_mutation': 30
    }
)
```

## Testing & Validation

### Test Coverage

- 12+ unit tests in `mutation_test.c`
- Covers all mutation operators
- Validates RNG distribution
- Tests self-modification safety

### Running Tests

```bash
cd /home/deval/JOCKY/src/runtime/build
cmake -B . -S ..
make
ctest -V
```

### Validation Strategy

1. **Semantic Equivalence:** Verify mutated code produces same output
2. **Randomness Quality:** Check RNG distribution uniformity
3. **Crash Testing:** Run mutated code under heavy stress
4. **Instruction Verification:** Disassemble and check validity
5. **Performance Profiling:** Measure overhead per intensity

## Limitations & Future Work

### Current Limitations

- Basic block extraction is conservative (may underestimate blocks)
- Self-modification requires writable `.text` section (not portable)
- RNG is fast but not cryptographically secure
- No feedback-driven mutation based on detection

### Future Enhancements

- Hardware-accelerated RNG (RDRAND on x86)
- Feedback loop from anti-analysis checks
- Context-aware mutations (OS/hypervisor detection)
- Hybrid compile-time + runtime mutations
- Machine learning-based mutation selection

## Security Considerations

### Strengths

- Changes per execution defeat pattern matching
- Self-modification defeats memory snapshots
- Polymorphic dispatch resists IDA analysis
- Checksum verification detects tampering

### Weaknesses

- RNG can be predicted if seed is known
- Mutations may be reconstructible via symbolic execution
- Writable code section is a security risk
- Performance overhead may be detectable

### Recommendations

- Combine with other evasion techniques
- Use high intensity for sensitive functions
- Pair with anti-debug and anti-analysis checks
- Monitor for EDR hooks on memory protection calls

## References

- x86-64 Instruction Set Architecture
- De Morgan's Laws in Boolean Algebra
- Polymorphic and Metamorphic Code
- Runtime Code Mutation Techniques
