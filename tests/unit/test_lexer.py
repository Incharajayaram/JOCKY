import pytest
from jocky.language.lexer import Lexer, TokenType, LexerError

def test_hello_tokens():
    src = '''ffi puts(s: string) -> i32;
fn main() -> i32 {
    puts("Hello!");
    return 0;
}
'''
    lexer = Lexer(src)
    tokens = lexer.tokenize()
    types = [t.type for t in tokens]
    assert TokenType.FFI in types
    assert TokenType.FN in types
    assert TokenType.RETURN in types
    assert TokenType.STRING in types
    assert tokens[-1].type == TokenType.EOF

def test_numbers():
    lexer = Lexer("42 0xFF")
    tokens = lexer.tokenize()
    assert tokens[0].type == TokenType.NUMBER
    assert tokens[0].value == 42
    assert tokens[1].type == TokenType.NUMBER
    assert tokens[1].value == 255

def test_operators():
    lexer = Lexer("+ - * / == != <= >= && || -> ...")
    tokens = lexer.tokenize()
    types = [t.type for t in tokens[:-1]]
    assert types == [
        TokenType.PLUS, TokenType.MINUS, TokenType.STAR, TokenType.SLASH,
        TokenType.EQEQ, TokenType.NEQ, TokenType.LE, TokenType.GE,
        TokenType.ANDAND, TokenType.OROR, TokenType.ARROW, TokenType.ELLIPSIS,
    ]

def test_comments_ignored():
    lexer = Lexer("// this is a comment\n42")
    tokens = lexer.tokenize()
    assert tokens[0].type == TokenType.NUMBER
    assert tokens[0].value == 42


# --- Error paths ---

def test_plusplus_error():
    with pytest.raises(LexerError, match=r"\+\+.*not supported"):
        Lexer("i++").tokenize()


def test_minusminus_error():
    with pytest.raises(LexerError, match=r"--.*not supported"):
        Lexer("i--").tokenize()


def test_unterminated_string():
    with pytest.raises(LexerError, match="Unterminated string"):
        Lexer('"hello').tokenize()


def test_unexpected_char():
    with pytest.raises(LexerError, match="Unexpected character"):
        Lexer("§foo").tokenize()


# --- New keywords ---

def test_break_continue_null_keywords():
    tokens = Lexer("break continue null").tokenize()
    types = [t.type for t in tokens[:-1]]
    assert types == [TokenType.BREAK, TokenType.CONTINUE, TokenType.NULL]


# --- Bitwise operators ---

def test_bitwise_operators():
    tokens = Lexer("| ^ ~ << >>").tokenize()
    types = [t.type for t in tokens[:-1]]
    assert types == [
        TokenType.PIPE, TokenType.CARET, TokenType.TILDE,
        TokenType.LSHIFT, TokenType.RSHIFT,
    ]


# --- Disambiguation ---

def test_ampersand_vs_andand():
    tokens = Lexer("& &&").tokenize()
    types = [t.type for t in tokens[:-1]]
    assert types == [TokenType.AMPERSAND, TokenType.ANDAND]


def test_pipe_vs_oror():
    tokens = Lexer("| ||").tokenize()
    types = [t.type for t in tokens[:-1]]
    assert types == [TokenType.PIPE, TokenType.OROR]


def test_lt_lshift_le():
    tokens = Lexer("< << <=").tokenize()
    types = [t.type for t in tokens[:-1]]
    assert types == [TokenType.LT, TokenType.LSHIFT, TokenType.LE]


def test_gt_rshift_ge():
    tokens = Lexer("> >> >=").tokenize()
    types = [t.type for t in tokens[:-1]]
    assert types == [TokenType.GT, TokenType.RSHIFT, TokenType.GE]


# --- Escape sequences ---

def test_string_escapes():
    tokens = Lexer(r'"\n\t\\\"\0"').tokenize()
    assert tokens[0].type == TokenType.STRING
    val = tokens[0].value
    assert val == "\n\t\\\"\0"


# --- Position tracking ---

def test_line_column():
    src = "42\n99"
    tokens = Lexer(src).tokenize()
    assert tokens[0].line == 1
    assert tokens[0].column == 1
    assert tokens[1].line == 2
    assert tokens[1].column == 1
