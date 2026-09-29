import sys
import pathlib

sys.path.insert(0, str(pathlib.Path(__file__).parent.parent.parent / "src"))

from jocky.language.lexer import Lexer, LexerError
from jocky.language.parser import Parser, ParseError
from jocky.language.ast import Program

def fuzz_parser(data: bytes):
    """Fuzz the parser with arbitrary input.

    Tests that the parser handles all valid token sequences gracefully,
    either by producing a valid AST or raising a ParseError. No crashes allowed.
    """
    try:
        code = data.decode('utf-8', errors='ignore')

        try:
            lexer = Lexer(code)
            tokens = lexer.tokenize()
        except LexerError:
            # Lexer rejection is fine, parser should never see invalid tokens
            return

        parser = Parser(tokens)
        ast = parser.parse()

        # Verify invariants
        assert ast is None or isinstance(ast, Program), \
            f"parse() must return Program or None, got {type(ast)}"

        if ast is not None:
            assert hasattr(ast, 'decls'), "Program must have decls attribute"
            assert isinstance(ast.decls, list), "Program.decls must be a list"
            for decl in ast.decls:
                # All declarations should have basic attributes
                assert hasattr(decl, '__class__'), "declaration missing __class__"

    except ParseError:
        # Parser is allowed to reject invalid syntax
        pass
    except UnicodeDecodeError:
        # Decoding errors are acceptable
        pass
    except Exception as e:
        # Any other exception is a bug
        raise AssertionError(f"Unexpected exception in parser: {type(e).__name__}: {e}") from e


if __name__ == '__main__':
    data = sys.stdin.buffer.read()
    fuzz_parser(data)
