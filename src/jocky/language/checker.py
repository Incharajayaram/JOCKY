from .ast import *
from typing import Dict, Any, Optional

class TypeError(Exception):
    pass

class TypeChecker:
    def __init__(self):
        self.functions: Dict[str, (List[JType], JType)] = {}
        self.locals: Dict[str, JType] = {}
        self.current_ret: JType = JType("void")

    def check(self, prog: Program):
        # First pass: collect function signatures
        for decl in prog.decls:
            if isinstance(decl, FuncDecl):
                ptypes = [p.type for p in decl.params]
                self.functions[decl.name] = (ptypes, decl.ret_type, False)
            elif isinstance(decl, FFIDecl):
                ptypes = [p.type for p in decl.params]
                self.functions[decl.name] = (ptypes, decl.ret_type, decl.variadic)

        # Second pass: check bodies
        for decl in prog.decls:
            if isinstance(decl, FuncDecl):
                self.check_func(decl)

    def check_func(self, decl: FuncDecl):
        old_locals = self.locals
        self.locals = {}
        for p in decl.params:
            self.locals[p.name] = p.type
        self.current_ret = decl.ret_type
        self.check_block(decl.body)
        self.locals = old_locals
        # Update function signature with variadic flag (always false for user funcs)
        ptypes = [p.type for p in decl.params]
        self.functions[decl.name] = (ptypes, decl.ret_type, False)

    def check_block(self, block: Block):
        for stmt in block.stmts:
            self.check_stmt(stmt)

    def check_stmt(self, stmt: Any):
        if isinstance(stmt, LetStmt):
            init_type = self.typeof(stmt.init)
            if stmt.type is not None:
                if not self.types_equal(stmt.type, init_type):
                    raise TypeError(f"Type mismatch in let: expected {stmt.type}, got {init_type}")
                self.locals[stmt.name] = stmt.type
            else:
                self.locals[stmt.name] = init_type
        elif isinstance(stmt, AssignStmt):
            lt = self.typeof(stmt.target)
            rt = self.typeof(stmt.value)
            if not self.types_equal(lt, rt):
                raise TypeError(f"Type mismatch in assignment: {lt} vs {rt}")
        elif isinstance(stmt, IfStmt):
            ct = self.typeof(stmt.cond)
            if ct.name != "bool":
                raise TypeError("If condition must be bool")
            self.check_block(stmt.then_block)
            if stmt.else_block:
                self.check_block(stmt.else_block)
        elif isinstance(stmt, WhileStmt):
            ct = self.typeof(stmt.cond)
            if ct.name != "bool":
                raise TypeError("While condition must be bool")
            self.check_block(stmt.body)
        elif isinstance(stmt, ForStmt):
            self.check_stmt(stmt.init)
            ct = self.typeof(stmt.cond)
            if ct.name != "bool":
                raise TypeError("For condition must be bool")
            self.check_stmt(ExprStmt(stmt.step))
            self.check_block(stmt.body)
        elif isinstance(stmt, ReturnStmt):
            if stmt.value is None:
                if self.current_ret.name != "void":
                    raise TypeError("Return value required")
            else:
                vt = self.typeof(stmt.value)
                if not self.types_equal(vt, self.current_ret):
                    raise TypeError(f"Return type mismatch: expected {self.current_ret}, got {vt}")
        elif isinstance(stmt, ExprStmt):
            self.typeof(stmt.expr)

    def typeof(self, expr: Any) -> JType:
        if isinstance(expr, IntLiteral):
            # Infer based on value size? Default to i32
            return JType("i32")
        elif isinstance(expr, BoolLiteral):
            return JType("bool")
        elif isinstance(expr, StringLiteral):
            return JType("string")
        elif isinstance(expr, VarRef):
            if expr.name in self.locals:
                return self.locals[expr.name]
            raise TypeError(f"Undefined variable: {expr.name}")
        elif isinstance(expr, BinaryOp):
            lt = self.typeof(expr.left)
            rt = self.typeof(expr.right)
            if expr.op in ("+", "-", "*", "/", "%"):
                if not self.is_numeric(lt) or not self.is_numeric(rt):
                    raise TypeError(f"Arithmetic op requires numeric types: {lt}, {rt}")
                return lt
            elif expr.op in ("==", "!=", "<", ">", "<=", ">="):
                if not self.types_equal(lt, rt):
                    raise TypeError(f"Comparison requires matching types: {lt}, {rt}")
                return JType("bool")
            elif expr.op in ("&&", "||"):
                if lt.name != "bool" or rt.name != "bool":
                    raise TypeError(f"Logical op requires bool: {lt}, {rt}")
                return JType("bool")
            else:
                raise TypeError(f"Unknown binary op: {expr.op}")
        elif isinstance(expr, UnaryOp):
            t = self.typeof(expr.operand)
            if expr.op == "-":
                if not self.is_numeric(t):
                    raise TypeError("Unary - requires numeric type")
                return t
            elif expr.op == "!":
                if t.name != "bool":
                    raise TypeError("Unary ! requires bool")
                return JType("bool")
            else:
                raise TypeError(f"Unknown unary op: {expr.op}")
        elif isinstance(expr, CallExpr):
            if expr.name not in self.functions:
                raise TypeError(f"Undefined function: {expr.name}")
            sig = self.functions[expr.name]
            if len(sig) == 3:
                ptypes, ret, is_variadic = sig
            else:
                ptypes, ret = sig
                is_variadic = False
            min_args = len(ptypes)
            if len(expr.args) < min_args:
                raise TypeError(f"Function {expr.name} expects at least {min_args} args, got {len(expr.args)}")
            for i, (pt, arg) in enumerate(zip(ptypes, expr.args)):
                at = self.typeof(arg)
                if not self.types_equal(pt, at):
                    raise TypeError(f"Arg {i} to {expr.name}: expected {pt}, got {at}")
            return ret
        elif isinstance(expr, DerefExpr):
            t = self.typeof(expr.operand)
            if not t.is_pointer and t.name != "string":
                raise TypeError(f"Cannot dereference non-pointer: {t}")
            return JType(t.name, is_pointer=False)
        elif isinstance(expr, AddrOfExpr):
            t = self.typeof(expr.operand)
            if isinstance(expr.operand, VarRef):
                # Variable address -> pointer to variable type
                return JType(t.name, is_pointer=True)
            raise TypeError("Can only take address of variables")
        elif isinstance(expr, CastExpr):
            return expr.target_type
        elif isinstance(expr, IndexExpr):
            bt = self.typeof(expr.base)
            it = self.typeof(expr.index)
            if not self.is_numeric(it):
                raise TypeError("Index must be numeric")
            if bt.is_pointer or bt.name == "string":
                return JType(bt.name, is_pointer=False)
            raise TypeError(f"Cannot index type {bt}")
        else:
            raise TypeError(f"Unknown expression type: {type(expr).__name__}")

    def is_numeric(self, t: JType) -> bool:
        return t.name in ("i8", "i32", "i64")

    def types_equal(self, a: JType, b: JType) -> bool:
        return a.name == b.name and a.is_pointer == b.is_pointer
