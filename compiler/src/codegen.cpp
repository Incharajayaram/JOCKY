#include "codegen.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

namespace jocky {

CodeGen::CodeGen() {}

llvm::Type* CodeGen::llvmType(const JType& t) {
    llvm::Type* base = nullptr;
    switch (t.kind) {
        case JTypeKind::Void:   base = llvm::Type::getVoidTy(ctx); break;
        case JTypeKind::Bool:   base = llvm::Type::getInt1Ty(ctx); break;
        case JTypeKind::I8:     base = llvm::Type::getInt8Ty(ctx); break;
        case JTypeKind::I32:    base = llvm::Type::getInt32Ty(ctx); break;
        case JTypeKind::I64:    base = llvm::Type::getInt64Ty(ctx); break;
        case JTypeKind::String: base = llvm::Type::getInt8Ty(ctx); break;
        case JTypeKind::Custom: base = llvm::StructType::getTypeByName(ctx, t.customName); break;
        case JTypeKind::Pointer: {
            JType inner = t;
            inner.isPointer = false;
            return llvm::PointerType::get(llvmType(inner), 0);
        }
    }
    if (!base) throw CodeGenError("Unknown type kind");
    if (t.isPointer || t.kind == JTypeKind::String)
        return llvm::PointerType::get(base, 0);
    return base;
}

bool CodeGen::isNumeric(const JType& t) const {
    return t.kind == JTypeKind::I8 || t.kind == JTypeKind::I32 || t.kind == JTypeKind::I64;
}

llvm::Value* CodeGen::emitCast(llvm::Value* val, const JType& from, const JType& to) {
    if (from.kind == to.kind && from.isPointer == to.isPointer) return val;

    llvm::Type* toTy = llvmType(to);

    if (from.kind == JTypeKind::Bool && isNumeric(to)) {
        return builder->CreateZExt(val, toTy, "cast");
    }
    if (isNumeric(from) && to.kind == JTypeKind::Bool) {
        return builder->CreateICmpNE(val, llvm::ConstantInt::get(val->getType(), 0), "cast");
    }
    if (isNumeric(from) && isNumeric(to)) {
        unsigned fromBits = val->getType()->getIntegerBitWidth();
        unsigned toBits = toTy->getIntegerBitWidth();
        if (fromBits < toBits)
            return builder->CreateSExt(val, toTy, "cast");
        else
            return builder->CreateTrunc(val, toTy, "cast");
    }
    if (from.isPointer && to.isPointer) {
        return builder->CreateBitCast(val, toTy, "cast");
    }
    throw CodeGenError("Unsupported cast");
}

std::unique_ptr<llvm::Module> CodeGen::generate(Program& prog, const std::string& moduleName) {
    mod = std::make_unique<llvm::Module>(moduleName, ctx);
    builder = std::make_unique<llvm::IRBuilder<>>(ctx);

    // Collect signatures
    for (auto& decl : prog.decls) {
        if (auto* fd = dynamic_cast<FuncDecl*>(decl.get())) {
            declareFunc(*fd);
        } else if (auto* ffi = dynamic_cast<FFIDecl*>(decl.get())) {
            declareFFI(*ffi);
        }
    }

    // Emit FFI declarations
    for (auto& decl : prog.decls) {
        if (auto* ffi = dynamic_cast<FFIDecl*>(decl.get())) {
            // Already declared, nothing more
            (void)ffi;
        }
    }

    // Emit function bodies
    for (auto& decl : prog.decls) {
        if (auto* fd = dynamic_cast<FuncDecl*>(decl.get())) {
            emitFunc(*fd);
        }
    }

    // Verify module
    std::string err;
    llvm::raw_string_ostream errs(err);
    if (llvm::verifyModule(*mod, &errs)) {
        throw CodeGenError("Module verification failed: " + err);
    }

    return std::move(mod);
}

void CodeGen::declareFunc(FuncDecl& decl) {
    std::vector<llvm::Type*> paramTypes;
    for (auto& p : decl.params) {
        paramTypes.push_back(llvmType(p.type));
    }
    llvm::FunctionType* ft = llvm::FunctionType::get(llvmType(decl.retType), paramTypes, false);
    llvm::Function* f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, decl.name, mod.get());

    // Set parameter names
    size_t i = 0;
    for (auto& arg : f->args()) {
        arg.setName(decl.params[i++].name);
    }

