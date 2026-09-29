MLIR_PASSES = [
    {
        "id": "string_encrypt",
        "name": "String Encryption",
        "description": "Encrypts all string literals with XOR/RC4 and decrypts at runtime",
        "default": True,
        "flag": "--string-encrypt",
    },
    {
        "id": "constant_obfuscate",
        "name": "Constant Obfuscation",
        "description": "Replaces numeric constants with opaque expressions",
        "default": True,
        "flag": "--constant-obfuscate",
    },
    {
        "id": "symbol_obfuscate",
        "name": "Symbol Obfuscation",
        "description": "Renames internal symbols to randomized identifiers",
        "default": True,
        "flag": "--symbol-obfuscate",
    },
]

LLVM_PASSES = [
    {
        "id": "boguscf",
        "name": "Bogus Control Flow",
        "description": "Inserts opaque predicates and dead code branches",
        "default": True,
        "flag": "boguscf",
    },
    {
        "id": "flattening",
        "name": "Control Flow Flattening",
        "description": "Converts structured control flow into a switch-based dispatcher",
        "default": True,
        "flag": "flattening",
    },
    {
        "id": "substitution",
        "name": "Instruction Substitution",
        "description": "Replaces standard instructions with equivalent complex sequences",
        "default": True,
        "flag": "substitution",
    },
    {
        "id": "split",
        "name": "Basic Block Splitting",
        "description": "Splits basic blocks to increase control flow graph complexity",
        "default": True,
        "flag": "split",
    },
    {
        "id": "indirect_call",
        "name": "Indirect Calls",
        "description": "Converts direct calls to indirect via function pointer tables",
        "default": True,
        "flag": "indirect-call",
    },
    {
        "id": "strip_signature",
        "name": "Strip Signatures",
        "description": "Removes debug metadata and function signature information",
        "default": True,
        "flag": "strip-signature",
    },
]
