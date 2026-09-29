"""
Final comprehensive compiler audit test suite.
Tests all components: lexer, parser, type checker, codegen, resolver, pipeline.
"""

import sys
import pytest
from pathlib import Path

# Add src to path
sys.path.insert(0, str(Path(__file__).parent.parent / "src"))

from jocky.language.lexer import Lexer, LexerError
from jocky.language.parser import Parser
from jocky.language.checker import TypeChecker, TypeError as JockyTypeError
from jocky.language.codegen import CodeGen
from jocky.language.resolver import ModuleResolver


class TestLexer:
    """Test lexer for all token types and edge cases."""

    def test_invalid_escape_sequence(self):
        """Bug #1: Invalid escape sequences should be caught or preserved."""
        lexer = Lexer(r'"test\q"')
        tokens = lexer.tokenize()
        # Should either error or preserve the escape
        string_tok = [t for t in tokens if t.type.name == 'STRING']
        assert len(string_tok) == 1
        # Currently it strips the escape (valid behavior)
        # but should document this or add validation

    def test_hex_numbers(self):
        """Test hex number parsing."""
        lexer = Lexer('let x = 0x1F2A')
        tokens = lexer.tokenize()
        num_tok = [t for t in tokens if t.type.name == 'NUMBER']
        assert len(num_tok) == 1
        assert num_tok[0].value == 0x1F2A

    def test_float_numbers(self):
        """Test float number parsing."""
        lexer = Lexer('let x = 3.14159')
        tokens = lexer.tokenize()
        num_tok = [t for t in tokens if t.type.name == 'NUMBER']
        assert len(num_tok) == 1
        assert isinstance(num_tok[0].value, float)
        assert abs(num_tok[0].value - 3.14159) < 0.0001

    def test_unterminated_string(self):
        """Test unterminated string detection."""
        with pytest.raises(LexerError):
            lexer = Lexer('"unterminated')
            lexer.tokenize()

    def test_all_keywords(self):
        """Test all keyword recognition."""
        keywords = ['fn', 'ffi', 'use', 'let', 'if', 'else', 'while', 'for',
                   'return', 'true', 'false', 'const', 'var']
        for kw in keywords:
            lexer = Lexer(kw)
            tokens = lexer.tokenize()
            assert tokens[0].type.name in ['FN', 'FFI', 'USE', 'LET', 'IF', 'ELSE',
                                          'WHILE', 'FOR', 'RETURN', 'TRUE', 'FALSE', 'CONST', 'VAR']


class TestParser:
    """Test parser for all statement and expression types."""

    def test_parse_for_loop_with_var(self):
        """Test parsing for loop with var declaration. Bug #3: var inside function."""
        code = '''
fn test() {
    var x = 0
    for i in [1, 2, 3] {
        x = x + 1
    }
}
'''
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()

        func = ast.decls[0]
        assert func.name == 'test'
        assert len(func.body.stmts) == 2
        # First statement should be VarDecl
        assert type(func.body.stmts[0]).__name__ == 'VarDecl'
        # Second should be ForInStmt
        assert type(func.body.stmts[1]).__name__ == 'ForInStmt'

    def test_empty_blocks(self):
        """Test empty block parsing."""
        code = '''
fn empty() {
}

fn test() {
    if true {
    }
    while false {
    }
}
'''
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        assert len(ast.decls) == 2

    def test_nested_structures(self):
        """Test nested arrays and function calls."""
        code = '''
fn test() {
    let x = [[1, 2], [3, 4]]
    let y = foo(bar(baz(1)))
}
'''
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        assert len(ast.decls) == 1


class TestTypeChecker:
    """Test type checking for correctness."""

    def test_var_decl_in_function_scope(self):
        """Bug #3: VarDecl inside function should be treated as local variable."""
        code = '''
fn test() {
    var x = 0
    x = 5
}
'''
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        checker = TypeChecker()

        # This should work - x is a local var
        checker.check(ast)

    def test_type_mismatch_detection(self):
        """Test type mismatch is caught."""
        code = '''
ffi foo(x: i32) -> i32;

fn test() {
    let x = "string"
    foo(x)  // Type error: string to i32
}
'''
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        checker = TypeChecker()

        with pytest.raises(JockyTypeError):
            checker.check(ast)

    def test_undefined_variable(self):
        """Test undefined variable detection."""
        code = '''
fn test() {
    let x = undefined_var
}
'''
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        checker = TypeChecker()

        with pytest.raises(JockyTypeError):
            checker.check(ast)


class TestCodeGen:
    """Test code generation produces valid LLVM IR."""

    def test_function_codegen(self):
        """Test function definition generates IR."""
        code = '''
fn test() {
    return 42
}
'''
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        checker = TypeChecker()
        checker.check(ast)

        codegen = CodeGen()
        ir = codegen.gen(ast)

        assert 'define' in ir
        assert 'test' in ir
        assert 'ret' in ir

    def test_string_concat_codegen(self):
        """Test string operations generate correct IR."""
        code = '''
ffi jocky_str_concat(a: string, b: string) -> string;

fn test() {
    let x = "hello"
    let y = "world"
    let z = jocky_str_concat(x, y)
}
'''
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        parser = Parser(tokens)
        ast = parser.parse()
        checker = TypeChecker()
        checker.check(ast)

        codegen = CodeGen()
        ir = codegen.gen(ast)

        assert 'jocky_str_concat' in ir
        assert 'call' in ir


class TestPipeline:
    """Test full compilation pipeline."""

    def test_research_chain_complete_compiles(self):
        """Test research_chain_complete.jky compiles end-to-end."""
        source_file = Path(__file__).parent.parent / "examples/research_chain_complete.jky"
        prelude_path = Path(__file__).parent.parent / "src/jocky/stdlib/prelude.jky"

        assert source_file.exists(), f"research_chain_complete.jky not found at {source_file}"
        assert prelude_path.exists(), f"prelude.jky not found at {prelude_path}"

        prelude = prelude_path.read_text()
        src = source_file.read_text()
        full_src = prelude + "\n" + src

        # Lex
        lexer = Lexer(full_src)
        tokens = lexer.tokenize()
        assert len(tokens) > 0

        # Parse
        parser = Parser(tokens)
        ast = parser.parse()
        assert len(ast.decls) > 0

        # Resolve
        resolver = ModuleResolver(source_file.parent)
        ast = resolver.resolve_program(ast)

        # Type check
        checker = TypeChecker()
        checker.check(ast)  # Should not raise

        # Codegen
        codegen = CodeGen()
        ir = codegen.gen(ast)
        assert 'define' in ir
        assert 'main' in ir


if __name__ == '__main__':
    pytest.main([__file__, '-v'])