    FuncSig sig;
    for (auto& p : decl.params) sig.params.push_back(p.type);
    sig.ret = decl.retType;
    funcs[decl.name] = sig;
}

void CodeGen::declareFFI(FFIDecl& decl) {
    std::vector<llvm::Type*> paramTypes;
    for (auto& p : decl.params) {
        paramTypes.push_back(llvmType(p.type));
    }
    llvm::FunctionType* ft = llvm::FunctionType::get(llvmType(decl.retType), paramTypes, decl.variadic);
    llvm::Function::Create(ft, llvm::Function::ExternalLinkage, decl.name, mod.get());

    FuncSig sig;
    for (auto& p : decl.params) sig.params.push_back(p.type);
    sig.ret = decl.retType;
    funcs[decl.name] = sig;
}

void CodeGen::emitFunc(FuncDecl& decl) {
    llvm::Function* f = mod->getFunction(decl.name);
    if (!f) throw CodeGenError("Function not found: " + decl.name);
    currentFunc = f;

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(ctx, "entry", f);
    builder->SetInsertPoint(entry);
    locals.clear();

    // Allocate and store parameters
    for (auto& arg : f->args()) {
        std::string name = arg.getName().str();
        llvm::AllocaInst* alloca = builder->CreateAlloca(arg.getType(), nullptr, name + ".addr");
        builder->CreateStore(&arg, alloca);
        JType t;
        for (auto& p : decl.params) {
            if (p.name == name) { t = p.type; break; }
        }
        locals[name] = {alloca, t};
    }

    emitBlock(*decl.body);

    // Ensure terminator
    if (!builder->GetInsertBlock()->getTerminator()) {
        if (decl.retType.kind == JTypeKind::Void) {
            builder->CreateRetVoid();
        } else {
            builder->CreateRet(llvm::ConstantInt::get(llvmType(decl.retType), 0));
        }
    }
}

void CodeGen::emitBlock(Block& block) {
    for (auto& stmt : block.stmts) {
        emitStmt(*stmt);
    }
}

void CodeGen::emitStmt(Stmt& stmt) {
    if (auto* let = dynamic_cast<LetStmt*>(&stmt)) {
        JType t = let->type;
        if (t.kind == JTypeKind::Void) {
            t = JType::makeI32(); // default fallback
        }
        llvm::Value* val = emitExpr(*let->init);
        llvm::AllocaInst* alloca = builder->CreateAlloca(llvmType(t), nullptr, let->name);
        builder->CreateStore(val, alloca);
        locals[let->name] = {alloca, t};
    }
    else if (auto* assign = dynamic_cast<AssignStmt*>(&stmt)) {
        if (auto* vr = dynamic_cast<VarRef*>(assign->target.get())) {
            auto it = locals.find(vr->name);
            if (it == locals.end()) throw CodeGenError("Undefined variable: " + vr->name);
            llvm::Value* val = emitExpr(*assign->value);
            builder->CreateStore(val, it->second.alloca);
        } else {
            throw CodeGenError("Unsupported assignment target");
        }
    }
    else if (auto* ifs = dynamic_cast<IfStmt*>(&stmt)) {
        llvm::Value* cond = emitExpr(*ifs->cond);
        llvm::Function* f = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(ctx, "then", f);
        llvm::BasicBlock* elseBB = ifs->elseBlock ? llvm::BasicBlock::Create(ctx, "else", f) : nullptr;
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(ctx, "merge", f);

        if (elseBB) {
            builder->CreateCondBr(cond, thenBB, elseBB);
        } else {
            builder->CreateCondBr(cond, thenBB, mergeBB);
        }

        builder->SetInsertPoint(thenBB);
        emitBlock(*ifs->thenBlock);
        if (!builder->GetInsertBlock()->getTerminator())
            builder->CreateBr(mergeBB);

        if (elseBB) {
            builder->SetInsertPoint(elseBB);
            emitBlock(*ifs->elseBlock);
            if (!builder->GetInsertBlock()->getTerminator())
                builder->CreateBr(mergeBB);
        }

        builder->SetInsertPoint(mergeBB);
    }
    else if (auto* wh = dynamic_cast<WhileStmt*>(&stmt)) {
        llvm::Function* f = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock* headerBB = llvm::BasicBlock::Create(ctx, "while.cond", f);
        llvm::BasicBlock* bodyBB = llvm::BasicBlock::Create(ctx, "while.body", f);
        llvm::BasicBlock* endBB = llvm::BasicBlock::Create(ctx, "while.end", f);

        builder->CreateBr(headerBB);
        builder->SetInsertPoint(headerBB);
        llvm::Value* cond = emitExpr(*wh->cond);
        builder->CreateCondBr(cond, bodyBB, endBB);

        builder->SetInsertPoint(bodyBB);
        emitBlock(*wh->body);
        if (!builder->GetInsertBlock()->getTerminator())
            builder->CreateBr(headerBB);

        builder->SetInsertPoint(endBB);
    }
    else if (auto* ret = dynamic_cast<ReturnStmt*>(&stmt)) {
        if (ret->value) {
            builder->CreateRet(emitExpr(*ret->value));
        } else {
            builder->CreateRetVoid();
        }
    }
    else if (auto* es = dynamic_cast<ExprStmt*>(&stmt)) {
        emitExpr(*es->expr);
    }
}

