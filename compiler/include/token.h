#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace jocky {

enum class TokenType {
    // End
    End,

    // Literals
    Number,
    String,
    Ident,

    // Keywords
    Fn,
    Ffi,
    Let,
    If,
    Else,
    While,
    For,
    Return,
    True,
    False,

    // Types
    I8, I32, I64,
    Bool,
    Void,
    StringKw,

    // Operators
    Plus, Minus, Star, Slash, Percent,
    Eq, EqEq, Neq,
    Lt, Gt, Le, Ge,
    AndAnd, OrOr,
    Bang,
    Ampersand,
    Arrow,
    Ellipsis,

    // Delimiters
    LParen, RParen,
    LBrace, RBrace,
    LBracket, RBracket,
    Colon, Semicolon, Comma,
};

struct Token {
    TokenType type;
    std::string text;
    int64_t intValue = 0;
    int line = 1;
    int col = 1;
};

const char* tokenTypeName(TokenType t);

} // namespace jocky
