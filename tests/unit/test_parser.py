import pytest
from jocky.language.parser import parse_source, ParseError
from jocky.language.ast import (
    Program, FuncDecl, FFIDecl, LetStmt, IfStmt, WhileStmt,
    ForStmt, ReturnStmt, BinaryOp, UnaryOp, CallExpr,
    IntLiteral, StringLiteral, BoolLiteral, VarRef, NullLiteral,
    BreakStmt, ContinueStmt, AssignStmt, ExprStmt,
    DerefExpr, AddrOfExpr, CastExpr, IndexExpr, Block, JType,
)

def test_parse_hello():
    src = '''ffi puts(s: string) -> i32;
fn main() -> i32 {
    puts("Hello!");
    return 0;
}
'''
    prog = parse_source(src)
    assert len(prog.decls) == 2
    assert isinstance(prog.decls[0], FFIDecl)
    assert prog.decls[0].name == "puts"
    assert isinstance(prog.decls[1], FuncDecl)
    assert prog.decls[1].name == "main"

def test_parse_fib():
    src = '''fn fib(n: i32) -> i32 {
    if n <= 1 {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}
'''
    prog = parse_source(src)
    assert len(prog.decls) == 1
    fib = prog.decls[0]
    assert fib.name == "fib"
    assert len(fib.body.stmts) == 2
    assert isinstance(fib.body.stmts[0], IfStmt)
    assert isinstance(fib.body.stmts[1], ReturnStmt)

def test_parse_while():
    src = '''fn main() -> i32 {
    let i: i32 = 0;
    while i < 10 {
        i = i + 1;
    }
    return 0;
}
'''
    prog = parse_source(src)
    fn = prog.decls[0]
    assert isinstance(fn.body.stmts[1], WhileStmt)

def test_parse_for():
    src = '''fn main() -> i32 {
    for (let i: i32 = 0; i < 10; i = i + 1) {
        puts("hi");
    }
    return 0;
}
'''
    prog = parse_source(src)
    fn = prog.decls[0]
    assert isinstance(fn.body.stmts[0], ForStmt)

def test_variadic_ffi():
    src = '''ffi printf(fmt: string, ...) -> i32;
fn main() -> i32 {
    printf("test %d", 42);
    return 0;
}
'''
    prog = parse_source(src)
    assert isinstance(prog.decls[0], FFIDecl)
    assert prog.decls[0].variadic


# --- New statements ---

def _wrap(body: str) -> str:
    return f"fn main() -> i32 {{\n{body}\n    return 0;\n}}"


def test_parse_break():
    prog = parse_source(_wrap("while true { break; }"))
    fn = prog.decls[0]
    while_stmt = fn.body.stmts[0]
    assert isinstance(while_stmt, WhileStmt)
    assert isinstance(while_stmt.body.stmts[0], BreakStmt)


def test_parse_continue():
    prog = parse_source(_wrap("while true { continue; }"))
    fn = prog.decls[0]
    while_stmt = fn.body.stmts[0]
    assert isinstance(while_stmt, WhileStmt)
    assert isinstance(while_stmt.body.stmts[0], ContinueStmt)


def test_parse_else_if():
    src = """fn f(a: bool, b: bool) -> void {
    if a { } else if b { } else { }
}"""
    prog = parse_source(src)
    fn = prog.decls[0]
    if_stmt = fn.body.stmts[0]
    assert isinstance(if_stmt, IfStmt)
    assert if_stmt.else_block is not None
    inner = if_stmt.else_block.stmts[0]
    assert isinstance(inner, IfStmt)
    assert inner.else_block is not None


# --- New expressions ---

def test_parse_null_literal():
    prog = parse_source(_wrap("let p: i8* = null;"))
    fn = prog.decls[0]
    let_stmt = fn.body.stmts[0]
    assert isinstance(let_stmt, LetStmt)
    assert isinstance(let_stmt.init, NullLiteral)


def test_parse_bitwise_ops():
    # a | b ^ c & d   =>  a | (b ^ (c & d))  (& tightest, | loosest)
    prog = parse_source(_wrap("let x: i32 = a | b ^ c & d;"))
    let_stmt = prog.decls[0].body.stmts[0]
    outer = let_stmt.init
    assert isinstance(outer, BinaryOp)
    assert outer.op == "|"
    inner_xor = outer.right
    assert isinstance(inner_xor, BinaryOp)
    assert inner_xor.op == "^"
    inner_and = inner_xor.right
    assert isinstance(inner_and, BinaryOp)
    assert inner_and.op == "&"


