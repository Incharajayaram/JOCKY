import pytest
import sys
import pathlib

sys.path.insert(0, str(pathlib.Path(__file__).parent.parent.parent / "src"))

from jocky.language.parser import parse_source
from jocky.language.checker import TypeChecker
from jocky.language.codegen import CodeGen, CodeGenError


def _ir(source: str) -> str:
    ast = parse_source(source)
    tc = TypeChecker()
    tc.check(ast)
    return CodeGen().gen(ast)


def _fn_ir(body: str, params: str = "", ret: str = "void") -> str:
    return _ir(f"fn f({params}) -> {ret} {{\n{body}\n}}")


# ============================================================
# IR structure
# ============================================================

def test_codegen_entry_block_alloca():
    """Alloca for a loop-local variable must appear in the entry block, not inside the loop."""
    src = "fn f() -> void { while true { let x: i32 = 0; } }"
    ir = _ir(src)
    lines = ir.splitlines()
    alloca_lines = [i for i, l in enumerate(lines) if "alloca" in l]
    non_entry_label_lines = [
        i for i, l in enumerate(lines)
        if l.rstrip().endswith(":") and "entry" not in l
    ]
    assert alloca_lines, "no alloca found in IR"
    assert non_entry_label_lines, "no non-entry label found in IR"
    assert max(alloca_lines) < min(non_entry_label_lines), (
        "alloca must appear before any non-entry label"
    )


def test_codegen_ffi_declare():
    ir = _ir("ffi printf(fmt: string, ...) -> i32;")
    assert "declare i32 @printf(i8*, ...)" in ir


def test_codegen_string_dedup():
    src = """
ffi puts(s: string) -> i32;
fn main() -> i32 {
    puts("hello");
    puts("hello");
    return 0;
}
"""
    ir = _ir(src)
    # Only one constant definition for the same string literal
    constant_defs = [l for l in ir.splitlines() if "private constant" in l and "hello" in l]
    assert len(constant_defs) == 1


def test_codegen_function_signature():
    ir = _ir("fn add(a: i32, b: i32) -> i32 { return a + b; }")
    assert "define i32 @add(i32 %a, i32 %b)" in ir


def test_codegen_implicit_void_return():
    ir = _fn_ir("let x: i32 = 1;")
    assert "ret void" in ir


# ============================================================
# Arithmetic / logic IR instructions
# ============================================================

def test_codegen_add():
    ir = _fn_ir("let a: i32 = 1; let b: i32 = 2; let c: i32 = a + b;", ret="void")
    assert "add i32" in ir


def test_codegen_sub():
    ir = _fn_ir("let a: i32 = 3; let b: i32 = 1; let c: i32 = a - b;", ret="void")
    assert "sub i32" in ir


def test_codegen_div():
    ir = _fn_ir("let a: i32 = 10; let b: i32 = 2; let c: i32 = a / b;", ret="void")
    assert "sdiv i32" in ir


def test_codegen_mod():
    ir = _fn_ir("let a: i32 = 10; let b: i32 = 3; let c: i32 = a % b;", ret="void")
    assert "srem i32" in ir


def test_codegen_negate():
    ir = _fn_ir("let x: i32 = 5; let y: i32 = -x;", ret="void")
    assert "sub i32 0," in ir


def test_codegen_bool_not():
    ir = _fn_ir("let b: bool = true; let r: bool = !b;", ret="void")
    assert "xor i1" in ir


# ============================================================
# Bitwise IR instructions
# ============================================================

def test_codegen_or():
    ir = _fn_ir("let a: i32 = 0; let b: i32 = 0; let r: i32 = a | b;", ret="void")
    assert "or i32" in ir


def test_codegen_xor():
    ir = _fn_ir("let a: i32 = 0; let b: i32 = 0; let r: i32 = a ^ b;", ret="void")
    assert "xor i32" in ir


def test_codegen_bitand():
    ir = _fn_ir("let a: i32 = 0; let b: i32 = 0; let r: i32 = a & b;", ret="void")
    assert "and i32" in ir


def test_codegen_shl():
    ir = _fn_ir("let a: i32 = 1; let b: i32 = 3; let r: i32 = a << b;", ret="void")
    assert "shl i32" in ir


def test_codegen_shr():
    ir = _fn_ir("let a: i32 = 8; let b: i32 = 1; let r: i32 = a >> b;", ret="void")
    assert "lshr i32" in ir


def test_codegen_bitnot():
    ir = _fn_ir("let a: i32 = 0; let r: i32 = ~a;", ret="void")
    assert "xor i32" in ir
    assert "-1" in ir


# ============================================================
# Short-circuit IR (phi nodes)
# ============================================================

def test_codegen_short_circuit_and():
    ir = _fn_ir("let a: bool = true; let b: bool = false; let r: bool = a && b;", ret="void")
    assert "sc_rhs" in ir
    assert "sc_merge" in ir
    assert "phi i1 [ 0," in ir


