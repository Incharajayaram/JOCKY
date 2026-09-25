import pytest
from jocky.language.parser import parse_source, ParseError
from jocky.language.checker import TypeChecker, TypeError
from jocky.language.codegen import CodeGen
from jocky.language.ast import (
    MatchExpr, MatchArm, Pattern, WildcardPattern, LiteralPattern,
    VariantPattern, IntLiteral, BoolLiteral, VarRef, Block
)


# ==================== PARSER TESTS ====================

def test_parse_match_literal_numbers():
    """Test parsing match with literal number patterns."""
    src = """fn test() -> i32 {
    let x: i32 = 5;
    match x {
        0 => 10,
        1 => 20,
        _ => 99
    }
    return 0;
}
"""
    prog = parse_source(src)
    fn = prog.decls[0]
    match_expr = fn.body.stmts[1].expr
    assert isinstance(match_expr, MatchExpr)
    assert len(match_expr.arms) == 3
    assert isinstance(match_expr.arms[0].pattern, LiteralPattern)
    assert isinstance(match_expr.arms[2].pattern, WildcardPattern)


def test_parse_match_bool_literals():
    """Test parsing match with boolean patterns."""
    src = """fn test() -> i32 {
    let b: bool = true;
    match b {
        true => 1,
        false => 0
    }
    return 0;
}
"""
    prog = parse_source(src)
    fn = prog.decls[0]
    match_expr = fn.body.stmts[1].expr
    assert len(match_expr.arms) == 2
    assert isinstance(match_expr.arms[0].pattern, LiteralPattern)
    assert isinstance(match_expr.arms[0].pattern.value, BoolLiteral)
    assert match_expr.arms[0].pattern.value.value == True


def test_parse_match_wildcard():
    """Test parsing wildcard pattern."""
    src = """fn test() -> i32 {
    let x: i32 = 10;
    match x {
        _ => 42
    }
    return 0;
}
"""
    prog = parse_source(src)
    fn = prog.decls[0]
    match_expr = fn.body.stmts[1].expr
    assert isinstance(match_expr.arms[0].pattern, WildcardPattern)


def test_parse_match_enum_variants():
    """Test parsing match with enum variant patterns."""
    src = """enum Status { OK = 0, Error = 1 };
fn test() -> i32 {
    let s: Status = OK;
    match s {
        OK => 0,
        Error => 1
    }
    return 0;
}
"""
    prog = parse_source(src)
    fn = prog.decls[1]
    match_expr = fn.body.stmts[1].expr
    assert len(match_expr.arms) == 2
    assert isinstance(match_expr.arms[0].pattern, VariantPattern)
    assert match_expr.arms[0].pattern.name == "OK"
    assert isinstance(match_expr.arms[1].pattern, VariantPattern)
    assert match_expr.arms[1].pattern.name == "Error"


def test_parse_match_tagged_union():
    """Test parsing match with tagged union field bindings."""
    src = """enum Result { Ok(x: i32), Error(code: i32) };
fn test() -> i32 {
    let r: Result = Ok;
    match r {
        Ok(val) => val,
        Error(code) => code
    }
    return 0;
}
"""
    prog = parse_source(src)
    fn = prog.decls[1]
    match_expr = fn.body.stmts[1].expr
    assert isinstance(match_expr.arms[0].pattern, VariantPattern)
    assert match_expr.arms[0].pattern.bindings == ["val"]
    assert isinstance(match_expr.arms[1].pattern, VariantPattern)
    assert match_expr.arms[1].pattern.bindings == ["code"]


def test_parse_match_multiple_arms():
    """Test parsing match with multiple arms and various patterns."""
    src = """fn test() -> i32 {
    let x: i32 = 5;
    match x {
        1 => 10,
        2 => 20,
        3 => 30,
        4 => 40,
        _ => 99
    }
    return 0;
}
"""
    prog = parse_source(src)
    fn = prog.decls[0]
    match_expr = fn.body.stmts[1].expr
    assert len(match_expr.arms) == 5


def test_parse_match_block_body():
    """Test parsing match arms with expression bodies."""
    src = """fn test() -> i32 {
    let x: i32 = 5;
    match x {
        0 => 99,
        _ => 100
    }
    return 0;
}
"""
    prog = parse_source(src)
    fn = prog.decls[0]
    match_expr = fn.body.stmts[1].expr
    assert isinstance(match_expr, MatchExpr)
    assert len(match_expr.arms) == 2


# ==================== TYPE CHECKER TESTS ====================

