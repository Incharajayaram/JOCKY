#pragma once

#include "ast.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace jocky {

class CodeGenError : public std::runtime_error {
public:
    CodeGenError(const std::string& msg) : std::runtime_error(msg) {}
};

class CodeGen {
public:
    CodeGen();
    std::unique_ptr<llvm::Module> generate(Program& prog, const std::string& moduleName);

private:
    llvm::LLVMContext ctx;
    std::unique_ptr<llvm::Module> mod;
    std::unique_ptr<llvm::IRBuilder<>> builder;

    struct FuncSig {
        std::vector<JType> params;
        JType ret;
    };
    std::unordered_map<std::string, FuncSig> funcs;

    struct Local {
        llvm::Value* alloca = nullptr;
        JType type;
    };
    std::unordered_map<std::string, Local> locals;

    llvm::Function* currentFunc = nullptr;

    void declareFunc(FuncDecl& decl);
    void declareFFI(FFIDecl& decl);
    void emitFunc(FuncDecl& decl);
    void emitBlock(Block& block);
    void emitStmt(Stmt& stmt);

    llvm::Value* emitExpr(Expr& expr);
    llvm::Value* emitBinary(BinaryOp& op);
    llvm::Value* emitUnary(UnaryOp& op);
    llvm::Value* emitCall(CallExpr& call);

    llvm::Type* llvmType(const JType& t);
    llvm::Value* emitCast(llvm::Value* val, const JType& from, const JType& to);
    JType inferType(Expr& expr);
    bool isNumeric(const JType& t) const;
};

} // namespace jocky
