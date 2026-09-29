from hypothesis import given, strategies as st, assume, HealthCheck, settings
from jocky.language.lexer import Lexer, LexerError
from jocky.language.parser import Parser, ParseError, parse_source
from jocky.language.ast import Program


class TestParserProperties:
    """Property-based tests for parser robustness."""

    @settings(suppress_health_check=[HealthCheck.too_slow], deadline=None)
    @given(st.text())
    def test_parser_never_crashes(self, code):
        """Parser should never crash on any input."""
        try:
            ast = parse_source(code)
            assert ast is None or isinstance(ast, Program)
        except (LexerError, ParseError):
            pass

    @settings(suppress_health_check=[HealthCheck.too_slow], deadline=None)
    @given(st.text())
    def test_parse_produces_program_or_none(self, code):
        """Parser should always return Program or None."""
        try:
            result = parse_source(code)
            assert result is None or isinstance(result, Program)
        except (LexerError, ParseError):
            pass

    @given(st.just(""))
    def test_empty_program(self, code):
        """Parser should handle empty input gracefully."""
        try:
            ast = parse_source(code)
            if ast is not None:
                assert hasattr(ast, 'decls')
                assert isinstance(ast.decls, list)
        except (LexerError, ParseError):
            pass

    @given(st.just("// comment only"))
    def test_comment_only_program(self, code):
        """Parser should handle comment-only input."""
        try:
            ast = parse_source(code)
            if ast is not None:
                assert hasattr(ast, 'decls')
                assert isinstance(ast.decls, list)
        except (LexerError, ParseError):
            pass

    @given(st.text(alphabet=st.characters(blacklist_categories=('Cc', 'Cs'))))
    def test_printable_text_parsing(self, code):
        """Parser should handle printable text gracefully."""
        assume(len(code) < 500)
        try:
            ast = parse_source(code)
            assert ast is None or isinstance(ast, Program)
        except (LexerError, ParseError):
            pass

    @given(st.lists(st.integers(min_value=0, max_value=255), max_size=300))
    def test_arbitrary_byte_parsing(self, byte_values):
        """Parser should handle arbitrary bytes gracefully."""
        data = bytes(byte_values)
        code = data.decode('utf-8', errors='ignore')
        try:
            ast = parse_source(code)
            assert ast is None or isinstance(ast, Program)
        except (LexerError, ParseError):
            pass

    def test_nested_parens(self):
        """Parser should handle deeply nested parentheses."""
        code = "fn f() -> i32 { let x = " + "(" * 20 + "1" + ")" * 20 + "; return 0; }"
        try:
            ast = parse_source(code)
            if ast is not None:
                assert isinstance(ast, Program)
        except ParseError:
            pass

    def test_token_sequences(self):
        """Parser should handle various token sequences."""
        test_cases = [
            "fn",
            "fn f",
            "fn f(",
            "fn f() ->",
            "fn f() -> i32",
            "fn f() -> i32 {",
            "let let let",
            "+ - * /",
            "[ [ [",
            "{ { {",
        ]
        for code in test_cases:
            try:
                ast = parse_source(code)
                assert ast is None or isinstance(ast, Program)
            except (LexerError, ParseError):
                pass
