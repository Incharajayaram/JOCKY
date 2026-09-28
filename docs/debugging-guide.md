# JOCKY Debugger Guide

Enable source-level debugging of JOCKY compiled programs using GDB or LLDB.

## Features

- **Source-level debugging**: Step through JOCKY source code line by line
- **Breakpoints**: Set breakpoints at functions, files, or line numbers
- **Stack traces**: Inspect the call stack at any point
- **Variable inspection**: Print and examine variable values
- **Register inspection**: View CPU register states
- **Memory inspection**: Examine memory contents at addresses
- **Instruction stepping**: Step through individual machine instructions

## Prerequisites

- GDB (default) or LLDB installed on your system
- JOCKY programs compiled with debug symbols (enabled by default)

On Linux:
```bash
apt-get install gdb  # or lldb
```

On macOS:
```bash
brew install gdb  # or lldb (included with Xcode)
```

On Windows:
```bash
# GDB available via MSYS2/MinGW or WSL
```

## Compiling for Debugging

Debug symbols are generated automatically during compilation. To compile a JOCKY program with debugging support:

```bash
jocky build myprogram.jky
```

This generates an executable with DWARF 4 debug information included. The debug information includes:

- Source file references
- Line and column information for all statements
- Function signatures and parameter types
- Local variable type information

## Quick Start

### Start debugging a program:

```bash
jocky debug myprogram
```

### Set breakpoints before running:

```bash
jocky debug myprogram --breakpoint main --breakpoint process_data
```

### Pass arguments to the program:

```bash
jocky debug myprogram arg1 arg2 arg3
```

### Use LLDB instead of GDB:

```bash
jocky debug --lldb myprogram
```

## Debugger Commands

Once the debugger starts, you can use these commands:

### Navigation
- **`c`, `continue`**: Resume execution until next breakpoint
- **`s`, `step`**: Execute one source line (step into function calls)
- **`n`, `next`**: Execute one source line (step over function calls)
- **`si`, `stepi`**: Execute one machine instruction

### Inspection
- **`bt`, `backtrace`**: Print the call stack
- **`reg`, `registers`**: Print all CPU registers
- **`print VAR`**: Print the value of a variable
- **`print *PTR`**: Dereference and print pointed value
- **`print &VAR`**: Print the address of a variable

### Breakpoints
- **`break LOCATION`**: Set a breakpoint
  - `break main` - At function start
  - `break file.jky:42` - At specific line
  - `break process_data:10` - At function+offset

### Other
- **`help`**: Show command list
- **`quit`, `exit`**: Exit the debugger

## Interactive Debug Session

Example debugging session:

```bash
$ jocky debug fibonacci --breakpoint fibonacci --breakpoint main
JOCKY Debugger
  Executable    fibonacci
  Debugger      gdb

  Setting breakpoint at fibonacci
  Setting breakpoint at main
  Starting fibonacci...

Type 'help' for commands, 'quit' to exit
(jocky-dbg) c
Continuing...
Breakpoint 1, main () at fibonacci.jky:5
(jocky-dbg) print n
$1 = 10
(jocky-dbg) step
...
(jocky-dbg) bt
#0  fibonacci (n=5) at fibonacci.jky:12
#1  0x00005555555546b0 in fibonacci (n=6) at fibonacci.jky:15
#2  0x00005555555546b0 in main () at fibonacci.jky:5
(jocky-dbg) continue
...
(jocky-dbg) quit
```

## Debug Information Format

JOCKY generates DWARF 4 debug information, which is the standard format used by GDB, LLDB, and other debuggers.

### Compilation Unit Metadata

Each compiled program includes:
- Source file reference
- Compiler identification (JOCKY Compiler)
- Optimization flags
- Emission kind (full debug info)

### Function Information

For each function:
- Source location (file, line, column)
- Function signature and return type
- Parameter types and names
- Local variable type information

### Type Information

All JOCKY types are mapped to DWARF types:
- `void` → DW_ATE_void
- `bool` → DW_ATE_boolean
- `i8` → DW_ATE_signed (8-bit)
- `i32` → DW_ATE_signed (32-bit)
- `i64` → DW_ATE_signed (64-bit)
- `string` → DW_ATE_address
- User-defined types → Encoded as address types

## Breakpoint Implementation

Breakpoints are implemented using:

- **On x86/x64**: INT3 (0xCC) instruction replacement
- **Memory protection**: Pages made writable during breakpoint setup
- **Restoration**: Original instruction restored before continuing

The debugger maintains a breakpoint registry and automatically:
1. Saves the original instruction at the breakpoint address
2. Replaces it with INT3
3. Catches the SIGTRAP signal
4. Restores the original instruction
5. Continues execution