def test_parse_shift_ops():
    # x << 2 >> 1  =>  (x << 2) >> 1  (left-assoc)
    prog = parse_source(_wrap("let x: i32 = a << 2 >> 1;"))
    let_stmt = prog.decls[0].body.stmts[0]
    outer = let_stmt.init
    assert isinstance(outer, BinaryOp)
    assert outer.op == ">>"
    assert isinstance(outer.left, BinaryOp)
    assert outer.left.op == "<<"


def test_parse_bitwise_not():
    prog = parse_source(_wrap("let x: i32 = ~a;"))
    let_stmt = prog.decls[0].body.stmts[0]
    expr = let_stmt.init
    assert isinstance(expr, UnaryOp)
    assert expr.op == "~"
    assert isinstance(expr.operand, VarRef)
    assert expr.operand.name == "a"


def test_parse_cast():
    prog = parse_source(_wrap("let x: i64 = (i64)a;"))
    let_stmt = prog.decls[0].body.stmts[0]
    expr = let_stmt.init
    assert isinstance(expr, CastExpr)
    assert expr.target_type == JType("i64")
    assert isinstance(expr.operand, VarRef)


def test_parse_deref():
    prog = parse_source("fn f(ptr: i8*) -> i8 { return *ptr; }")
    fn = prog.decls[0]
    ret = fn.body.stmts[0]
    assert isinstance(ret, ReturnStmt)
    assert isinstance(ret.value, DerefExpr)


def test_parse_addr_of():
    prog = parse_source("fn f(x: i32) -> i32* { return &x; }")
    fn = prog.decls[0]
    ret = fn.body.stmts[0]
    assert isinstance(ret, ReturnStmt)
    assert isinstance(ret.value, AddrOfExpr)
    assert isinstance(ret.value.operand, VarRef)


def test_parse_index():
    prog = parse_source(_wrap("let v: i8 = arr[0];"))
    let_stmt = prog.decls[0].body.stmts[0]
    expr = let_stmt.init
    assert isinstance(expr, IndexExpr)
    assert isinstance(expr.base, VarRef)
    assert isinstance(expr.index, IntLiteral)
    assert expr.index.value == 0


def test_parse_assign_deref():
    prog = parse_source("fn f(ptr: i32*) -> void { *ptr = 5; }")
    fn = prog.decls[0]
    assign = fn.body.stmts[0]
    assert isinstance(assign, AssignStmt)
    assert isinstance(assign.target, DerefExpr)
    assert isinstance(assign.value, IntLiteral)
    assert assign.value.value == 5


def test_parse_assign_index():
    prog = parse_source("fn f(arr: i32*, i: i32, v: i32) -> void { arr[i] = v; }")
    fn = prog.decls[0]
    assign = fn.body.stmts[0]
    assert isinstance(assign, AssignStmt)
    assert isinstance(assign.target, IndexExpr)
    assert isinstance(assign.value, VarRef)


# --- For loop edge cases ---

def test_parse_for_empty_step():
    src = _wrap("for (let i: i32 = 0; i < 10;) { }")
    prog = parse_source(src)
    for_stmt = prog.decls[0].body.stmts[0]
    assert isinstance(for_stmt, ForStmt)
    assert isinstance(for_stmt.step, IntLiteral)
    assert for_stmt.step.value == 0


# --- Operator precedence ---

def test_precedence_add_vs_mul():
    prog = parse_source(_wrap("let x: i32 = a + b * c;"))
    let_stmt = prog.decls[0].body.stmts[0]
    outer = let_stmt.init
    assert isinstance(outer, BinaryOp)
    assert outer.op == "+"
    assert isinstance(outer.right, BinaryOp)
    assert outer.right.op == "*"


def test_precedence_logical_vs_bitwise():
    # a || b | c  =>  a || (b | c)  (|| is lower precedence than |)
    # Parser makes no type distinction; both VarRef nodes are fine
    prog = parse_source("fn f() -> void { let x = a || b | c; }")
    let_stmt = prog.decls[0].body.stmts[0]
    outer = let_stmt.init
    assert isinstance(outer, BinaryOp)
    assert outer.op == "||"
    assert isinstance(outer.right, BinaryOp)
    assert outer.right.op == "|"


# --- Error paths ---

def test_parse_error_missing_semicolon():
    with pytest.raises(ParseError):
        parse_source(_wrap("let x: i32 = 5"))


def test_parse_error_unexpected_token():
    with pytest.raises(ParseError):
        parse_source("fn main { return 0; }")
