#pragma once

#include <string>
#include <vector>
#include <memory>

namespace jocky {

enum class JTypeKind {
    Void, Bool,
    I8, I32, I64,
    String,
    Pointer,
    Custom
};

struct JType {
    JTypeKind kind = JTypeKind::Void;
    bool isPointer = false;
    std::string customName;

    static JType makeVoid()   { JType t; t.kind = JTypeKind::Void;   return t; }
    static JType makeBool()   { JType t; t.kind = JTypeKind::Bool;   return t; }
    static JType makeI8()     { JType t; t.kind = JTypeKind::I8;     return t; }
    static JType makeI32()    { JType t; t.kind = JTypeKind::I32;    return t; }
    static JType makeI64()    { JType t; t.kind = JTypeKind::I64;    return t; }
    static JType makeString() { JType t; t.kind = JTypeKind::String; return t; }
    static JType makePtr(JType base) { base.isPointer = true; return base; }

    std::string toString() const;
    std::string llvmType() const;
};

/* -------------------------------------------------------------------------- */
/* AST Nodes                                                                  */
/* -------------------------------------------------------------------------- */

struct Expr;
struct Stmt;

struct Program {
    std::vector<std::unique_ptr<struct Decl>> decls;
};

struct Param {
    std::string name;
    JType type;
};

struct Decl {
    virtual ~Decl() = default;
};

struct FuncDecl : Decl {
    std::string name;
    std::vector<Param> params;
    JType retType;
    std::unique_ptr<struct Block> body;
};

struct FFIDecl : Decl {
    std::string name;
    std::vector<Param> params;
    JType retType;
    bool variadic = false;
};

struct Block {
    std::vector<std::unique_ptr<Stmt>> stmts;
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct LetStmt : Stmt {
    std::string name;
    JType type;
    std::unique_ptr<Expr> init;
};

struct AssignStmt : Stmt {
    std::unique_ptr<Expr> target;
    std::unique_ptr<Expr> value;
};

struct IfStmt : Stmt {
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Block> thenBlock;
    std::unique_ptr<Block> elseBlock;
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Block> body;
};

struct ForStmt : Stmt {
    std::unique_ptr<Stmt> init;
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Expr> step;
    std::unique_ptr<Block> body;
};

struct ReturnStmt : Stmt {
    std::unique_ptr<Expr> value;
};

struct ExprStmt : Stmt {
    std::unique_ptr<Expr> expr;
};

struct Expr {
    virtual ~Expr() = default;
};

struct IntLiteral : Expr {
    int64_t value = 0;
};

struct BoolLiteral : Expr {
    bool value = false;
};

struct StringLiteral : Expr {
    std::string value;
};

struct VarRef : Expr {
    std::string name;
};

struct BinaryOp : Expr {
    std::string op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

struct UnaryOp : Expr {
    std::string op;
    std::unique_ptr<Expr> operand;
};

struct CallExpr : Expr {
    std::string name;
    std::vector<std::unique_ptr<Expr>> args;
};

struct DerefExpr : Expr {
    std::unique_ptr<Expr> operand;
};

struct AddrOfExpr : Expr {
    std::unique_ptr<Expr> operand;
};

struct CastExpr : Expr {
    JType targetType;
    std::unique_ptr<Expr> operand;
};

struct IndexExpr : Expr {
    std::unique_ptr<Expr> base;
    std::unique_ptr<Expr> index;
};

} // namespace jocky
