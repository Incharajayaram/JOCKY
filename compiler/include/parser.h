#pragma once

#include "lexer.h"
#include "ast.h"
#include <memory>
#include <stdexcept>

namespace jocky {

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& msg) : std::runtime_error(msg) {}
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::unique_ptr<Program> parse();

private:
    std::vector<Token> toks;
    size_t pos = 0;

    const Token& peek(size_t offset = 0) const;
    const Token& advance();
    bool match(TokenType t) const;
    bool matchAny(std::initializer_list<TokenType> types) const;
    const Token& expect(TokenType t, const std::string& msg = "");

    JType parseType();

    std::unique_ptr<Decl> parseDecl();
    std::unique_ptr<FuncDecl> parseFuncDecl();
    std::unique_ptr<FFIDecl> parseFFIDecl();
    std::vector<Param> parseParams();

    std::unique_ptr<Block> parseBlock();
    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Stmt> parseLetStmt();
    std::unique_ptr<Stmt> parseIfStmt();
    std::unique_ptr<Stmt> parseWhileStmt();
    std::unique_ptr<Stmt> parseForStmt();
    std::unique_ptr<Stmt> parseReturnStmt();
    std::unique_ptr<Stmt> parseExprOrAssignStmt();

    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseOr();
    std::unique_ptr<Expr> parseAnd();
    std::unique_ptr<Expr> parseEq();
    std::unique_ptr<Expr> parseRel();
    std::unique_ptr<Expr> parseAdd();
    std::unique_ptr<Expr> parseMul();
    std::unique_ptr<Expr> parseUnary();
    std::unique_ptr<Expr> parsePostfix();
    std::unique_ptr<Expr> parsePrimary();
    std::unique_ptr<Expr> parseCallArgs(std::string name);
};

} // namespace jocky
