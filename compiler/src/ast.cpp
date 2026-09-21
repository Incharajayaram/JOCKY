#include "ast.h"

namespace jocky {

std::string JType::toString() const {
    switch (kind) {
        case JTypeKind::Void: return "void";
        case JTypeKind::Bool: return "bool";
        case JTypeKind::I8: return "i8";
        case JTypeKind::I32: return "i32";
        case JTypeKind::I64: return "i64";
        case JTypeKind::String: return "string";
        case JTypeKind::Custom: return customName;
        case JTypeKind::Pointer: {
            JType base = *this;
            base.isPointer = false;
            return base.toString() + "*";
        }
    }
    return "unknown";
}

std::string JType::llvmType() const {
    std::string base;
    switch (kind) {
        case JTypeKind::Void: base = "void"; break;
        case JTypeKind::Bool: base = "i1"; break;
        case JTypeKind::I8: base = "i8"; break;
        case JTypeKind::I32: base = "i32"; break;
        case JTypeKind::I64: base = "i64"; break;
        case JTypeKind::String: base = "i8"; break;
        case JTypeKind::Custom: base = "%" + customName; break;
        case JTypeKind::Pointer: {
            JType inner = *this;
            inner.isPointer = false;
            return inner.llvmType() + "*";
        }
    }
    if (isPointer || kind == JTypeKind::String) return base + "*";
    return base;
}

} // namespace jocky
