"""Tests for closure/lambda expressions in JOCKY language."""

import pytest
from src.jocky.language.parser import parse_source
from src.jocky.language.ast import ClosureExpr, CaptureVar, IntLiteral, VarRef


class TestClosureParsing:
    """Test closure syntax parsing."""

    def test_simple_closure(self):
        """Test parsing a simple closure: |x| { x }"""
        code = "let f = |x| { x };"
        program = parse_source(code)

        assert len(program.decls) == 1
        decl = program.decls[0]

        # Check closure is parsed
        assert isinstance(decl.init, ClosureExpr)
        closure = decl.init

        # Should have one parameter
        assert len(closure.params) == 1
        assert closure.params[0].name == "x"

    def test_closure_with_type_annotation(self):
        """Test closure with parameter type: |x: i32| { x }"""
        code = "let f = |x: i32| { x };"
        program = parse_source(code)

        closure = program.decls[0].init
        assert isinstance(closure, ClosureExpr)

        # Check parameter type
        assert closure.params[0].name == "x"
        assert closure.params[0].type.name == "i32"

    def test_closure_with_return_type(self):
        """Test closure with return type annotation: |x| -> i32 { x }"""
        code = "let f = |x| -> i32 { x };"
        program = parse_source(code)

        closure = program.decls[0].init
        assert isinstance(closure, ClosureExpr)

        # Check return type
        assert closure.ret_type is not None
        assert closure.ret_type.name == "i32"

    def test_closure_with_multiple_params(self):
        """Test closure with multiple parameters: |x, y| { x + y }"""
        code = "let add = |x: i32, y: i32| -> i32 { x + y };"
        program = parse_source(code)

        closure = program.decls[0].init
        assert isinstance(closure, ClosureExpr)

        # Check parameters
        assert len(closure.params) == 2
        assert closure.params[0].name == "x"
        assert closure.params[1].name == "y"

    def test_closure_with_capture(self):
        """Test closure with captured variable: let n = 10; let f = |x| { x + n };"""
        code = """
        let n = 10;
        let f = |x| { x + n };
        """
        program = parse_source(code)

        # Get closure from second declaration
        closure = program.decls[1].init
        assert isinstance(closure, ClosureExpr)

        # Check captures (n should be captured)
        assert len(closure.captures) == 1
        assert closure.captures[0].name == "n"
        assert closure.captures[0].by_ref == False

    def test_closure_with_ref_capture(self):
        """Test closure with reference capture: |&x| { x }"""
        code = "let f = |&x| { x };"
        program = parse_source(code)

        closure = program.decls[0].init
        assert isinstance(closure, ClosureExpr)

        # Check reference capture
        assert len(closure.captures) == 1
        assert closure.captures[0].name == "x"
        assert closure.captures[0].by_ref == True

    def test_closure_expression_body(self):
        """Test closure with expression body (no braces): |x| x * 2"""
        code = "let double = |x: i32| x * 2;"
        program = parse_source(code)

        closure = program.decls[0].init
        assert isinstance(closure, ClosureExpr)

        # Body should be an expression, not a block
        from src.jocky.language.ast import BinaryOp
        assert isinstance(closure.body, BinaryOp)

    def test_closure_in_expression(self):
        """Test closure as part of larger expression"""
        code = "let result = (|x| x + 1)(5);"
        program = parse_source(code)

        # The value should be a call expression with a closure
        from src.jocky.language.ast import CallExpr
        call = program.decls[0].init
        assert isinstance(call, CallExpr)
        assert isinstance(call.name, ClosureExpr)  # Wait, this might not work with current structure

    def test_empty_closure_params(self):
        """Test closure with no parameters: || { 42 }"""
        code = "let const_fn = || { 42 };"
        program = parse_source(code)

        closure = program.decls[0].init
        assert isinstance(closure, ClosureExpr)

        # No parameters
        assert len(closure.params) == 0


class TestClosureTypeInference:
    """Test type inference for closures (future feature)."""

    @pytest.mark.skip(reason="Type inference not yet implemented")
    def test_infer_param_type(self):
        """Test inferring parameter type from usage"""
        code = "let f = |x| { x + 1 };"  # x should be inferred as i32
        # This test would verify type checking once implemented

    @pytest.mark.skip(reason="Type inference not yet implemented")
    def test_infer_return_type(self):
        """Test inferring return type from body"""
        code = "let f = |x: i32| { x + 1 };"  # Return type should be inferred as i32
        # This test would verify type checking once implemented


class TestClosureCodegen:
    """Test code generation for closures (future feature)."""

    @pytest.mark.skip(reason="Codegen not yet implemented")
    def test_simple_closure_codegen(self):
        """Test LLVM codegen for simple closure"""
        code = "let f = |x: i32| -> i32 { x * 2 };"
        # This test would verify LLVM IR generation once implemented


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
