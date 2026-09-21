#include "lexer.h"
#include <cctype>
#include <unordered_map>

namespace jocky {

static const std::unordered_map<std::string, TokenType> keywords = {
    {"fn", TokenType::Fn}, {"ffi", TokenType::Ffi},
    {"let", TokenType::Let}, {"if", TokenType::If},
    {"else", TokenType::Else}, {"while", TokenType::While},
    {"for", TokenType::For}, {"return", TokenType::Return},
    {"true", TokenType::True}, {"false", TokenType::False},
    {"i8", TokenType::I8}, {"i32", TokenType::I32},
    {"i64", TokenType::I64}, {"bool", TokenType::Bool},
    {"void", TokenType::Void}, {"string", TokenType::StringKw},
};

Lexer::Lexer(std::string source) : src(std::move(source)) {}

char Lexer::peek(size_t offset) const {
    size_t p = pos + offset;
    if (p >= src.size()) return '\0';
    return src[p];
}

char Lexer::advance() {
    char c = peek();
    pos++;
    if (c == '\n') { line++; col = 1; }
    else { col++; }
    return c;
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(peek()))) advance();
}

void Lexer::skipComment() {
    if (peek() == '/' && peek(1) == '/') {
        while (peek() != '\n' && peek() != '\0') advance();
    }
}

Token Lexer::makeToken(TokenType type, std::string text) {
    return Token{type, std::move(text), 0, line, col};
}

Token Lexer::makeNumber(int64_t value) {
    return Token{TokenType::Number, "", value, line, col};
}

Token Lexer::makeString(std::string value) {
    return Token{TokenType::String, std::move(value), 0, line, col};
}

Token Lexer::makeIdent(std::string name) {
    auto it = keywords.find(name);
    if (it != keywords.end()) {
        return Token{it->second, std::move(name), 0, line, col};
    }
    return Token{TokenType::Ident, std::move(name), 0, line, col};
}

Token Lexer::readString() {
    advance(); // consume opening "
    std::string value;
    while (peek() != '"') {
        if (peek() == '\0') throw LexerError("Unterminated string");
        if (peek() == '\\') {
            advance();
            char esc = advance();
            switch (esc) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                case '0': value += '\0'; break;
                default: value += esc; break;
            }
        } else {
            value += advance();
        }
    }
    advance(); // consume closing "
    return makeString(std::move(value));
}

Token Lexer::readNumber() {
    std::string value;
    if (peek() == '0' && (peek(1) == 'x' || peek(1) == 'X')) {
        value += advance(); value += advance();
        while (std::isxdigit(static_cast<unsigned char>(peek()))) value += advance();
        return makeNumber(std::stoll(value, nullptr, 16));
    }
    while (std::isdigit(static_cast<unsigned char>(peek()))) value += advance();
    return makeNumber(std::stoll(value));
}

