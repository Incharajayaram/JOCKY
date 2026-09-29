"""Pass registry: maps profile-friendly names to actual opt pass flags."""

PASS_REGISTRY = {
    # LLVM IR passes
    "bogus_control_flow": "boguscf",
    "flatten_cfg": "flattening",
    "instruction_substitution": "substitution",
    "basic_block_split": "split",
    "linear_mba": "linear-mba",
    "opaque_predicates": "opaque-pred",
    "indirect_call": "indirect-call",
    "strip_signature": "strip-signature",
    "pdata_strip": "pdata-strip",
    "anti_debug": "anti-debug",
    "virtualize": "virtualize",
    # Runtime passes
    "polymorphic_mutation": "polymorphic",
    # MLIR passes
    "string_encrypt": "string-encrypt",
    "constant_obfuscate": "constant-obfuscate",
    "symbol_obfuscate": "symbol-obfuscate",
    "crypto_hash": "crypto-hash",
    "scf_obfuscate": "scf-obfuscate",
    "import_obfuscate": "import-obfuscate",
}

# Categorize passes
FUNCTION_PASSES = {
    "boguscf", "flattening", "substitution", "split",
    "linear-mba", "opaque-pred",
}

MODULE_PASSES = {
    "indirect-call", "strip-signature", "pdata-strip",
    "anti-debug", "virtualize",
}

MLIR_PASSES = {
    "string-encrypt", "constant-obfuscate", "symbol-obfuscate",
    "crypto-hash", "scf-obfuscate", "import-obfuscate",
}

def resolve_pass(name: str) -> str:
    if name in PASS_REGISTRY:
        return PASS_REGISTRY[name]
    # Allow direct pass flags too
    if name in FUNCTION_PASSES or name in MODULE_PASSES or name in MLIR_PASSES:
        return name
    raise ValueError(f"Unknown pass: {name}")

def format_opt_passes(pass_names: list[str]) -> str:
    """Format pass names for opt -passes= string."""
    llvm_passes = [p for p in pass_names if resolve_pass(p) not in MLIR_PASSES]
    if not llvm_passes:
        return ""

    fp = []
    mp = []
    for name in llvm_passes:
        flag = resolve_pass(name)
        if flag in FUNCTION_PASSES:
            fp.append(flag)
        elif flag in MODULE_PASSES:
            mp.append(flag)
        else:
            mp.append(flag)

    parts = []
    if fp:
        parts.append(f"function({','.join(fp)})")
    if mp:
        parts.extend(mp)
    return ",".join(parts)

def get_mlir_passes(pass_names: list[str]) -> list[str]:
    return [resolve_pass(p) for p in pass_names if resolve_pass(p) in MLIR_PASSES]