## Debugging Common Issues

### "Debugger not found" error

Make sure GDB or LLDB is installed:
```bash
# Check if GDB is installed
which gdb
gdb --version

# Check if LLDB is installed
which lldb
lldb --version
```

### Symbols not found

Ensure the program was compiled with JOCKY (which includes debug symbols by default):
```bash
# Compile with debug symbols
jocky build myprogram.jky
```

Check that debug symbols are in the executable:
```bash
# With GDB
gdb myprogram
(gdb) info sources

# With LLDB
lldb myprogram
(lldb) script print(lldb.target)
```

### Breakpoint not hit

1. Check breakpoint syntax: `break function_name` or `break file.jky:line`
2. Verify the function/location exists in the source
3. Step through manually to confirm execution path
4. Use `backtrace` to see current location

### Program crashes in debugger

1. Run with `step` or `next` to find exact crash location
2. Use `backtrace` to see call stack
3. Use `print VAR` to inspect variable states before crash
4. Check memory around crash address with GDB commands

## Advanced Debugging

### GDB-specific commands

Access GDB directly within JOCKY debugger:
```
(jocky-dbg) break main                    # GDB command
(jocky-dbg) print $rbp                    # Print register
(jocky-dbg) print *(int*)$rbp             # Dereference
```

### LLDB-specific commands

```
(jocky-dbg) break set --name main         # LLDB breakpoint syntax
(jocky-dbg) reg read                      # LLDB register command
```

### Performance profiling

While debugging, you can identify performance hotspots:

1. Set a breakpoint in a loop
2. Note the address of the function
3. Use `stepi` to watch instruction-level execution
4. Count instructions for performance-critical code

## Example: Debugging a JOCKY Program

### fibonacci.jky
```jocky
fn fibonacci(n: i32) -> i32 {
    if n <= 1 {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

fn main() -> i32 {
    let result: i32 = fibonacci(10);
    return result;
}
```

### Debug session:

```bash
$ jocky build fibonacci.jky
$ jocky debug fibonacci --breakpoint fibonacci

(jocky-dbg) break fibonacci           # Set breakpoint at function
(jocky-dbg) c                          # Continue
# Program stops at first call to fibonacci

(jocky-dbg) print n                    # Print parameter
$1 = 10

(jocky-dbg) step                       # Step into function
(jocky-dbg) step                       # Execute next line

(jocky-dbg) bt                         # See call stack
#0  fibonacci (n=10) at fibonacci.jky:2
#1  0x....... in main () at fibonacci.jky:8

(jocky-dbg) continue                   # Continue until next breakpoint
# Stops at n=9

(jocky-dbg) quit
```

## Runtime Breakpoint API

For advanced use cases, JOCKY runtime provides C API:

```c
#include "jocky_debug.h"

// Set a breakpoint at address with location info
jocky_set_breakpoint(0x1234567, "fibonacci.jky:5");

// Remove breakpoint
jocky_remove_breakpoint(0x1234567);

// Enable/disable without removing
jocky_enable_breakpoint(0x1234567, false);

// Set debug callback
void my_debug_callback(const char *event, uintptr_t addr) {
    printf("Debug event: %s at 0x%lx\n", event, addr);
}
jocky_set_debug_callback(my_debug_callback);

// Initialize debugger
jocky_debug_init();
```

## Performance Impact

Debug symbols add:
- **File size**: ~20-30% larger executable
- **Runtime overhead**: None (debug info is only used by debugger)
- **Compilation time**: <5% overhead for debug info generation

Debug symbols are NOT included in stripped executables:
```bash
strip executable  # Remove debug symbols after debugging
```

## Support and Limitations

### Supported
- ✅ Function breakpoints
- ✅ Line-based breakpoints
- ✅ Variable inspection (locals, parameters)
- ✅ Stack traces
- ✅ Memory inspection
- ✅ Instruction stepping
- ✅ GDB and LLDB support

### Not Yet Supported
- ❌ Conditional breakpoints (use GDB `condition` command directly)
- ❌ Watch points (memory watches)
- ❌ Remote debugging (connect directly to GDB/LLDB)
- ❌ Debugging closures (limited support)

## Troubleshooting

See the [Troubleshooting Guide](troubleshooting.md) for solutions to common debugger issues.

## See Also

- [GDB Documentation](https://sourceware.org/gdb/documentation/)
- [LLDB Documentation](https://lldb.llvm.org/use/tutorial.html)
- [DWARF Debugging Format](https://dwarfstd.org/)
