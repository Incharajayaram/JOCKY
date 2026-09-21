import pytest
from jocky.language.lexer import Lexer, TokenType

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
