import pytest
from jocky.language.parser import parse_source
from jocky.language.ast import (
    Program, FuncDecl, FFIDecl, LetStmt, IfStmt, WhileStmt,
    ForStmt, ReturnStmt, BinaryOp, CallExpr, IntLiteral, StringLiteral,
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