llvm::Value* CodeGen::emitExpr(Expr& expr) {
    if (auto* lit = dynamic_cast<IntLiteral*>(&expr)) {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), lit->value);
    }
    if (auto* bl = dynamic_cast<BoolLiteral*>(&expr)) {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), bl->value ? 1 : 0);
    }
    if (auto* sl = dynamic_cast<StringLiteral*>(&expr)) {
        llvm::Constant* str = builder->CreateGlobalStringPtr(sl->value, ".str");
        return str;
    }
    if (auto* vr = dynamic_cast<VarRef*>(&expr)) {
        auto it = locals.find(vr->name);
        if (it == locals.end()) throw CodeGenError("Undefined variable: " + vr->name);
        return builder->CreateLoad(llvmType(it->second.type), it->second.alloca, vr->name);
    }
    if (auto* bin = dynamic_cast<BinaryOp*>(&expr)) {
        return emitBinary(*bin);
    }
    if (auto* un = dynamic_cast<UnaryOp*>(&expr)) {
        return emitUnary(*un);
    }
    if (auto* call = dynamic_cast<CallExpr*>(&expr)) {
        return emitCall(*call);
    }
    throw CodeGenError("Unknown expression type");
}

llvm::Value* CodeGen::emitBinary(BinaryOp& op) {
    llvm::Value* L = emitExpr(*op.left);
    llvm::Value* R = emitExpr(*op.right);

    if (op.op == "+") return builder->CreateAdd(L, R, "add");
    if (op.op == "-") return builder->CreateSub(L, R, "sub");
    if (op.op == "*") return builder->CreateMul(L, R, "mul");
    if (op.op == "/") return builder->CreateSDiv(L, R, "div");
    if (op.op == "%") return builder->CreateSRem(L, R, "rem");
    if (op.op == "==") return builder->CreateICmpEQ(L, R, "eq");
    if (op.op == "!=") return builder->CreateICmpNE(L, R, "ne");
    if (op.op == "<")  return builder->CreateICmpSLT(L, R, "lt");
    if (op.op == ">")  return builder->CreateICmpSGT(L, R, "gt");
    if (op.op == "<=") return builder->CreateICmpSLE(L, R, "le");
    if (op.op == ">=") return builder->CreateICmpSGE(L, R, "ge");
    if (op.op == "&&") return builder->CreateAnd(L, R, "and");
    if (op.op == "||") return builder->CreateOr(L, R, "or");

    throw CodeGenError("Unknown binary operator: " + op.op);
}

llvm::Value* CodeGen::emitUnary(UnaryOp& op) {
    llvm::Value* V = emitExpr(*op.operand);
    if (op.op == "-") {
        return builder->CreateNeg(V, "neg");
    }
    if (op.op == "!") {
        return builder->CreateXor(V, llvm::ConstantInt::get(V->getType(), 1), "not");
    }
    throw CodeGenError("Unknown unary operator: " + op.op);
}

llvm::Value* CodeGen::emitCall(CallExpr& call) {
    llvm::Function* f = mod->getFunction(call.name);
    if (!f) throw CodeGenError("Undefined function: " + call.name);

    std::vector<llvm::Value*> args;
    for (auto& arg : call.args) {
        args.push_back(emitExpr(*arg));
    }

    return builder->CreateCall(f, args, call.name + "_call");
}

} // namespace jocky
