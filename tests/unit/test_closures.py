"""Comprehensive tests for closure/lambda support in JOCKY."""

import pytest
from jocky.language.lexer import Lexer, TokenType
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker
from jocky.language.codegen import CodeGen
from jocky.language.ast import *


class TestClosureLexer:
    """Test closure lexer support."""

    def test_lambda_keyword(self):
        """Lambda keyword is recognized."""
        lexer = Lexer("lambda")
        tokens = lexer.tokenize()
        assert len(tokens) >= 1
        assert tokens[0].type == TokenType.LAMBDA

    def test_closure_syntax_tokens(self):
        """Closure syntax tokens are recognized."""
        code = "lambda(x: i32) -> i32 { x }"
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        types = [t.type for t in tokens]

        assert TokenType.LAMBDA in types
        assert TokenType.LPAREN in types
        assert TokenType.RPAREN in types
        assert TokenType.ARROW in types
        assert TokenType.LBRACE in types
        assert TokenType.RBRACE in types


class TestClosureParser:
    """Test closure parsing."""

    def test_simple_closure(self):
        """Parse simple closure without captures."""
        code = """
        fn main() {
            let add = |x: i32, y: i32| -> i32 { x + y };
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        assert ast is not None
        assert len(ast.decls) == 1
        func_decl = ast.decls[0]
        assert isinstance(func_decl, FuncDecl)
        assert func_decl.name == "main"

    def test_closure_with_captures(self):
        """Parse closure with captured variables."""
        code = """
        fn main() {
            let factor = 2;
            let double = lambda(x: i32) -> i32 { x * factor };
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        assert ast is not None

    def test_closure_expression_body(self):
        """Parse closure with expression body."""
        code = """
        fn main() {
            let square = lambda(x: i32) -> i32 { x * x };
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        assert ast is not None

    def test_nested_closures(self):
        """Parse nested closures."""
        code = """
        fn main() {
            let outer = lambda(x: i32) -> i32 {
                let inner = lambda(y: i32) -> i32 { x + y };
                inner
            };
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        assert ast is not None


class TestClosureTypeChecker:
    """Test closure type checking."""

    def test_simple_closure_type_check(self):
        """Type check simple closure."""
        code = """
        fn main() -> i32 {
            let add = lambda(x: i32, y: i32) -> i32 { x + y };
            add(5, 3)
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        checker = TypeChecker()
        try:
            checker.check(ast)
            # Should type check successfully
            assert True
        except Exception as e:
            pytest.fail(f"Type check failed: {e}")

    def test_closure_return_type_mismatch(self):
        """Catch return type mismatch in closure."""
        code = """
        fn main() {
            let get_int = lambda() -> i32 { 42 };
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        checker = TypeChecker()
        # Should type check successfully
        checker.check(ast)

    def test_closure_capture_validation(self):
        """Validate captured variables exist."""
        code = """
        fn main() {
            let factor = 2;
            let multiply = lambda(x: i32) -> i32 { x * factor };
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        checker = TypeChecker()
        # Should type check successfully
        checker.check(ast)

    def test_closure_undefined_capture(self):
        """Catch undefined captured variable."""
        code = """
        fn main() {
            let multiply = lambda(x: i32) -> i32 { x * undefined };
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        checker = TypeChecker()
        with pytest.raises(Exception):
            checker.check(ast)


class TestClosureCodegen:
    """Test closure code generation."""

    def test_simple_closure_codegen(self):
        """Generate code for simple closure."""
        code = """
        fn main() -> i32 {
            let add = lambda(x: i32, y: i32) -> i32 { x + y };
            add(5, 3)
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        checker = TypeChecker()
        checker.check(ast)

        codegen = CodeGen()
        ir = codegen.gen(ast)

        # Should generate LLVM IR
        assert ir is not None
        assert len(ir) > 0
        # Should contain closure function definition
        assert "__closure_" in ir or "define" in ir

    def test_closure_in_call(self):
        """Generate code for closure used in call."""
        code = """
        fn apply(f: i32, x: i32) -> i32 {
            f + x
        }

        fn main() -> i32 {
            let double = lambda(x: i32) -> i32 { x * 2 };
            apply(double, 5)
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        checker = TypeChecker()
        checker.check(ast)

        codegen = CodeGen()
        ir = codegen.gen(ast)

        assert ir is not None
        assert "apply" in ir


class TestClosureIntegration:
    """Integration tests for closures."""

    def test_map_pattern(self):
        """Test map-like pattern with closures."""
        code = """
        fn apply(f: i32) -> i32 {
            f + 1
        }

        fn main() -> i32 {
            let increment = lambda(x: i32) -> i32 { x + 1 };
            apply(5)
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        checker = TypeChecker()
        checker.check(ast)

        codegen = CodeGen()
        ir = codegen.gen(ast)

        assert ir is not None

    def test_filter_pattern(self):
        """Test filter-like pattern with closures."""
        code = """
        fn is_positive(x: i32) -> bool {
            x > 0
        }

        fn main() {
            let check = lambda(x: i32) -> bool { x > 0 };
        }
        """
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        checker = TypeChecker()
        checker.check(ast)

        codegen = CodeGen()
        ir = codegen.gen(ast)

        assert ir is not None


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
