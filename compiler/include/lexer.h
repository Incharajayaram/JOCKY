#pragma once

#include "token.h"
#include <string>
#include <vector>
#include <stdexcept>

namespace jocky {

class LexerError : public std::runtime_error {
public:
    LexerError(const std::string& msg) : std::runtime_error(msg) {}
};

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t pos = 0;
    int line = 1;
    int col = 1;

    char peek(size_t offset = 0) const;
    char advance();
    void skipWhitespace();
    void skipComment();
    Token makeToken(TokenType type, std::string text);
    Token makeNumber(int64_t value);
    Token makeString(std::string value);
    Token makeIdent(std::string name);

    Token readString();
    Token readNumber();
    Token readIdent();
};

} // namespace jocky
