# JOCKY Bytecode VM Architecture

## Overview

The JOCKY Bytecode Virtual Machine is a stack-based interpreter designed for maximum obfuscation of sensitive functions. It converts selected functions into bytecode that executes on a custom 256-register, stack-based VM rather than native machine code.

## Key Characteristics

- **Stack-Based Execution**: Operations use a stack for intermediate values
- **256 Registers**: R0-R255, each 64-bit, for storing local values
- **40+ Instructions**: Comprehensive instruction set covering arithmetic, logic, memory, and control flow
- **Variable-Length Encoding**: Instructions are 1-5 bytes, reducing bytecode size
- **Polymorphic Dispatch**: Multiple instruction variants for same operation (anti-analysis)
- **Obfuscation Support**: Opaque predicates and control flow obfuscation built-in

## Architecture

### Register Model

```
Registers: R0-R255 (64-bit each)
Stack: 4KB (VM_MAX_STACK = 8192 entries)
Program Counter: 32-bit (bytecode offset)
Flags: Zero, Sign, Overflow, Carry
```

### Execution Model

```c
typedef struct {
    uint64_t registers[256];     // General purpose registers
    uint64_t stack[8192];        // Execution stack
    uint32_t sp;                 // Stack pointer
    uint32_t pc;                 // Program counter
    uint8_t *bytecode;           // VM bytecode
    uint32_t bytecode_len;       // Bytecode length
    uint64_t *call_stack;        // Return addresses
    uint32_t call_sp;            // Call stack pointer
    void *external_funcs[256];   // External function pointers
    uint32_t num_external;       // Number of external functions
    uint8_t flags;               // Processor flags
} jocky_vm_t;
```

## Instruction Set

### Categories

#### 1. Stack Operations (0x00-0x0A)
- `NOP` (0x00): No operation
- `PUSH_IMM8` (0x01): Push 8-bit immediate
- `PUSH_IMM16` (0x02): Push 16-bit immediate
- `PUSH_IMM32` (0x03): Push 32-bit immediate
- `PUSH_IMM64` (0x04): Push 64-bit immediate
- `PUSH_REG` (0x05): Push register value to stack
- `POP_REG` (0x06): Pop stack to register
- `POP_DISCARD` (0x07): Discard top of stack
- `MOV_REG` (0x08): Move register to register
- `SWAP` (0x09): Swap top two stack values
- `DUP` (0x0A): Duplicate top of stack

#### 2. Arithmetic (0x10-0x16)
- `ADD` (0x10): Pop two values, push sum
- `SUB` (0x11): Pop two values, push difference
- `MUL` (0x12): Pop two values, push product
- `DIV` (0x13): Signed division
- `UDIV` (0x14): Unsigned division
- `MOD` (0x15): Signed modulo
- `UMOD` (0x16): Unsigned modulo

#### 3. Bitwise/Logical (0x20-0x26)
- `AND` (0x20): Bitwise AND
- `OR` (0x21): Bitwise OR
- `XOR` (0x22): Bitwise XOR
- `NOT` (0x23): Bitwise NOT
- `SHL` (0x24): Shift left
- `SHR` (0x25): Logical shift right
- `SAR` (0x26): Arithmetic shift right

#### 4. Comparison (0x30-0x3A)
- `CMP` (0x30): Compare and set flags
- `CMP_EQ` (0x31): Push 1 if equal, 0 otherwise
- `CMP_NE` (0x32): Not equal
- `CMP_LT` (0x33): Less than (signed)
- `CMP_LE` (0x34): Less than or equal (signed)
- `CMP_GT` (0x35): Greater than (signed)
- `CMP_GE` (0x36): Greater than or equal (signed)
- `CMP_ULT` (0x37): Less than (unsigned)
- `CMP_ULE` (0x38): Less than or equal (unsigned)
- `CMP_UGT` (0x39): Greater than (unsigned)
- `CMP_UGE` (0x3A): Greater than or equal (unsigned)

#### 5. Memory Operations (0x40-0x47)
- `LOAD8` (0x40): Load 8-bit value from address
- `LOAD16` (0x41): Load 16-bit value
- `LOAD32` (0x42): Load 32-bit value
- `LOAD64` (0x43): Load 64-bit value
- `STORE8` (0x44): Store 8-bit value
- `STORE16` (0x45): Store 16-bit value
- `STORE32` (0x46): Store 32-bit value
- `STORE64` (0x47): Store 64-bit value

#### 6. Control Flow (0x50-0x56)
- `JMP` (0x50): Unconditional jump
- `JZ` (0x51): Jump if zero
- `JNZ` (0x52): Jump if not zero
- `JLT` (0x53): Jump if less than (uses flags)
- `JLE` (0x54): Jump if less than or equal
- `JGT` (0x55): Jump if greater than
- `JGE` (0x56): Jump if greater than or equal

#### 7. Function Calls (0x60-0x62)
- `CALL` (0x60): Call external function
- `RET` (0x61): Return from function
- `HALT` (0x62): Halt execution and return

#### 8. System (0x70)
- `SYSCALL` (0x70): System call

## Instruction Encoding

### Format

```
Byte 0: Opcode (8 bits)
Bytes 1-4: Operands (variable length)
```

### Operand Types

