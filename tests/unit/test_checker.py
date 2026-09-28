import pytest
import sys
import pathlib

sys.path.insert(0, str(pathlib.Path(__file__).parent.parent.parent / "src"))

from jocky.language.parser import parse_source
from jocky.language.checker import TypeChecker
from jocky.language.checker import TypeError as JockyTypeError


def check(source: str):
    ast = parse_source(source)
    tc = TypeChecker()
    tc.check(ast)


def _fn(body: str, params: str = "", ret: str = "void") -> str:
    return f"fn f({params}) -> {ret} {{\n{body}\n}}"


# ============================================================
# Happy path
# ============================================================

def test_check_hello_world():
    check("""
ffi puts(s: string) -> i32;
fn main() -> i32 {
    puts("Hello!");
    return 0;
}
""")


def test_check_fibonacci():
    check("""
fn fib(n: i32) -> i32 {
    if n <= 1 { return n; }
    return fib(n - 1) + fib(n - 2);
}
""")


def test_check_i64_literal():
    check(_fn("let x: i64 = 100;"))


def test_check_i8_literal():
    check(_fn("let b: i8 = 42;"))


def test_check_pointer_types():
    check(_fn("let p: i8* = null; let q: i32* = null;"))


def test_check_null_compat_with_any_pointer():
    # null (typed as i8*) must be compatible with i64* via types_equal rule
    check(_fn("let p: i64* = null;"))


def test_check_bitwise_ops():
    check(_fn(
        "let a: i32 = 0; let b: i32 = 0;\n"
        "let r1: i32 = a | b;\n"
        "let r2: i32 = a ^ b;\n"
        "let r3: i32 = a & b;\n"
        "let r4: i32 = a << b;\n"
        "let r5: i32 = a >> b;\n"
    ))


def test_check_bitwise_not():
    check(_fn("let x: i32 = 0; let r: i32 = ~x;"))


def test_check_break_in_while():
    check(_fn("while true { break; }"))


def test_check_continue_in_while():
    check(_fn("while true { continue; }"))


def test_check_break_in_for():
    check(_fn("for (let i: i32 = 0; i < 10; i = i + 1) { break; }"))


def test_check_nested_loop_break():
    check(_fn("""
while true {
    while true {
        break;
    }
    break;
}
"""))


def test_check_else_if():
    check(_fn(
        "let a: bool = true; let b: bool = false;\n"
        "if a { } else if b { } else { }",
    ))


def test_check_short_circuit():
    check(_fn("let a: bool = true; let b: bool = false; let c: bool = a && b;"))


def test_check_for_loop_assign_step():
    # Previously crashed because step was AssignStmt wrapped in ExprStmt
    check(_fn("for (let i: i32 = 0; i < 10; i = i + 1) { }"))


def test_check_void_return():
    check(_fn("return;"))


def test_check_variadic_call():
    check("""
ffi printf(fmt: string, ...) -> i32;
fn main() -> i32 {
    printf("val: %d", 42);
    return 0;
}
""")


def test_check_string_type():
    check("""
ffi puts(s: string) -> i32;
fn f() -> void {
    puts("hello");
}
""")


def test_check_cast_expr():
    check(_fn("let x: i32 = 0; let y: i64 = (i64)x;"))


def test_check_deref_pointer():
    check("fn f(p: i32*) -> i32 { return *p; }")


def test_check_addr_of():
    check("fn f(x: i32) -> i32* { return &x; }")


def test_check_index_expr():
    check("fn f(arr: i8*, i: i32) -> i8 { return arr[i]; }")


# ============================================================
# Type errors
# ============================================================

def test_check_error_type_mismatch_let():
    with pytest.raises(JockyTypeError, match="Type mismatch"):
        check(_fn("let x: i32 = true;"))


def test_check_error_return_type_mismatch():
    with pytest.raises(JockyTypeError, match="Return type mismatch"):
        check("fn f() -> bool { return 1; }")


def test_check_error_arithmetic_on_bool():
    with pytest.raises(JockyTypeError, match="Arithmetic op requires numeric"):
        check(_fn("let a: bool = true; let b: bool = false; let c = a + b;"))


