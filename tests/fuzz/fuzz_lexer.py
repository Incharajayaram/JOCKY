import sys
import pathlib

sys.path.insert(0, str(pathlib.Path(__file__).parent.parent.parent / "src"))

from jocky.language.lexer import Lexer, LexerError

def fuzz_lexer(data: bytes):
    """Fuzz the lexer with arbitrary input.

    Tests that the lexer handles all inputs gracefully, either by producing
    a valid token list or raising a LexerError. No crashes or hangs allowed.
    """
    try:
        code = data.decode('utf-8', errors='ignore')
        lexer = Lexer(code)
        tokens = lexer.tokenize()

        # Verify invariants
        assert isinstance(tokens, list), "tokenize() must return a list"
        assert len(tokens) > 0, "token list must not be empty"
        assert tokens[-1].type.name == "EOF", "last token must be EOF"

        for token in tokens:
            assert hasattr(token, 'type'), f"token {token} missing type attribute"
            assert hasattr(token, 'value'), f"token {token} missing value attribute"
            assert hasattr(token, 'line'), f"token {token} missing line attribute"
            assert hasattr(token, 'column'), f"token {token} missing column attribute"
            assert isinstance(token.line, int), f"token line must be int, got {type(token.line)}"
            assert isinstance(token.column, int), f"token column must be int, got {type(token.column)}"
            assert token.line >= 1, f"line numbers must be >= 1, got {token.line}"
            assert token.column >= 1, f"column numbers must be >= 1, got {token.column}"

    except LexerError:
        # Lexer is allowed to reject invalid input
        pass
    except UnicodeDecodeError:
        # Decoding errors are acceptable
        pass
    except Exception as e:
        # Any other exception is a bug
        raise AssertionError(f"Unexpected exception in lexer: {type(e).__name__}: {e}") from e


if __name__ == '__main__':
    data = sys.stdin.buffer.read()
    fuzz_lexer(data)
