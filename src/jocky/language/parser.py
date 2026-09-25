from .lexer import Token, TokenType, Lexer
from .ast import (
    Program, FuncDecl, FFIDecl, Param, Block,
    LetStmt, AssignStmt, IfStmt, WhileStmt, ForStmt,
    ReturnStmt, ExprStmt, BreakStmt, ContinueStmt,
    IntLiteral, BoolLiteral, StringLiteral, VarRef, NullLiteral,
    BinaryOp, UnaryOp, CallExpr, DerefExpr, AddrOfExpr,
    CastExpr, IndexExpr,
    StructDef, StructField, StructLiteralExpr, FieldAccessExpr,
    ArrayType, ArrayLiteralExpr,
    EnumDef, EnumVariant,
    TypeAlias,
    Pattern, WildcardPattern, LiteralPattern, VariantPattern, MatchArm, MatchExpr,
    UseStmt,
    JType,
)
from typing import Optional, Any, List

class ParseError(Exception):
    pass

class Parser:
    def __init__(self, tokens: List[Token]):
        self.tokens = tokens
        self.pos = 0

    def peek(self) -> Token:
        if self.pos < len(self.tokens):
            return self.tokens[self.pos]
        return self.tokens[-1]

    def advance(self) -> Token:
        tok = self.peek()
        self.pos += 1
        return tok

    def expect(self, ttype: TokenType, msg: str = "") -> Token:
        tok = self.peek()
        if tok.type != ttype:
            raise ParseError(f"Expected {ttype.name}, got {tok.type.name} ({tok.value}) at line {tok.line}{': ' + msg if msg else ''}")
        return self.advance()

    def match(self, *types: TokenType) -> bool:
        return self.peek().type in types

    def parse_type(self) -> JType:
        tok = self.peek()
        if tok.type == TokenType.I8:
            self.advance()
            t = JType("i8")
        elif tok.type == TokenType.I32:
            self.advance()
            t = JType("i32")
        elif tok.type == TokenType.I64:
            self.advance()
            t = JType("i64")
        elif tok.type == TokenType.BOOL:
            self.advance()
            t = JType("bool")
        elif tok.type == TokenType.VOID:
            self.advance()
            t = JType("void")
        elif tok.type == TokenType.STRING_KW:
            self.advance()
            t = JType("string")
        elif tok.type == TokenType.IDENT:
            # User-defined type (struct or enum)
            name = self.advance().value
            t = JType(name)
        else:
            raise ParseError(f"Expected type, got {tok.type.name} at line {tok.line}")

        # Array type
        if self.match(TokenType.LBRACKET):
            self.advance()
            size = 0
            if self.match(TokenType.NUMBER):
                size = self.advance().value
            self.expect(TokenType.RBRACKET)
            t = JType(t.name, is_array=True, array_size=size)

        # Pointer type
        while self.match(TokenType.STAR):
            self.advance()
            t = JType(t.name, is_pointer=True, is_array=t.is_array, array_size=t.array_size)

        return t

    def parse(self) -> Program:
        decls = []
        while not self.match(TokenType.EOF):
            decls.append(self.parse_decl())
        return Program(decls)

    def parse_decl(self) -> Any:
        if self.match(TokenType.USE):
            return self.parse_use_stmt()
        elif self.match(TokenType.FN):
            return self.parse_func_decl()
        elif self.match(TokenType.FFI):
            return self.parse_ffi_decl()
        elif self.match(TokenType.STRUCT):
            return self.parse_struct_decl()
        elif self.match(TokenType.ENUM):
            return self.parse_enum_decl()
        elif self.match(TokenType.TYPE):
            return self.parse_type_alias()
        else:
            raise ParseError(f"Unexpected token {self.peek().type.name} at line {self.peek().line}; expected use, fn, ffi, struct, enum, or type")

    def parse_func_decl(self) -> FuncDecl:
        self.expect(TokenType.FN)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.LPAREN)
        params = self.parse_params()
        self.expect(TokenType.RPAREN)
        ret_type = JType("void")
        if self.match(TokenType.ARROW):
            self.advance()
            ret_type = self.parse_type()
        body = self.parse_block()
        return FuncDecl(name, params, ret_type, body)

    def parse_ffi_decl(self) -> FFIDecl:
        self.expect(TokenType.FFI)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.LPAREN)
        params, variadic = self.parse_params_variadic()
        self.expect(TokenType.RPAREN)
        self.expect(TokenType.ARROW)
        ret_type = self.parse_type()
        self.expect(TokenType.SEMICOLON)
        return FFIDecl(name, params, ret_type, variadic)

    def parse_struct_decl(self) -> StructDef:
        self.expect(TokenType.STRUCT)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.LBRACE)
        fields = []
        while not self.match(TokenType.RBRACE):
            field_name = self.expect(TokenType.IDENT).value
            self.expect(TokenType.COLON)
            field_type = self.parse_type()
            fields.append(StructField(field_name, field_type))
            if self.match(TokenType.SEMICOLON):
                self.advance()
        self.expect(TokenType.RBRACE)
        self.expect(TokenType.SEMICOLON)
        return StructDef(name, fields)

    def parse_enum_decl(self) -> EnumDef:
        self.expect(TokenType.ENUM)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.LBRACE)
        variants = []
        while not self.match(TokenType.RBRACE):
            var_name = self.expect(TokenType.IDENT).value
            value = None
            fields = None

            # Check for tagged union fields: VariantName(field: Type, ...)
            if self.match(TokenType.LPAREN):
                self.advance()
                fields = []
                while not self.match(TokenType.RPAREN):
                    field_name = self.expect(TokenType.IDENT).value
                    self.expect(TokenType.COLON)
                    field_type = self.parse_type()
                    fields.append(StructField(field_name, field_type))
                    if self.match(TokenType.COMMA):
                        self.advance()
                self.expect(TokenType.RPAREN)
            # Or plain variant with value: VariantName = 42
            elif self.match(TokenType.EQ):
                self.advance()
                value = self.expect(TokenType.NUMBER).value

            variants.append(EnumVariant(var_name, value, fields))
            if self.match(TokenType.COMMA):
                self.advance()
        self.expect(TokenType.RBRACE)
        self.expect(TokenType.SEMICOLON)
        return EnumDef(name, variants)

    def parse_type_alias(self) -> TypeAlias:
        self.expect(TokenType.TYPE)
        name = self.expect(TokenType.IDENT).value
        self.expect(TokenType.EQ)
        target_type = self.parse_type()
        self.expect(TokenType.SEMICOLON)
        return TypeAlias(name, target_type)

    def parse_use_stmt(self) -> UseStmt:
        self.expect(TokenType.USE)
        # Parse module path: jocky.linux.modules
        parts = [self.expect(TokenType.IDENT).value]
        while self.match(TokenType.DOT):
            self.advance()
            parts.append(self.expect(TokenType.IDENT).value)
        module_path = ".".join(parts)
        self.expect(TokenType.SEMICOLON)
        return UseStmt(module_path)

    def parse_match_expr(self) -> MatchExpr:
        self.expect(TokenType.MATCH)
        scrutinee = self.parse_expr()
        self.expect(TokenType.LBRACE)
        arms = []
        while not self.match(TokenType.RBRACE):
            pattern = self.parse_pattern()
            self.expect(TokenType.ARROW_FAT)
            # Parse the arm body (could be a block or single expression)
            if self.match(TokenType.LBRACE):
                body = self.parse_block()
            else:
                # Single expression arm
                expr = self.parse_expr()
                body = Block([ExprStmt(expr)])
            arms.append(MatchArm(pattern, body))
            if self.match(TokenType.COMMA):
                self.advance()
        self.expect(TokenType.RBRACE)
        return MatchExpr(scrutinee, arms)

    def parse_pattern(self) -> Pattern:
        tok = self.peek()
        if tok.type == TokenType.IDENT:
            # Variant pattern or wildcard
            name = self.advance().value
            if name == "_":
                return WildcardPattern()
            # Check for tagged union field bindings: VariantName(x, y, ...)
            bindings = None
            if self.match(TokenType.LPAREN):
                self.advance()
                bindings = []
                while not self.match(TokenType.RPAREN):
                    binding_name = self.expect(TokenType.IDENT).value
                    bindings.append(binding_name)
                    if self.match(TokenType.COMMA):
                        self.advance()
                self.expect(TokenType.RPAREN)
            return VariantPattern(name, bindings)
        elif tok.type == TokenType.NUMBER:
            # Literal pattern
            val = self.advance().value
            return LiteralPattern(IntLiteral(val))
        elif tok.type == TokenType.TRUE:
            self.advance()
            return LiteralPattern(BoolLiteral(True))
        elif tok.type == TokenType.FALSE:
            self.advance()
            return LiteralPattern(BoolLiteral(False))
        else:
            raise ParseError(f"Expected pattern, got {tok.type.name} at line {tok.line}")

    def parse_params(self) -> List[Param]:
        params, _ = self.parse_params_variadic()
        return params

    def parse_params_variadic(self) -> (List[Param], bool):
        params = []
        variadic = False
        if self.match(TokenType.RPAREN):
            return params, variadic
        while True:
            if self.match(TokenType.ELLIPSIS):
                self.advance()
                variadic = True
                break
            name = self.expect(TokenType.IDENT).value
            self.expect(TokenType.COLON)
            ptype = self.parse_type()
            params.append(Param(name, ptype))
            if self.match(TokenType.COMMA):
                self.advance()
            else:
                break
        return params, variadic

    def parse_block(self) -> Block:
        self.expect(TokenType.LBRACE)
        stmts = []
        while not self.match(TokenType.RBRACE):
            stmts.append(self.parse_stmt())
        self.expect(TokenType.RBRACE)
        return Block(stmts)

    def parse_stmt(self) -> Any:
        if self.match(TokenType.LET):
            return self.parse_let_stmt()
        elif self.match(TokenType.IF):
            return self.parse_if_stmt()
        elif self.match(TokenType.WHILE):
            return self.parse_while_stmt()
        elif self.match(TokenType.FOR):
            return self.parse_for_stmt()
        elif self.match(TokenType.RETURN):
            return self.parse_return_stmt()
        elif self.match(TokenType.BREAK):
            self.advance()
            self.expect(TokenType.SEMICOLON)
            return BreakStmt()
        elif self.match(TokenType.CONTINUE):
            self.advance()
            self.expect(TokenType.SEMICOLON)
            return ContinueStmt()
        else:
            return self.parse_expr_or_assign_stmt()

    def parse_let_stmt(self) -> LetStmt:
        self.expect(TokenType.LET)
        name = self.expect(TokenType.IDENT).value
        typ: Optional[JType] = None
        if self.match(TokenType.COLON):
            self.advance()
            typ = self.parse_type()
        self.expect(TokenType.EQ)
        init = self.parse_expr()
        self.expect(TokenType.SEMICOLON)
        return LetStmt(name, typ, init)

    def parse_if_stmt(self) -> IfStmt:
        self.expect(TokenType.IF)
        cond = self.parse_expr()
        then_block = self.parse_block()
        else_block = None
        if self.match(TokenType.ELSE):
            self.advance()
            if self.match(TokenType.IF):
                # else-if chaining: wrap the nested if in a Block
                else_block = Block([self.parse_if_stmt()])
            else:
                else_block = self.parse_block()
        return IfStmt(cond, then_block, else_block)

    def parse_while_stmt(self) -> WhileStmt:
        self.expect(TokenType.WHILE)
        cond = self.parse_expr()
        body = self.parse_block()
        return WhileStmt(cond, body)

    def parse_for_stmt(self) -> ForStmt:
        self.expect(TokenType.FOR)
        self.expect(TokenType.LPAREN)
        init = self.parse_for_init()
        cond = self.parse_expr()
        self.expect(TokenType.SEMICOLON)
        step = self.parse_for_step()
        self.expect(TokenType.RPAREN)
        body = self.parse_block()
        return ForStmt(init, cond, step, body)

    def parse_for_init(self) -> Any:
        if self.match(TokenType.LET):
            self.expect(TokenType.LET)
            name = self.expect(TokenType.IDENT).value
            typ = None
            if self.match(TokenType.COLON):
                self.advance()
                typ = self.parse_type()
            self.expect(TokenType.EQ)
            init = self.parse_expr()
            self.expect(TokenType.SEMICOLON)
            return LetStmt(name, typ, init)
        elif self.match(TokenType.SEMICOLON):
            self.advance()
            return ExprStmt(IntLiteral(0))
        else:
            expr = self.parse_expr()
            self.expect(TokenType.SEMICOLON)
            return ExprStmt(expr)

    def parse_for_step(self) -> Any:
        if self.match(TokenType.RPAREN):
            return IntLiteral(0)
        expr = self.parse_expr()
        if self.match(TokenType.EQ):
            self.advance()
            rhs = self.parse_expr()
            return AssignStmt(expr, rhs)
        return expr

    def parse_return_stmt(self) -> ReturnStmt:
        self.expect(TokenType.RETURN)
        val = None
        if not self.match(TokenType.SEMICOLON):
            val = self.parse_expr()
        self.expect(TokenType.SEMICOLON)
        return ReturnStmt(val)

    def parse_expr_or_assign_stmt(self) -> Any:
        expr = self.parse_expr()
        if self.match(TokenType.EQ):
            self.advance()
            rhs = self.parse_expr()
            self.expect(TokenType.SEMICOLON)
            return AssignStmt(expr, rhs)
        # Match expressions consume their own braces, so no semicolon needed
        if not isinstance(expr, MatchExpr):
            self.expect(TokenType.SEMICOLON)
        return ExprStmt(expr)

    # ---- Expression parsing with precedence climbing ----
    # Precedence (low to high):
    #   || → && → | → ^ → & → == → < → << → + → * → unary

    def parse_expr(self) -> Any:
        return self.parse_or()

    def parse_or(self) -> Any:
        left = self.parse_and()
        while self.match(TokenType.OROR):
            op = self.advance().value
            right = self.parse_and()
            left = BinaryOp(op, left, right)
        return left

    def parse_and(self) -> Any:
        left = self.parse_bitor()
        while self.match(TokenType.ANDAND):
            op = self.advance().value
            right = self.parse_bitor()
            left = BinaryOp(op, left, right)
        return left

    def parse_bitor(self) -> Any:
        left = self.parse_bitxor()
        while self.match(TokenType.PIPE):
            op = self.advance().value
            right = self.parse_bitxor()
            left = BinaryOp(op, left, right)
        return left

    def parse_bitxor(self) -> Any:
        left = self.parse_bitand()
        while self.match(TokenType.CARET):
            op = self.advance().value
            right = self.parse_bitand()
            left = BinaryOp(op, left, right)
        return left

    def parse_bitand(self) -> Any:
        left = self.parse_eq()
        while self.match(TokenType.AMPERSAND):
            op = self.advance().value
            right = self.parse_eq()
            left = BinaryOp(op, left, right)
        return left

    def parse_eq(self) -> Any:
        left = self.parse_rel()
        while self.match(TokenType.EQEQ, TokenType.NEQ):
            op = self.advance().value
            right = self.parse_rel()
            left = BinaryOp(op, left, right)
        return left

    def parse_rel(self) -> Any:
        left = self.parse_shift()
        while self.match(TokenType.LT, TokenType.GT, TokenType.LE, TokenType.GE):
            op = self.advance().value
            right = self.parse_shift()
            left = BinaryOp(op, left, right)
        return left

    def parse_shift(self) -> Any:
        left = self.parse_add()
        while self.match(TokenType.LSHIFT, TokenType.RSHIFT):
            op = self.advance().value
            right = self.parse_add()
            left = BinaryOp(op, left, right)
        return left

    def parse_add(self) -> Any:
        left = self.parse_mul()
        while self.match(TokenType.PLUS, TokenType.MINUS):
            op = self.advance().value
            right = self.parse_mul()
            left = BinaryOp(op, left, right)
        return left

    def parse_mul(self) -> Any:
        left = self.parse_unary()
        while self.match(TokenType.STAR, TokenType.SLASH, TokenType.PERCENT):
            op = self.advance().value
            right = self.parse_unary()
            left = BinaryOp(op, left, right)
        return left

    def parse_unary(self) -> Any:
        if self.match(TokenType.MINUS, TokenType.BANG, TokenType.STAR,
                      TokenType.AMPERSAND, TokenType.TILDE):
            op = self.advance().value
            operand = self.parse_unary()
            if op == "*":
                return DerefExpr(operand)
            if op == "&":
                return AddrOfExpr(operand)
            return UnaryOp(op, operand)
        return self.parse_postfix()

    def parse_postfix(self) -> Any:
        node = self.parse_primary()
        while True:
            if self.match(TokenType.LPAREN):
                self.advance()
                args = self.parse_args()
                self.expect(TokenType.RPAREN)
                if isinstance(node, VarRef):
                    node = CallExpr(node.name, args)
                else:
                    raise ParseError(f"Cannot call non-identifier at line {self.peek().line}")
            elif self.match(TokenType.LBRACKET):
                self.advance()
                idx = self.parse_expr()
                self.expect(TokenType.RBRACKET)
                node = IndexExpr(node, idx)
            elif self.match(TokenType.DOT):
                self.advance()
                field_name = self.expect(TokenType.IDENT).value
                node = FieldAccessExpr(node, field_name)
            else:
                break
        return node

    def parse_args(self) -> List[Any]:
        args = []
        if self.match(TokenType.RPAREN):
            return args
        while True:
            args.append(self.parse_expr())
            if self.match(TokenType.COMMA):
                self.advance()
            else:
                break
        return args

    def parse_primary(self) -> Any:
        tok = self.peek()
        if tok.type == TokenType.NUMBER:
            self.advance()
            return IntLiteral(tok.value)
        elif tok.type == TokenType.STRING:
            self.advance()
            return StringLiteral(tok.value)
        elif tok.type == TokenType.TRUE:
            self.advance()
            return BoolLiteral(True)
        elif tok.type == TokenType.FALSE:
            self.advance()
            return BoolLiteral(False)
        elif tok.type == TokenType.NULL:
            self.advance()
            return NullLiteral()
        elif tok.type == TokenType.IDENT:
            self.advance()
            return VarRef(tok.value)
        elif tok.type == TokenType.MATCH:
            # Match expression: match expr { pattern => body, ... }
            return self.parse_match_expr()
        elif tok.type == TokenType.LBRACKET:
            # Array literal: [1, 2, 3]
            self.advance()
            elements = []
            if not self.match(TokenType.RBRACKET):
                while True:
                    elements.append(self.parse_expr())
                    if self.match(TokenType.COMMA):
                        self.advance()
                    else:
                        break
            self.expect(TokenType.RBRACKET)
            return ArrayLiteralExpr(elements)
        elif tok.type == TokenType.LPAREN:
            self.advance()
            # Check for cast: ( type ) expr
            saved_pos = self.pos
            try:
                typ = self.parse_type()
                if self.match(TokenType.RPAREN):
                    self.advance()
                    operand = self.parse_unary()
                    return CastExpr(typ, operand)
            except ParseError:
                pass
            self.pos = saved_pos
            expr = self.parse_expr()
            self.expect(TokenType.RPAREN)
            return expr
        else:
            raise ParseError(f"Unexpected token {tok.type.name} ({tok.value}) in expression at line {tok.line}")

def parse_source(source: str) -> Program:
    lexer = Lexer(source)
    tokens = lexer.tokenize()
    parser = Parser(tokens)
    return parser.parse()