def test_check_match_literal_type_safety():
    """Test type checking for literal patterns."""
    src = """fn test() -> i32 {
    let x: i32 = 5;
    match x {
        0 => 10,
        _ => 20
    }
    return 0;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    # Should not raise TypeError


def test_check_match_bool_patterns():
    """Test type checking for boolean patterns."""
    src = """fn test() -> i32 {
    let b: bool = true;
    match b {
        true => 1,
        false => 0
    }
    return 0;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    # Should not raise TypeError


def test_check_match_enum_variants():
    """Test type checking for enum variant patterns."""
    src = """enum Status { OK = 0, Error = 1 };
fn test() -> i32 {
    let s: Status = OK;
    match s {
        OK => 100,
        Error => 200
    }
    return 0;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    # Should not raise TypeError


def test_check_match_uniform_arm_types():
    """Test that all match arms return same type."""
    src = """fn test() -> i32 {
    let x: i32 = 5;
    let result: i32 = match x {
        0 => 10,
        1 => 20,
        _ => 30
    };
    return result;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    # Should not raise TypeError


def test_check_match_type_mismatch_patterns():
    """Test type checker catches pattern-scrutinee type mismatch."""
    src = """fn test() -> i32 {
    let x: i32 = 5;
    match x {
        true => 10,
        false => 20
    }
    return 0;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    with pytest.raises(TypeError):
        checker.check(prog)


def test_check_match_pattern_binding_types():
    """Test pattern bindings get correct types from tagged unions."""
    src = """enum Result { Ok(x: i32), Error };
fn test() -> i32 {
    let r: Result = Ok;
    match r {
        Ok(val) => val,
        Error => 0
    }
    return 0;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    # Should not raise TypeError


# ==================== CODEGEN TESTS ====================

def test_codegen_match_literals():
    """Test codegen generates switch instruction for literal patterns."""
    src = """fn test() -> i32 {
    let x: i32 = 5;
    match x {
        0 => 10,
        1 => 20,
        _ => 99
    }
    return 0;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    codegen = CodeGen()
    ir = codegen.gen(prog)

    # Check IR contains switch instruction
    assert "switch" in ir
    assert "label %" in ir


def test_codegen_match_bool():
    """Test codegen for boolean pattern matching."""
    src = """fn test() -> i32 {
    let b: bool = true;
    match b {
        true => 1,
        false => 0
    }
    return 0;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    codegen = CodeGen()
    ir = codegen.gen(prog)

    # Check IR contains switch instruction
    assert "switch i1" in ir or "switch i32" in ir


def test_codegen_match_enum_dispatch():
    """Test codegen for enum variant matching."""
    src = """enum Status { OK = 0, Error = 1 };
fn test() -> i32 {
    let s: Status = OK;
    match s {
        OK => 100,
        Error => 200
    }
    return 0;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    codegen = CodeGen()
    ir = codegen.gen(prog)

    # Check IR contains switch instruction for enum dispatch
    assert "switch" in ir
    assert "label %" in ir


def test_codegen_match_result_value():
    """Test codegen correctly returns match expression value."""
    src = """fn get_value(x: i32) -> i32 {
    match x {
        0 => 10,
        1 => 20,
        _ => 30
    }
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    codegen = CodeGen()
    ir = codegen.gen(prog)

    # IR should be valid LLVM
    assert "define" in ir
    assert "switch" in ir


# ==================== END-TO-END TESTS ====================

def test_match_full_pipeline_literal():
    """Full pipeline test: parse -> check -> codegen for literal patterns."""
    src = """fn classify(x: i32) -> i32 {
    match x {
        0 => 0,
        1 => 1,
        2 => 2,
        _ => 99
    }
}

fn main() -> i32 {
    let result: i32 = classify(1);
    return result;
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    codegen = CodeGen()
    ir = codegen.gen(prog)

    # Verify IR is generated without errors
    assert len(ir) > 0
    assert "switch" in ir


def test_match_full_pipeline_enum():
    """Full pipeline test: parse -> check -> codegen for enum patterns."""
    src = """enum Color { Red = 0, Green = 1, Blue = 2 };

fn describe(c: Color) -> i32 {
    match c {
        Red => 1,
        Green => 2,
        Blue => 3
    }
}

fn main() -> i32 {
    let col: Color = Red;
    return describe(col);
}
"""
    prog = parse_source(src)
    checker = TypeChecker()
    checker.check(prog)
    codegen = CodeGen()
    ir = codegen.gen(prog)

    # Verify IR is generated without errors
    assert len(ir) > 0
    assert "switch" in ir


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
