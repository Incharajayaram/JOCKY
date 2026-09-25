"""
End-to-end tests: parse → type-check → codegen.
Each test drives the full language front-end pipeline and verifies the LLVM IR output.
"""
import pytest
import sys
import pathlib

sys.path.insert(0, str(pathlib.Path(__file__).parent.parent.parent / "src"))

from jocky.language.parser import parse_source
from jocky.language.checker import TypeChecker
from jocky.language.checker import TypeError as JockyTypeError
from jocky.language.codegen import CodeGen


def _ir(source: str) -> str:
    ast = parse_source(source)
    tc = TypeChecker()
    tc.check(ast)
    return CodeGen().gen(ast)


HELLO_WORLD = """
ffi puts(s: string) -> i32;
fn main() -> i32 {
    puts("Hello, JOCKY!");
    return 0;
}
"""

FIBONACCI = """
fn fib(n: i32) -> i32 {
    if n <= 1 { return n; }
    return fib(n - 1) + fib(n - 2);
}
"""


def test_e2e_hello_world():
    ir = _ir(HELLO_WORLD)
    assert ir.strip()
    assert "@main" in ir
    assert "declare i32 @puts" in ir
    assert "Hello, JOCKY!" in ir


def test_e2e_fibonacci():
    ir = _ir(FIBONACCI)
    assert "define i32 @fib" in ir
    assert "call i32 @fib" in ir


def test_e2e_while_break():
    src = """
fn f() -> i32 {
    let i: i32 = 0;
    while i < 10 {
        if i == 5 { break; }
        i = i + 1;
    }
    return i;
}
"""
    ir = _ir(src)
    assert "while_exit" in ir
    assert "while_body" in ir


def test_e2e_for_continue():
    src = """
fn f() -> void {
    for (let i: i32 = 0; i < 10; i = i + 1) {
        if i == 3 { continue; }
    }
}
"""
    ir = _ir(src)
    # All allocas must be in the entry block (not inside loop body)
    lines = ir.splitlines()
    alloca_lines = [idx for idx, l in enumerate(lines) if "alloca" in l]
    non_entry = [
        idx for idx, l in enumerate(lines)
        if l.rstrip().endswith(":") and "entry" not in l
    ]
    if alloca_lines and non_entry:
        assert max(alloca_lines) < min(non_entry), "all allocas must be in entry block"
    assert "for_step" in ir


def test_e2e_bitwise_mask():
    src = """
fn f(x: i32) -> void {
    let mask: i32 = 0xFF & (x >> 2);
}
"""
    ir = _ir(src)
    assert "lshr i32" in ir
    assert "and i32" in ir


def test_e2e_null_pointer_compare():
    src = """
fn f() -> void {
    let p: i8* = null;
    if p == null { }
}
"""
    ir = _ir(src)
    assert "store i8* null," in ir
    assert "icmp eq" in ir


def test_e2e_i64_arithmetic():
    src = """
fn f() -> void {
    let a: i64 = 1;
    let b: i64 = 2;
    let c: i64 = a + b;
}
"""
    ir = _ir(src)
    assert "alloca i64" in ir
    assert "add i64" in ir


def test_e2e_variadic_printf():
    src = """
ffi printf(fmt: string, ...) -> i32;
fn main() -> i32 {
    printf("val: %d", 42);
    return 0;
}
"""
    ir = _ir(src)
    assert "declare i32 @printf(i8*, ...)" in ir
    assert "call i32 @printf" in ir


def test_e2e_pointer_ops():
    src = """
ffi malloc(n: i32) -> i8*;
fn main() -> i32 {
    let p: i8* = malloc(16);
    return 0;
}
"""
    ir = _ir(src)
    assert "declare i8* @malloc" in ir
    assert "call i8* @malloc" in ir


def test_e2e_short_circuit_and():
    src = """
fn f(a: bool, b: bool) -> bool {
    return a && b;
}
"""
    ir = _ir(src)
    # Must use phi-based short-circuit, not a single 'and i1'
    assert "phi i1 [ 0," in ir
    assert "and i1" not in ir


def test_e2e_short_circuit_or():
    src = """
fn f(a: bool, b: bool) -> bool {
    return a || b;
}
"""
    ir = _ir(src)
    assert "phi i1 [ 1," in ir
    assert "or i1" not in ir


def test_e2e_else_if():
    src = """
fn classify(x: i32) -> i32 {
    if x < 0 {
        return 0 - 1;
    } else if x == 0 {
        return 0;
    } else {
        return 1;
    }
}
"""
    ir = _ir(src)
    assert "define i32 @classify" in ir
    # There must be multiple conditional branches (else-if creates nested ifs)
    assert ir.count("br i1") >= 2


def test_e2e_nested_loops():
    src = """
fn f() -> void {
    let i: i32 = 0;
    while i < 3 {
        let j: i32 = 0;
        while j < 3 {
            j = j + 1;
        }
        i = i + 1;
    }
}
"""
    ir = _ir(src)
    # Two nested whiles produce two while headers
    assert ir.count("while.") >= 2


def test_e2e_error_propagation():
    """A type error must propagate as JockyTypeError, not an unhandled internal exception."""
    bad_src = """
fn f() -> i32 {
    return true;
}
"""
    with pytest.raises(JockyTypeError):
        _ir(bad_src)


def test_e2e_hex_literal():
    src = """
fn f() -> void {
    let x: i32 = 0xFF;
}
"""
    ir = _ir(src)
    # 0xFF is parsed as integer 255; the codegen emits the decimal value
    assert "255" in ir


def test_e2e_multiple_ffi_dedup():
    """Declaring the same FFI twice (e.g. from a prelude + user code) should only emit one declare."""
    src = """
ffi puts(s: string) -> i32;
ffi puts(s: string) -> i32;
fn main() -> i32 {
    puts("hi");
    return 0;
}
"""
    ir = _ir(src)
    # Should have exactly one declare for puts
    declare_lines = [l for l in ir.splitlines() if "declare" in l and "@puts" in l]
    assert len(declare_lines) == 1


def test_e2e_for_loop_full():
    src = """
ffi puts(s: string) -> i32;
fn main() -> i32 {
    for (let i: i32 = 0; i < 3; i = i + 1) {
        puts("x");
    }
    return 0;
}
"""
    ir = _ir(src)
    assert "for_cond" in ir
    assert "for_body" in ir
    assert "for_step" in ir
    assert "for_exit" in ir