Token Lexer::readIdent() {
    std::string value;
    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') value += advance();
    return makeIdent(std::move(value));
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> result;
    while (true) {
        skipWhitespace();
        skipComment();
        skipWhitespace();

        int startLine = line, startCol = col;
        char c = peek();

        if (c == '\0') {
            result.push_back(Token{TokenType::End, "", 0, startLine, startCol});
            break;
        }
        if (c == '"') {
            result.push_back(readString());
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c))) {
            result.push_back(readNumber());
            continue;
        }
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            result.push_back(readIdent());
            continue;
        }

        // Multi-char operators
        auto add2 = [&](TokenType t, const std::string& s) {
            advance(); advance();
            Token tok{t, s, 0, startLine, startCol};
            result.push_back(tok);
        };
        auto add1 = [&](TokenType t, const std::string& s) {
            advance();
            Token tok{t, s, 0, startLine, startCol};
            result.push_back(tok);
        };

        if (c == '+' && peek(1) == '+') { add2(TokenType::Plus, "++"); continue; }
        if (c == '+' ) { add1(TokenType::Plus, "+"); continue; }
        if (c == '-' && peek(1) == '>') { add2(TokenType::Arrow, "->"); continue; }
        if (c == '-' ) { add1(TokenType::Minus, "-"); continue; }
        if (c == '*' ) { add1(TokenType::Star, "*"); continue; }
        if (c == '/' ) { add1(TokenType::Slash, "/"); continue; }
        if (c == '%' ) { add1(TokenType::Percent, "%"); continue; }
        if (c == '=' && peek(1) == '=') { add2(TokenType::EqEq, "=="); continue; }
        if (c == '=' ) { add1(TokenType::Eq, "="); continue; }
        if (c == '!' && peek(1) == '=') { add2(TokenType::Neq, "!="); continue; }
        if (c == '!' ) { add1(TokenType::Bang, "!"); continue; }
        if (c == '<' && peek(1) == '=') { add2(TokenType::Le, "<="); continue; }
        if (c == '<' ) { add1(TokenType::Lt, "<"); continue; }
        if (c == '>' && peek(1) == '=') { add2(TokenType::Ge, ">="); continue; }
        if (c == '>' ) { add1(TokenType::Gt, ">"); continue; }
        if (c == '&' && peek(1) == '&') { add2(TokenType::AndAnd, "&&"); continue; }
        if (c == '&' ) { add1(TokenType::Ampersand, "&"); continue; }
        if (c == '|' && peek(1) == '|') { add2(TokenType::OrOr, "||"); continue; }
        if (c == '.' && peek(1) == '.' && peek(2) == '.') {
            advance(); advance(); advance();
            result.push_back(Token{TokenType::Ellipsis, "...", 0, startLine, startCol});
            continue;
        }
        if (c == '(' ) { add1(TokenType::LParen, "("); continue; }
        if (c == ')' ) { add1(TokenType::RParen, ")"); continue; }
        if (c == '{' ) { add1(TokenType::LBrace, "{"); continue; }
        if (c == '}' ) { add1(TokenType::RBrace, "}"); continue; }
        if (c == '[' ) { add1(TokenType::LBracket, "["); continue; }
        if (c == ']' ) { add1(TokenType::RBracket, "]"); continue; }
        if (c == ':' ) { add1(TokenType::Colon, ":"); continue; }
        if (c == ';' ) { add1(TokenType::Semicolon, ";"); continue; }
        if (c == ',' ) { add1(TokenType::Comma, ","); continue; }

        throw LexerError(std::string("Unexpected character: ") + c);
    }
    return result;
}

const char* tokenTypeName(TokenType t) {
    switch (t) {
        case TokenType::End: return "End";
        case TokenType::Number: return "Number";
        case TokenType::String: return "String";
        case TokenType::Ident: return "Ident";
        case TokenType::Fn: return "Fn";
        case TokenType::Ffi: return "Ffi";
        case TokenType::Let: return "Let";
        case TokenType::If: return "If";
        case TokenType::Else: return "Else";
        case TokenType::While: return "While";
        case TokenType::For: return "For";
        case TokenType::Return: return "Return";
        case TokenType::True: return "True";
        case TokenType::False: return "False";
        case TokenType::I8: return "I8";
        case TokenType::I32: return "I32";
        case TokenType::I64: return "I64";
        case TokenType::Bool: return "Bool";
        case TokenType::Void: return "Void";
        case TokenType::StringKw: return "StringKw";
        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";
        case TokenType::Percent: return "Percent";
        case TokenType::Eq: return "Eq";
        case TokenType::EqEq: return "EqEq";
        case TokenType::Neq: return "Neq";
        case TokenType::Lt: return "Lt";
        case TokenType::Gt: return "Gt";
        case TokenType::Le: return "Le";
        case TokenType::Ge: return "Ge";
        case TokenType::AndAnd: return "AndAnd";
        case TokenType::OrOr: return "OrOr";
        case TokenType::Bang: return "Bang";
        case TokenType::Ampersand: return "Ampersand";
        case TokenType::Arrow: return "Arrow";
        case TokenType::Ellipsis: return "Ellipsis";
        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";
        case TokenType::LBrace: return "LBrace";
        case TokenType::RBrace: return "RBrace";
        case TokenType::LBracket: return "LBracket";
        case TokenType::RBracket: return "RBracket";
        case TokenType::Colon: return "Colon";
        case TokenType::Semicolon: return "Semicolon";
        case TokenType::Comma: return "Comma";
    }
    return "Unknown";
}

} // namespace jocky