def test_check_error_comparison_type_mismatch():
    with pytest.raises(JockyTypeError, match="Comparison requires matching"):
        check(_fn("let x: i32 = 0; let r: bool = x == true;"))


def test_check_error_logical_on_int():
    with pytest.raises(JockyTypeError, match="Logical op requires bool"):
        check(_fn("let x: i32 = 1; let y: i32 = 2; let r: bool = x && y;"))


def test_check_error_bitwise_on_bool():
    with pytest.raises(JockyTypeError, match="Bitwise op requires integer"):
        check(_fn("let a: bool = true; let b: bool = false; let r = a | b;"))


def test_check_error_shift_on_bool():
    with pytest.raises(JockyTypeError, match="Shift op requires integer"):
        check(_fn("let a: bool = true; let b: bool = false; let r = a << b;"))


def test_check_error_bitwise_not_on_bool():
    with pytest.raises(JockyTypeError, match="Bitwise ~ requires integer"):
        check(_fn("let a: bool = true; let r = ~a;"))


def test_check_error_break_outside_loop():
    with pytest.raises(JockyTypeError, match="'break' outside loop"):
        check(_fn("break;"))


def test_check_error_continue_outside_loop():
    with pytest.raises(JockyTypeError, match="'continue' outside loop"):
        check(_fn("continue;"))


def test_check_error_undefined_var():
    with pytest.raises(JockyTypeError, match="Undefined variable"):
        check(_fn("let x = y;"))


def test_check_error_undefined_func():
    with pytest.raises(JockyTypeError, match="Undefined function"):
        check(_fn("foo();"))


def test_check_error_too_few_args():
    with pytest.raises(JockyTypeError, match="expects at least"):
        check("""
ffi puts(s: string) -> i32;
fn main() -> i32 { puts(); return 0; }
""")


def test_check_error_deref_non_pointer():
    with pytest.raises(JockyTypeError, match="Cannot dereference non-pointer"):
        check("fn f(x: i32) -> i32 { return *x; }")


def test_check_error_index_non_pointer():
    with pytest.raises(JockyTypeError, match="Cannot index type"):
        check("fn f(x: i32) -> i32 { return x[0]; }")


def test_check_error_index_non_numeric():
    with pytest.raises(JockyTypeError, match="Index must be numeric"):
        check("fn f(arr: i8*, flag: bool) -> i8 { return arr[flag]; }")


def test_check_error_addr_of_non_var():
    with pytest.raises(JockyTypeError, match="Can only take address of variables"):
        check("fn f(x: i32) -> i32* { return &(x + 1); }")


def test_check_error_void_return_with_value():
    with pytest.raises(JockyTypeError, match="Return type mismatch"):
        check("fn f() -> void { return 5; }")


def test_check_error_nonvoid_return_without_value():
    with pytest.raises(JockyTypeError, match="Return value required"):
        check("fn f() -> i32 { return; }")


def test_check_error_if_condition_not_bool():
    with pytest.raises(JockyTypeError, match="If condition must be bool"):
        check(_fn("if 1 { }"))


def test_check_error_while_condition_not_bool():
    with pytest.raises(JockyTypeError, match="While condition must be bool"):
        check(_fn("while 1 { }"))


def test_check_error_for_condition_not_bool():
    with pytest.raises(JockyTypeError, match="For condition must be bool"):
        check(_fn("for (let i: i32 = 0; 1; i = i + 1) { }"))


# ============================================================
# Builtin functions
# ============================================================

def test_check_sizeof_builtin():
    check(_fn("let x: i32 = 0; let size: i32 = sizeof(x);"))


def test_check_nameof_builtin():
    check(_fn("let x: i32 = 0; let name: string = nameof(x);"))


def test_check_sizeof_wrong_arg_count():
    with pytest.raises(JockyTypeError, match="sizeof expects 1 argument"):
        check(_fn("let x: i32 = sizeof();"))


def test_check_nameof_wrong_arg_count():
    with pytest.raises(JockyTypeError, match="nameof expects 1 argument"):
        check(_fn("let x: string = nameof(1, 2);"))


def test_check_nameof_requires_var():
    with pytest.raises(JockyTypeError, match="nameof requires a variable"):
        check(_fn("let x: string = nameof(1 + 2);"))
