#include "parser.h"
#include <algorithm>

namespace jocky {

Parser::Parser(std::vector<Token> tokens) : toks(std::move(tokens)) {}

const Token& Parser::peek(size_t offset) const {
    size_t p = pos + offset;
    if (p < toks.size()) return toks[p];
    return toks.back();
}

const Token& Parser::advance() {
    const Token& t = peek();
    pos++;
    return t;
}

bool Parser::match(TokenType t) const {
    return peek().type == t;
}

bool Parser::matchAny(std::initializer_list<TokenType> types) const {
    return std::any_of(types.begin(), types.end(),
        [&](TokenType t) { return match(t); });
}

const Token& Parser::expect(TokenType t, const std::string& msg) {
    if (!match(t)) {
        throw ParseError("Expected " + std::string(tokenTypeName(t)) +
                         " at line " + std::to_string(peek().line) +
                         ", got " + tokenTypeName(peek().type) +
                         (msg.empty() ? "" : ": " + msg));
    }
    return advance();
}

JType Parser::parseType() {
    JType t;
    switch (peek().type) {
        case TokenType::I8:   advance(); t = JType::makeI8(); break;
        case TokenType::I32:  advance(); t = JType::makeI32(); break;
        case TokenType::I64:  advance(); t = JType::makeI64(); break;
        case TokenType::Bool: advance(); t = JType::makeBool(); break;
        case TokenType::Void: advance(); t = JType::makeVoid(); break;
        case TokenType::StringKw: advance(); t = JType::makeString(); break;
        default:
            throw ParseError("Expected type at line " + std::to_string(peek().line));
    }
    while (match(TokenType::Star)) {
        advance();
        t = JType::makePtr(t);
    }
    return t;
}

std::unique_ptr<Program> Parser::parse() {
    auto prog = std::make_unique<Program>();
    while (!match(TokenType::End)) {
        prog->decls.push_back(parseDecl());
    }
    return prog;
}

std::unique_ptr<Decl> Parser::parseDecl() {
    if (match(TokenType::Fn)) return parseFuncDecl();
    if (match(TokenType::Ffi)) return parseFFIDecl();
    throw ParseError("Expected fn or ffi at line " + std::to_string(peek().line));
}

std::unique_ptr<FuncDecl> Parser::parseFuncDecl() {
    expect(TokenType::Fn);
    auto decl = std::make_unique<FuncDecl>();
    decl->name = expect(TokenType::Ident).text;
    expect(TokenType::LParen);
    decl->params = parseParams();
    expect(TokenType::RParen);
    decl->retType = JType::makeVoid();
    if (match(TokenType::Arrow)) {
        advance();
        decl->retType = parseType();
    }
    decl->body = parseBlock();
    return decl;
}

std::unique_ptr<FFIDecl> Parser::parseFFIDecl() {
    expect(TokenType::Ffi);
    auto decl = std::make_unique<FFIDecl>();
    decl->name = expect(TokenType::Ident).text;
    expect(TokenType::LParen);
    decl->params = parseParams();
    decl->variadic = false;
    if (match(TokenType::Ellipsis)) {
        advance();
        decl->variadic = true;
    }
    expect(TokenType::RParen);
    expect(TokenType::Arrow);
    decl->retType = parseType();
    expect(TokenType::Semicolon);
    return decl;
}

std::vector<Param> Parser::parseParams() {
    std::vector<Param> params;
    if (match(TokenType::RParen) || match(TokenType::Ellipsis)) return params;
    while (true) {
        if (match(TokenType::Ellipsis)) break;
        Param p;
        p.name = expect(TokenType::Ident).text;
        expect(TokenType::Colon);
        p.type = parseType();
        params.push_back(std::move(p));
        if (match(TokenType::Comma)) {
            advance();
        } else {
            break;
        }
    }
    return params;
}

std::unique_ptr<Block> Parser::parseBlock() {
    expect(TokenType::LBrace);
    auto block = std::make_unique<Block>();
    while (!match(TokenType::RBrace)) {
        block->stmts.push_back(parseStmt());
    }
    expect(TokenType::RBrace);
    return block;
}

std::unique_ptr<Stmt> Parser::parseStmt() {
    if (match(TokenType::Let)) return parseLetStmt();
    if (match(TokenType::If)) return parseIfStmt();
    if (match(TokenType::While)) return parseWhileStmt();
    if (match(TokenType::For)) return parseForStmt();
    if (match(TokenType::Return)) return parseReturnStmt();
    return parseExprOrAssignStmt();
}

std::unique_ptr<Stmt> Parser::parseLetStmt() {
    expect(TokenType::Let);
    auto stmt = std::make_unique<LetStmt>();
    stmt->name = expect(TokenType::Ident).text;
    stmt->type = JType::makeVoid(); // inferred later
    if (match(TokenType::Colon)) {
        advance();
        stmt->type = parseType();
    }
    expect(TokenType::Eq);
    stmt->init = parseExpr();
    expect(TokenType::Semicolon);
    return stmt;
}

std::unique_ptr<Stmt> Parser::parseIfStmt() {
    expect(TokenType::If);
    auto stmt = std::make_unique<IfStmt>();
    stmt->cond = parseExpr();
    stmt->thenBlock = parseBlock();
    if (match(TokenType::Else)) {
        advance();
        stmt->elseBlock = parseBlock();
    }
    return stmt;
}

std::unique_ptr<Stmt> Parser::parseWhileStmt() {
    expect(TokenType::While);
    auto stmt = std::make_unique<WhileStmt>();
    stmt->cond = parseExpr();
    stmt->body = parseBlock();
    return stmt;
}

std::unique_ptr<Stmt> Parser::parseForStmt() {
    expect(TokenType::For);
    expect(TokenType::LParen);
    auto stmt = std::make_unique<ForStmt>();
    // init
    if (match(TokenType::Let)) {
        stmt->init = parseLetStmt();
    } else if (match(TokenType::Semicolon)) {
        advance();
        stmt->init = std::make_unique<ExprStmt>();
        static_cast<ExprStmt*>(stmt->init.get())->expr = std::make_unique<IntLiteral>();
    } else {
        auto es = std::make_unique<ExprStmt>();
        es->expr = parseExpr();
        expect(TokenType::Semicolon);
        stmt->init = std::move(es);
    }
    stmt->cond = parseExpr();
    expect(TokenType::Semicolon);
    stmt->step = parseExpr();
    expect(TokenType::RParen);
    stmt->body = parseBlock();
    return stmt;
}

std::unique_ptr<Stmt> Parser::parseReturnStmt() {
    expect(TokenType::Return);
    auto stmt = std::make_unique<ReturnStmt>();
    if (!match(TokenType::Semicolon)) {
        stmt->value = parseExpr();
    }
    expect(TokenType::Semicolon);
    return stmt;
}

std::unique_ptr<Stmt> Parser::parseExprOrAssignStmt() {
    auto expr = parseExpr();
    if (match(TokenType::Eq)) {
        advance();
        auto stmt = std::make_unique<AssignStmt>();
        stmt->target = std::move(expr);
        stmt->value = parseExpr();
        expect(TokenType::Semicolon);
        return stmt;
    }
    expect(TokenType::Semicolon);
    auto stmt = std::make_unique<ExprStmt>();
    stmt->expr = std::move(expr);
    return stmt;
}

/* -------------------------------------------------------------------------- */
/* Expression parsing (precedence climbing)                                   */
/* -------------------------------------------------------------------------- */

std::unique_ptr<Expr> Parser::parseExpr() { return parseOr(); }

std::unique_ptr<Expr> Parser::parseOr() {
    auto left = parseAnd();
    while (match(TokenType::OrOr)) {
        auto op = std::make_unique<BinaryOp>();
        op->op = advance().text;
        op->left = std::move(left);
        op->right = parseAnd();
        left = std::move(op);
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseAnd() {
    auto left = parseEq();
    while (match(TokenType::AndAnd)) {
        auto op = std::make_unique<BinaryOp>();
        op->op = advance().text;
        op->left = std::move(left);
        op->right = parseEq();
        left = std::move(op);
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseEq() {
    auto left = parseRel();
    while (matchAny({TokenType::EqEq, TokenType::Neq})) {
        auto op = std::make_unique<BinaryOp>();
        op->op = advance().text;
        op->left = std::move(left);
        op->right = parseRel();
        left = std::move(op);
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseRel() {
    auto left = parseAdd();
    while (matchAny({TokenType::Lt, TokenType::Gt, TokenType::Le, TokenType::Ge})) {
        auto op = std::make_unique<BinaryOp>();
        op->op = advance().text;
        op->left = std::move(left);
        op->right = parseAdd();
        left = std::move(op);
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseAdd() {
    auto left = parseMul();
    while (matchAny({TokenType::Plus, TokenType::Minus})) {
        auto op = std::make_unique<BinaryOp>();
        op->op = advance().text;
        op->left = std::move(left);
        op->right = parseMul();
        left = std::move(op);
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseMul() {
    auto left = parseUnary();
    while (matchAny({TokenType::Star, TokenType::Slash, TokenType::Percent})) {
        auto op = std::make_unique<BinaryOp>();
        op->op = advance().text;
        op->left = std::move(left);
        op->right = parseUnary();
        left = std::move(op);
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseUnary() {
    if (matchAny({TokenType::Minus, TokenType::Bang, TokenType::Star, TokenType::Ampersand})) {
        auto op = std::make_unique<UnaryOp>();
        op->op = advance().text;
        op->operand = parseUnary();
        return op;
    }
    return parsePostfix();
}

std::unique_ptr<Expr> Parser::parsePostfix() {
    auto node = parsePrimary();
    while (true) {
        if (match(TokenType::LParen)) {
            // Function call
            if (auto* vr = dynamic_cast<VarRef*>(node.get())) {
                std::string name = vr->name;
                node = parseCallArgs(std::move(name));
            } else {
                throw ParseError("Cannot call non-identifier");
            }
        } else if (match(TokenType::LBracket)) {
            advance();
            auto idx = std::make_unique<IndexExpr>();
            idx->base = std::move(node);
            idx->index = parseExpr();
            expect(TokenType::RBracket);
            node = std::move(idx);
        } else {
            break;
        }
    }
    return node;
}

std::unique_ptr<Expr> Parser::parseCallArgs(std::string name) {
    advance(); // (
    auto call = std::make_unique<CallExpr>();
    call->name = std::move(name);
    if (!match(TokenType::RParen)) {
        while (true) {
            call->args.push_back(parseExpr());
            if (match(TokenType::Comma)) {
                advance();
            } else {
                break;
            }
        }
    }
    expect(TokenType::RParen);
    return call;
}

std::unique_ptr<Expr> Parser::parsePrimary() {
    if (match(TokenType::Number)) {
        auto lit = std::make_unique<IntLiteral>();
        lit->value = advance().intValue;
        return lit;
    }
    if (match(TokenType::String)) {
        auto lit = std::make_unique<StringLiteral>();
        lit->value = advance().text;
        return lit;
    }
    if (match(TokenType::True)) {
        advance();
        auto lit = std::make_unique<BoolLiteral>();
        lit->value = true;
        return lit;
    }
    if (match(TokenType::False)) {
        advance();
        auto lit = std::make_unique<BoolLiteral>();
        lit->value = false;
        return lit;
    }
    if (match(TokenType::Ident)) {
        auto ref = std::make_unique<VarRef>();
        ref->name = advance().text;
        return ref;
    }
    if (match(TokenType::LParen)) {
        advance();
        // Try cast: ( type ) expr
        size_t saved = pos;
        try {
            JType ty = parseType();
            if (match(TokenType::RParen)) {
                advance();
                auto cast = std::make_unique<CastExpr>();
                cast->targetType = ty;
                cast->operand = parseUnary();
                return cast;
            }
        } catch (...) {
            // not a cast
        }
        pos = saved;
        auto expr = parseExpr();
        expect(TokenType::RParen);
        return expr;
    }
    throw ParseError("Unexpected token in expression: " + std::string(tokenTypeName(peek().type)));
}

} // namespace jocky