- **Register ID**: 1 byte (0-255)
- **Immediate8**: 1 byte
- **Immediate16**: 2 bytes (little-endian)
- **Immediate32**: 4 bytes (little-endian)
- **Immediate64**: 8 bytes (little-endian)
- **Offset**: 4 bytes (32-bit bytecode offset)

### Examples

```
PUSH_IMM8 42:
  01 2A

PUSH_REG R0:
  05 00

ADD:
  10

JMP 0x100:
  50 00 01 00 00

LOAD64:
  43
```

## Polymorphic Instruction Dispatch

The compiler randomly selects equivalent instruction variants during compilation. All variants execute identically:

```c
// Multiple opcodes for ADD operation
0x10 - ADD_VARIANT_A
0x11 - ADD_VARIANT_B
0x12 - ADD_VARIANT_C
0x13 - ADD_VARIANT_D
```

The interpreter uses a dispatch table to map all variants to the same handler:

```c
static const opcode_handler handlers[] = {
    [0x10] = handle_add,
    [0x11] = handle_add,
    [0x12] = handle_add,
    [0x13] = handle_add,
    // ...
};
```

## Obfuscation Techniques

### 1. Opaque Predicates

Fake conditional jumps that always follow the same path:

```c
// Always true predicate
if (x & (x+1) == 0) { /* never taken */ }
// jump to actual code

// Always false predicate  
if (x ^ x == 1) { /* never taken */ }
// jump to actual code
```

### 2. Control Flow Flattening

Linearize nested control flow into state machine:

```
Original:
  if (cond) { A() } else { B() }
  C()

Flattened:
  state = 0
  loop:
    switch(state):
      case 0: state = cond ? 1 : 2; goto loop
      case 1: A(); state = 3; goto loop
      case 2: B(); state = 3; goto loop
      case 3: C(); break
```

### 3. Polymorphic Instructions

Same operation, different opcodes:
- Makes reverse engineering harder
- Blocks simple pattern matching
- Enables runtime instruction rewriting

### 4. Register Randomization

Compiler assigns random registers to each local variable instead of sequential allocation.

### 5. Bytecode Shuffling

Reorder independent bytecode sequences with jumps:

```
Original: A; B; C; D
Obfuscated:
  JMP to A
  D
  JMP to C
  B
  JMP to D
  A
  JMP to B
  C
```

## Compilation Process

### Step 1: Parse and Analyze

- Identify functions marked with `@virtualize`
- Extract control flow graph
- Analyze data dependencies

### Step 2: Bytecode Generation

- Convert LLVM IR or AST to stack operations
- Allocate registers for local variables
- Generate jump targets

### Step 3: Obfuscation

- Apply obfuscation passes:
  1. Polymorphic instruction selection
  2. Opaque predicate injection
  3. Control flow flattening
  4. Register randomization

### Step 4: Encoding

- Encode instructions with variable-length format
- Embed in .jvm section
- Create function offset table

### Step 5: Runtime Wrapper

- Generate LLVM IR function stub
- Call `jocky_vm_load()` with bytecode
- Call `jocky_vm_execute()` with arguments
- Extract return value

## API Reference

### Initialization

```c
void jocky_vm_init(void);
void jocky_vm_destroy(void);
```

### Bytecode Loading

```c
void jocky_vm_load(uint8_t *bytecode, size_t size);
```

### Execution

```c
uint64_t jocky_vm_execute(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
```

### External Functions

```c
void jocky_vm_register_external(uint32_t index, void *func);
```

### Register Access

```c
uint64_t jocky_vm_get_register(uint32_t reg_idx);
void jocky_vm_set_register(uint32_t reg_idx, uint64_t value);
```

## Error Handling

### Stack Overflow
Returns 0 on stack overflow (>8192 entries)

### Invalid Register
Bounds check on register access (0-255)

### Division by Zero
Returns 0 on division by zero

### Invalid Bytecode
Gracefully handles corrupted bytecode with error codes

## Performance Characteristics

### Bytecode Size

Typical function with 100 instructions:
- Native x86-64: ~200-300 bytes
- JOCKY Bytecode: ~150-250 bytes (with obfuscation)

### Execution Overhead

Approximate slowdown vs. native code:
- Simple arithmetic: 5-10x slower
- Memory operations: 2-3x slower
- Control flow: 3-5x slower

### Optimization

Use `@virtualize` selectively on:
- Cryptographic functions
- Key validation routines
- Sensitive algorithms
- Critical security checks

Avoid virtualization on:
- Hot loops
- Performance-critical paths
- Recursive functions (deep recursion)

## Security Considerations

### Strengths
- Bytecode interpretation defeats static analysis
- Polymorphic instructions defeat pattern matching
- Opaque predicates increase reverse engineering effort
- No native machine code to disassemble

### Limitations
- Dynamic analysis can still trace execution
- Debuggers can intercept VM calls
- Power analysis can reveal operations
- Timing attacks may leak information

### Best Practices

1. **Defense in Depth**: Combine with other obfuscation
2. **Selective Use**: Virtualize only sensitive code
3. **Anti-Analysis**: Add VM anti-tampering checks
4. **Obfuscation Levels**: Use higher levels for critical code

## Future Enhancements

1. **JIT Compilation**: Cache frequently executed bytecode
2. **Vectorized Operations**: SIMD support for performance
3. **Encrypted Bytecode**: Decrypt at runtime
4. **VM Hardening**: Anti-debugging, anti-tampering
5. **Custom Instruction Set**: Per-binary obfuscation kernel
