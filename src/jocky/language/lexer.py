from dataclasses import dataclass
from typing import Optional, Any
from enum import Enum, auto

class TokenType(Enum):
    # Literals
    NUMBER = auto()
    STRING = auto()
    IDENT = auto()

    # Keywords
    FN = auto()
    FFI = auto()
    USE = auto()
    LET = auto()
    IF = auto()
    ELSE = auto()
    WHILE = auto()
    FOR = auto()
    RETURN = auto()
    TRUE = auto()
    FALSE = auto()
    BREAK = auto()
    CONTINUE = auto()
    NULL = auto()
    STRUCT = auto()
    ENUM = auto()
    TYPE = auto()
    MATCH = auto()
    MOD = auto()
    ARROW_FAT = auto()  # =>

    # Types
    I8 = auto()
    I32 = auto()
    I64 = auto()
    BOOL = auto()
    VOID = auto()
    STRING_KW = auto()

    # Operators
    PLUS = auto()
    MINUS = auto()
    STAR = auto()
    SLASH = auto()
    PERCENT = auto()
    EQ = auto()
    EQEQ = auto()
    NEQ = auto()
    LT = auto()
    GT = auto()
    LE = auto()
    GE = auto()
    ANDAND = auto()
    OROR = auto()
    BANG = auto()
    AMPERSAND = auto()
    ARROW = auto()
    ELLIPSIS = auto()
    # Bitwise
    PIPE = auto()
    CARET = auto()
    TILDE = auto()
    LSHIFT = auto()
    RSHIFT = auto()

    # Delimiters
    LPAREN = auto()
    RPAREN = auto()
    LBRACE = auto()
    RBRACE = auto()
    LBRACKET = auto()
    RBRACKET = auto()
    COLON = auto()
    SEMICOLON = auto()
    COMMA = auto()
    DOT = auto()
    COLONCOLON = auto()  # ::
    HASH = auto()  # #
    AT = auto()  # @

    EOF = auto()

KEYWORDS = {
    "fn": TokenType.FN,
    "ffi": TokenType.FFI,
    "use": TokenType.USE,
    "let": TokenType.LET,
    "if": TokenType.IF,
    "else": TokenType.ELSE,
    "while": TokenType.WHILE,
    "for": TokenType.FOR,
    "return": TokenType.RETURN,
    "true": TokenType.TRUE,
    "false": TokenType.FALSE,
    "break": TokenType.BREAK,
    "continue": TokenType.CONTINUE,
    "null": TokenType.NULL,
    "struct": TokenType.STRUCT,
    "enum": TokenType.ENUM,
    "type": TokenType.TYPE,
    "match": TokenType.MATCH,
    "mod": TokenType.MOD,
    "use": TokenType.USE,
    "i8": TokenType.I8,
    "i32": TokenType.I32,
    "i64": TokenType.I64,
    "bool": TokenType.BOOL,
    "void": TokenType.VOID,
    "string": TokenType.STRING_KW,
}

@dataclass
class Token:
    type: TokenType
    value: Any
    line: int
    column: int

class LexerError(Exception):
    pass

