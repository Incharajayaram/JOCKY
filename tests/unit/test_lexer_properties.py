from hypothesis import given, strategies as st, assume, HealthCheck, settings
from jocky.language.lexer import Lexer, LexerError, TokenType


class TestLexerProperties:
    """Property-based tests for lexer robustness."""

    @settings(suppress_health_check=[HealthCheck.too_slow], deadline=None)
    @given(st.text())
    def test_lexer_never_crashes(self, code):
        """Lexer should never crash on any input."""
        try:
            lexer = Lexer(code)
            tokens = lexer.tokenize()
            assert isinstance(tokens, list)
        except LexerError:
            pass

    @settings(suppress_health_check=[HealthCheck.too_slow], deadline=None)
    @given(st.text())
    def test_tokenize_produces_valid_list(self, code):
        """All tokens should have required attributes."""
        try:
            lexer = Lexer(code)
            tokens = lexer.tokenize()

            assert len(tokens) > 0, "token list must not be empty"
            assert tokens[-1].type == TokenType.EOF, "last token must be EOF"

            for token in tokens:
                assert hasattr(token, 'type')
                assert hasattr(token, 'value')
                assert hasattr(token, 'line')
                assert hasattr(token, 'column')
                assert token.line >= 1
                assert token.column >= 1
        except LexerError:
            pass

    @given(st.integers(min_value=1, max_value=10))
    def test_empty_input(self, repeat):
        """Lexer should handle empty input."""
        lexer = Lexer("")
        tokens = lexer.tokenize()
        assert len(tokens) == 1
        assert tokens[0].type == TokenType.EOF

    @given(st.just("   \n\t  \r\n  "))
    def test_whitespace_only(self, code):
        """Lexer should handle whitespace-only input."""
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        assert len(tokens) == 1
        assert tokens[0].type == TokenType.EOF

    @given(st.just("// comment\n// another"))
    def test_comments_only(self, code):
        """Lexer should handle comments-only input."""
        lexer = Lexer(code)
        tokens = lexer.tokenize()
        assert len(tokens) == 1
        assert tokens[0].type == TokenType.EOF

    @given(st.lists(st.just("let"), min_size=1, max_size=20))
    def test_repetition_handling(self, keywords):
        """Lexer should handle repeated keywords gracefully."""
        code = " ".join(keywords)
        try:
            lexer = Lexer(code)
            tokens = lexer.tokenize()
            assert len(tokens) > 0
            assert tokens[-1].type == TokenType.EOF
        except LexerError:
            pass

    @given(st.lists(st.sampled_from(["+", "-", "*", "/", "=="]), min_size=1, max_size=20))
    def test_operator_sequences(self, ops):
        """Lexer should handle operator sequences."""
        code = "".join(ops)
        try:
            lexer = Lexer(code)
            tokens = lexer.tokenize()
            assert len(tokens) > 0
            assert tokens[-1].type == TokenType.EOF
        except LexerError:
            pass

    @given(st.text(alphabet=st.characters(blacklist_categories=('Cc', 'Cs'))))
    def test_printable_text(self, code):
        """Lexer should handle printable text."""
        assume(len(code) < 1000)  # Limit size for performance
        try:
            lexer = Lexer(code)
            tokens = lexer.tokenize()
            assert isinstance(tokens, list)
        except LexerError:
            pass

    @given(st.lists(st.integers(min_value=0, max_value=255), max_size=500))
    def test_arbitrary_bytes(self, byte_values):
        """Lexer should handle arbitrary byte sequences gracefully."""
        data = bytes(byte_values)
        code = data.decode('utf-8', errors='ignore')
        try:
            lexer = Lexer(code)
            tokens = lexer.tokenize()
            assert isinstance(tokens, list)
        except LexerError:
            pass

    def test_position_tracking_consistency(self):
        """Line and column tracking should be consistent."""
        code = "let x: i32 = 5;\nlet y: i32 = 10;"
        lexer = Lexer(code)
        tokens = lexer.tokenize()

        for i, token in enumerate(tokens[:-1]):
            assert token.line >= 1, f"token {i}: line {token.line} < 1"
            assert token.column >= 1, f"token {i}: column {token.column} < 1"
            if i > 0:
                prev = tokens[i-1]
                if token.line == prev.line:
                    assert token.column > prev.column, \
                        f"tokens on same line should have increasing columns"