def test_codegen_short_circuit_or():
    ir = _fn_ir("let a: bool = true; let b: bool = false; let r: bool = a || b;", ret="void")
    assert "sc_rhs" in ir
    assert "sc_merge" in ir
    assert "phi i1 [ 1," in ir


def test_codegen_short_circuit_no_unconditional_and():
    """&& must NOT use 'and i1' — it must use conditional branches."""
    ir = _fn_ir("let a: bool = true; let b: bool = false; let r: bool = a && b;", ret="void")
    assert "and i1" not in ir


def test_codegen_short_circuit_no_unconditional_or():
    """|| must NOT use 'or i1' — it must use conditional branches."""
    ir = _fn_ir("let a: bool = true; let b: bool = false; let r: bool = a || b;", ret="void")
    assert "or i1" not in ir


# ============================================================
# Break / continue IR
# ============================================================

def test_codegen_break_in_while():
    ir = _fn_ir("while true { break; }", ret="void")
    # break must emit a br to the while_exit label
    assert "while_exit" in ir
    # the while_exit label must be reached by a br from break
    lines = ir.splitlines()
    br_lines = [l.strip() for l in lines if l.strip().startswith("br label") and "while_exit" in l]
    assert br_lines, "break must emit 'br label %while_exit...'"


def test_codegen_continue_in_while():
    ir = _fn_ir("let i: i32 = 0; while i < 10 { continue; }", ret="void")
    lines = ir.splitlines()
    # Find the while header label (e.g. "while.0:")
    header = None
    for l in lines:
        s = l.strip()
        if s.endswith(":") and s.startswith("while."):
            header = s[:-1]
            break
    assert header, "while header label not found"
    # Expect at least 2 branches to the header: initial entry + the one from continue
    branches = sum(1 for l in lines if f"br label %{header}" in l)
    assert branches >= 2, "continue must emit a branch back to the while header"


def test_codegen_break_in_for():
    ir = _fn_ir("for (let i: i32 = 0; i < 10; i = i + 1) { break; }", ret="void")
    assert "for_exit" in ir
    lines = ir.splitlines()
    br_lines = [l.strip() for l in lines if l.strip().startswith("br label") and "for_exit" in l]
    assert br_lines, "break in for must emit 'br label %for_exit...'"


def test_codegen_continue_in_for():
    """continue in a for loop must jump to the step block, not the condition block."""
    ir = _fn_ir("for (let i: i32 = 0; i < 10; i = i + 1) { continue; }", ret="void")
    assert "for_step" in ir
    lines = ir.splitlines()
    br_to_step = [l.strip() for l in lines if "br label" in l and "for_step" in l]
    assert br_to_step, "continue in for must branch to for_step label"


# ============================================================
# Null literal IR
# ============================================================

def test_codegen_null():
    ir = _fn_ir("let p: i8* = null;", ret="void")
    assert "store i8* null," in ir


# ============================================================
# Cast IR instructions
# ============================================================

def test_codegen_cast_sext():
    ir = _fn_ir("let x: i32 = 0; let y: i64 = (i64)x;", ret="void")
    assert "sext i32" in ir


def test_codegen_cast_trunc():
    ir = _fn_ir("let x: i64 = 0; let y: i32 = (i32)x;", ret="void")
    assert "trunc i64" in ir


def test_codegen_cast_inttoptr():
    ir = _ir("fn f() -> i8* { return (i8*)0; }")
    assert "inttoptr i32" in ir
    assert "to i8*" in ir


def test_codegen_cast_bitcast():
    ir = _ir("fn f(p: i8*) -> i32* { return (i32*)p; }")
    assert "bitcast i8*" in ir
    assert "to i32*" in ir


def test_codegen_cast_i8_to_i32_sext():
    ir = _fn_ir("let x: i8 = 0; let y: i32 = (i32)x;", ret="void")
    assert "sext i8" in ir


# ============================================================
# Type promotion in binary ops
# ============================================================

def test_codegen_type_promotion_i8_i32():
    """When i8 and i32 are added, the i8 is sext'd to i32 before the add."""
    # Use parameters so they are in self.locals when collect_lets runs
    ir = _ir("fn f(a: i8, b: i32) -> void { let r = a + b; }")
    assert "sext i8" in ir
    assert "add i32" in ir


# ============================================================
# Pointer operation IR
# ============================================================

def test_codegen_deref():
    ir = _ir("fn f(ptr: i32*) -> i32 { return *ptr; }")
    assert "load i32, i32*" in ir


def test_codegen_addr_of():
    """&x returns the alloca address — no load i32 should be emitted for the return."""
    ir = _ir("fn f(x: i32) -> i32* { return &x; }")
    assert "ret i32*" in ir
    # Verify no load of i32 value was needed for the addr-of expression
    lines = ir.splitlines()
    load_lines = [l for l in lines if "load i32," in l and "i32**" not in l]
    assert not load_lines, "&x should not require a load of the i32 value"


def test_codegen_index():
    ir = _ir("fn f(arr: i8*, i: i32) -> i8 { return arr[i]; }")
    assert "getelementptr" in ir
    assert "load i8, i8*" in ir