class Lexer:
    def __init__(self, source: str):
        self.source = source
        self.pos = 0
        self.line = 1
        self.column = 1
        self.tokens: list[Token] = []

    def error(self, msg: str):
        raise LexerError(f"{msg} at line {self.line}, column {self.column}")

    def peek(self, offset: int = 0) -> str:
        p = self.pos + offset
        if p >= len(self.source):
            return "\0"
        return self.source[p]

    def advance(self) -> str:
        ch = self.peek()
        self.pos += 1
        if ch == "\n":
            self.line += 1
            self.column = 1
        else:
            self.column += 1
        return ch

    def skip_whitespace(self):
        while self.peek() in " \t\r\n":
            self.advance()

    def skip_comment(self):
        if self.peek() == "/" and self.peek(1) == "/":
            while self.peek() not in "\n\0":
                self.advance()

    def read_string(self) -> str:
        start_line, start_col = self.line, self.column
        self.advance()  # consume opening "
        value = ""
        while self.peek() != '"':
            if self.peek() == "\0":
                self.error("Unterminated string")
            if self.peek() == "\\":
                self.advance()
                esc = self.advance()
                if esc == "n":
                    value += "\n"
                elif esc == "t":
                    value += "\t"
                elif esc == "\\":
                    value += "\\"
                elif esc == '"':
                    value += '"'
                elif esc == "0":
                    value += "\0"
                else:
                    value += esc
            else:
                value += self.advance()
        self.advance()  # consume closing "
        return value

    def read_number(self) -> int:
        value = ""
        # Support hex
        if self.peek() == "0" and self.peek(1) in "xX":
            value += self.advance()
            value += self.advance()
            while self.peek() in "0123456789abcdefABCDEF":
                value += self.advance()
            return int(value, 16)
        while self.peek() in "0123456789":
            value += self.advance()
        return int(value, 10)

    def read_ident(self) -> str:
        value = ""
        while self.peek().isalnum() or self.peek() == "_":
            value += self.advance()
        return value

    def tokenize(self) -> list[Token]:
        while True:
            # Skip whitespace and comments, repeating until none remain
            while True:
                self.skip_whitespace()
                if self.peek() == "/" and self.peek(1) == "/":
                    self.skip_comment()
                else:
                    break

            start_line, start_col = self.line, self.column
            ch = self.peek()

            if ch == "\0":
                self.tokens.append(Token(TokenType.EOF, None, start_line, start_col))
                break
            elif ch == '"':
                self.tokens.append(Token(TokenType.STRING, self.read_string(), start_line, start_col))
            elif ch.isdigit():
                self.tokens.append(Token(TokenType.NUMBER, self.read_number(), start_line, start_col))
            elif ch.isalpha() or ch == "_":
                ident = self.read_ident()
                tok_type = KEYWORDS.get(ident, TokenType.IDENT)
                self.tokens.append(Token(tok_type, ident, start_line, start_col))
            elif ch == "+" and self.peek(1) == "+":
                self.error("'++' is not supported; use 'x = x + 1' instead")
            elif ch == "+":
                self.advance()
                self.tokens.append(Token(TokenType.PLUS, "+", start_line, start_col))
            elif ch == "-" and self.peek(1) == "-":
                self.error("'--' is not supported; use 'x = x - 1' instead")
            elif ch == "-" and self.peek(1) == ">":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.ARROW, "->", start_line, start_col))
            elif ch == "-":
                self.advance()
                self.tokens.append(Token(TokenType.MINUS, "-", start_line, start_col))
            elif ch == "*":
                self.advance()
                self.tokens.append(Token(TokenType.STAR, "*", start_line, start_col))
            elif ch == "/":
                self.advance()
                self.tokens.append(Token(TokenType.SLASH, "/", start_line, start_col))
            elif ch == "%":
                self.advance()
                self.tokens.append(Token(TokenType.PERCENT, "%", start_line, start_col))
            elif ch == "=" and self.peek(1) == "=":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.EQEQ, "==", start_line, start_col))
            elif ch == "=" and self.peek(1) == ">":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.ARROW_FAT, "=>", start_line, start_col))
            elif ch == "=":
                self.advance()
                self.tokens.append(Token(TokenType.EQ, "=", start_line, start_col))
            elif ch == "!" and self.peek(1) == "=":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.NEQ, "!=", start_line, start_col))
            elif ch == "!":
                self.advance()
                self.tokens.append(Token(TokenType.BANG, "!", start_line, start_col))
            elif ch == "<" and self.peek(1) == "<":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.LSHIFT, "<<", start_line, start_col))
            elif ch == "<" and self.peek(1) == "=":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.LE, "<=", start_line, start_col))
            elif ch == "<":
                self.advance()
                self.tokens.append(Token(TokenType.LT, "<", start_line, start_col))
            elif ch == ">" and self.peek(1) == ">":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.RSHIFT, ">>", start_line, start_col))
            elif ch == ">" and self.peek(1) == "=":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.GE, ">=", start_line, start_col))
            elif ch == ">":
                self.advance()
                self.tokens.append(Token(TokenType.GT, ">", start_line, start_col))
            elif ch == "&" and self.peek(1) == "&":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.ANDAND, "&&", start_line, start_col))
            elif ch == "&":
                self.advance()
                self.tokens.append(Token(TokenType.AMPERSAND, "&", start_line, start_col))
            elif ch == "|" and self.peek(1) == "|":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.OROR, "||", start_line, start_col))
            elif ch == "|":
                self.advance()
                self.tokens.append(Token(TokenType.PIPE, "|", start_line, start_col))
            elif ch == "^":
                self.advance()
                self.tokens.append(Token(TokenType.CARET, "^", start_line, start_col))
            elif ch == "~":
                self.advance()
                self.tokens.append(Token(TokenType.TILDE, "~", start_line, start_col))
            elif ch == "." and self.peek(1) == "." and self.peek(2) == ".":
                self.advance(); self.advance(); self.advance()
                self.tokens.append(Token(TokenType.ELLIPSIS, "...", start_line, start_col))
            elif ch == ".":
                self.advance()
                self.tokens.append(Token(TokenType.DOT, ".", start_line, start_col))
            elif ch == "(":
                self.advance()
                self.tokens.append(Token(TokenType.LPAREN, "(", start_line, start_col))
            elif ch == ")":
                self.advance()
                self.tokens.append(Token(TokenType.RPAREN, ")", start_line, start_col))
            elif ch == "{":
                self.advance()
                self.tokens.append(Token(TokenType.LBRACE, "{", start_line, start_col))
            elif ch == "}":
                self.advance()
                self.tokens.append(Token(TokenType.RBRACE, "}", start_line, start_col))
            elif ch == "[":
                self.advance()
                self.tokens.append(Token(TokenType.LBRACKET, "[", start_line, start_col))
            elif ch == "]":
                self.advance()
                self.tokens.append(Token(TokenType.RBRACKET, "]", start_line, start_col))
            elif ch == ":" and self.peek(1) == ":":
                self.advance(); self.advance()
                self.tokens.append(Token(TokenType.COLONCOLON, "::", start_line, start_col))
            elif ch == ":":
                self.advance()
                self.tokens.append(Token(TokenType.COLON, ":", start_line, start_col))
            elif ch == ";":
                self.advance()
                self.tokens.append(Token(TokenType.SEMICOLON, ";", start_line, start_col))
            elif ch == ",":
                self.advance()
                self.tokens.append(Token(TokenType.COMMA, ",", start_line, start_col))
            elif ch == "#":
                self.advance()
                self.tokens.append(Token(TokenType.HASH, "#", start_line, start_col))
            elif ch == "@":
                self.advance()
                self.tokens.append(Token(TokenType.AT, "@", start_line, start_col))
            else:
                self.error(f"Unexpected character '{ch}'")

        return self.tokens
