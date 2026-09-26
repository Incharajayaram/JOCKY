# JOCKY Language Extensions: Closures and Attributes

**Status:** Implementation in progress
**Priority:** Tier 2 (Medium)
**Effort:** 2-3 days
**Date Started:** 2026-09-26

## Overview

Two major language features being added to JOCKY:
1. **Closures/Lambdas** - Anonymous functions with capture
2. **Attributes/Decorators** - Compiler directives

---

## 1. Attributes/Decorators

### Syntax

```jocky
#[inline]
fn fast_function() -> i32 {
    return 42;
}

#[no_mangle]
fn exported_function() {
    // ...
}

#[packed(2)]
struct Point {
    x: i32;
    y: i32;
}

#[cold]
fn rare_path() {
    // ...
}
```

### Built-in Attributes

| Attribute | Target | Effect | LLVM |
|-----------|--------|--------|------|
| `#[inline]` | Function | Force inline | `alwaysinline` |
| `#[inline(never)]` | Function | Prevent inlining | `noinline` |
| `#[no_mangle]` | Function | Don't mangle name | Link as-is |
| `#[cold]` | Function | Rarely called | `cold` |
| `#[hot]` | Function | Frequently called | `hot` |
| `#[pure]` | Function | No side effects | readonly |
| `#[const]` | Function | Compile-time eval | consteval |
| `#[packed(n)]` | Struct | Align to n bytes | packed |
| `#[repr(C)]` | Struct | C-compatible layout | C layout |
| `#[naked]` | Function | No prologue/epilogue | naked |
| `#[target_feature(...)]` | Function | CPU features | target-features |

### Examples

#### Optimization Hints
```jocky
#[inline]
fn add(a: i32, b: i32) -> i32 {
    return a + b;
}

#[cold]
fn panic_handler(msg: string) {
    // Error handling
}
```

#### Linking Control
```jocky
#[no_mangle]
fn exported_api() -> i32 {
    return 0;  // Callable from C as 'exported_api()'
}
```

#### Struct Packing
```jocky
#[packed(1)]
struct Packed {
    a: i8;      // 1 byte
    b: i32;     // 4 bytes immediately after (no padding)
}  // Total: 5 bytes (vs 8 with normal alignment)
```

#### Platform-Specific
```jocky
#[target_feature(+avx2)]
fn vectorized_operation() {
    // AVX2 instructions available
}
```

### Implementation Plan

1. **Lexer:** Add HASH token ✅
2. **Parser:** Add parse_attributes() method
3. **AST:** Add Attribute nodes ✅
4. **Type Checker:** Validate attribute names
5. **Codegen:** Emit LLVM metadata

---

## 2. Closures/Lambdas

### Syntax

```jocky
// Basic closure
let double = |x: i32| -> i32 { return x * 2; };
let result = double(5);  // 10

// Without type annotation (inferred)
let add = |a, b| { return a + b; };

// Capturing by value
let x = 10;
let add_x = |n| { return n + x; };
result = add_x(5);  // 15

// Capturing by reference
let mut y = 20;
let inc_y = |&y_ref| { y_ref = y_ref + 1; };
```

### Capture Semantics

```jocky
let count = 0;

// Value capture (copy)
let by_val = || { return count; };

// Reference capture (borrow)
let by_ref = |&c| { return c + 1; };

// Mutable reference (unique borrow)
let mut_ref = |&mut m| { m = 10; };
```

### Closure Types

| Type | Syntax | Captures | Closure Trait |
|------|--------|----------|---------------|
| Value | `\| \|` | Copy values | `Fn` |
| Reference | `\|&x\|` | Immutable refs | `Fn` |
| Mutable ref | `\|&mut x\|` | Mutable refs | `FnMut` |
| Move | `move \| \|` | Move ownership | `FnOnce` |

### Examples

#### Simple Closure
```jocky
let numbers = [1, 2, 3, 4, 5];

// Transform: double each
let double = |x: i32| -> i32 { return x * 2; };
// Usage with higher-order function:
// map(numbers, double)
```

#### Capturing Environment
```jocky
fn make_adder(n: i32) -> (*fn(i32) -> i32) {
    let adder = |x| { return x + n; };
    return &adder;  // Capture n from outer scope
}

let add5 = make_adder(5);
// add5(10) = 15
```

#### Mutable Closure
```jocky
let mut sum = 0;

let accumulate = |&mut s, x: i32| {
    s = s + x;
};

// accumulate(&mut sum, 5);
// sum = 5
```

### Implementation Plan

1. **Lexer:** Add PIPE token ✅ (already exists)
2. **Parser:** 
   - parse_closure_expr() - `|params| body`
   - parse_capture_list() - Variable references
3. **AST:** Add ClosureExpr nodes ✅
4. **Type Checker:**
   - Infer closure type from captures
   - Validate closure parameters
   - Type inference for closure body
5. **Codegen:**
   - Create anonymous function
   - Capture environment as struct
   - Generate function pointer

### Type Inference

```
Closure: |x, y| { x + y }

Input types: unknown
Output type: inferred from body (i32 + i32 → i32)
Closure type: fn(i32, i32) -> i32
```

---

## Parser Implementation

### Attribute Parsing

```python
def parse_attribute(self) -> Attribute:
    self.expect(TokenType.HASH)
    self.expect(TokenType.LBRACKET)
    name = self.expect(TokenType.IDENT).value
    
    args = None
    if self.match(TokenType.LPAREN):
        self.advance()
        args = []
        while not self.match(TokenType.RPAREN):
            args.append(self.parse_expr())
            if self.match(TokenType.COMMA):
                self.advance()
        self.expect(TokenType.RPAREN)
    
    self.expect(TokenType.RBRACKET)
    return Attribute(name, args)

def parse_attributes(self) -> List[Attribute]:
    attrs = []
    while self.match(TokenType.HASH):
        attrs.append(self.parse_attribute())
    return attrs
```

### Closure Parsing

```python
def parse_closure_expr(self) -> ClosureExpr:
    self.expect(TokenType.PIPE)
    
    # Parse parameters
    params = []
    captures = []
    
    while not self.match(TokenType.PIPE):
        by_ref = False
        if self.match(TokenType.AMPERSAND):
            self.advance()
            by_ref = True
        
        name = self.expect(TokenType.IDENT).value
        captures.append(CaptureVar(name, by_ref))
        
        # Optional type annotation
        if self.match(TokenType.COLON):
            self.advance()
            param_type = self.parse_type()
            params.append(Param(name, param_type))
        else:
            # Infer type later
            params.append(Param(name, None))
        
        if self.match(TokenType.COMMA):
            self.advance()
    
    self.expect(TokenType.PIPE)
    
    # Parse return type
    ret_type = None
    if self.match(TokenType.ARROW):
        self.advance()
        ret_type = self.parse_type()
    
    # Parse body (expression or block)
    if self.match(TokenType.LBRACE):
        body = self.parse_block()
    else:
        body = self.parse_expr()
    
    return ClosureExpr(params, ret_type, captures, body)
```

---

## Type Checking

### Attribute Validation

```python
VALID_ATTRIBUTES = {
    'inline': {'targets': ['function'], 'args': 0},
    'no_mangle': {'targets': ['function'], 'args': 0},
    'cold': {'targets': ['function'], 'args': 0},
    'hot': {'targets': ['function'], 'args': 0},
    'pure': {'targets': ['function'], 'args': 0},
    'packed': {'targets': ['struct'], 'args': 1},
    # ...
}

def check_attribute(self, attr: Attribute, target_type: str):
    if attr.name not in VALID_ATTRIBUTES:
        raise TypeError(f"Unknown attribute: {attr.name}")
    
    spec = VALID_ATTRIBUTES[attr.name]
    if target_type not in spec['targets']:
        raise TypeError(f"{attr.name} only valid on {spec['targets']}")
    
    if len(attr.args or []) != spec['args']:
        raise TypeError(f"{attr.name} requires {spec['args']} arguments")
```

### Closure Type Inference

```python
def infer_closure_type(self, closure: ClosureExpr) -> FunctionType:
    # Infer parameter types
    param_types = []
    for param in closure.params:
        if param.type:
            param_types.append(param.type)
        else:
            # Infer from usage in body
            param_types.append(self.infer_param_type(param.name, closure.body))
    
    # Infer return type from body
    if closure.ret_type:
        ret_type = closure.ret_type
    else:
        ret_type = self.infer_expr_type(closure.body)
    
    return FunctionType(param_types, ret_type)
```

---

## Code Generation

### Attribute Codegen

```llvm
; #[inline]
define i32 @fast_fn() #0 {
  ...
}
attributes #0 = { alwaysinline }

; #[no_mangle]
define i32 @exported_fn() {
  ...
}

; #[cold]
define void @cold_fn() #1 {
  ...
}
attributes #1 = { cold }
```

### Closure Codegen

```llvm
; Closure: |x| { x * 2 }
; Generated as anonymous struct + function

%closure.1 = type { i32* }  ; Captured environment

define i32 @closure.1(i32 %x) {
  %result = mul i32 %x, 2
  ret i32 %result
}

; Usage:
%fn_ptr = bitcast i32 (i32)* @closure.1 to i8*
%result = call i32 @closure.1(i32 10)
```

---

## Examples

### Optimization
```jocky
#[inline]
fn matrix_multiply(a: *i32, b: *i32, n: i32) -> *i32 {
    // Hot path - should be inlined
}

#[cold]
fn handle_error(code: i32) {
    // Error handling - rarely called
}
```

### API Bindings
```jocky
#[no_mangle]
fn jocky_custom_function() -> i32 {
    return 42;
}  // Callable as C function: jocky_custom_function()
```

### Higher-Order Functions
```jocky
fn apply_twice(f: (*fn(i32) -> i32), x: i32) -> i32 {
    return f(f(x));
}

fn main() {
    let double = |n| { return n * 2; };
    let result = apply_twice(&double, 5);  // ((5 * 2) * 2) = 20
}
```

---

## Status & Next Steps

### Completed
- ✅ AST node definitions (Attribute, ClosureExpr, CaptureVar)
- ✅ Lexer tokenization (HASH, PIPE tokens)
- ✅ FuncDecl extended with attributes field

### In Progress
- [ ] Parser support for attributes
- [ ] Parser support for closures
- [ ] Type checking for attributes
- [ ] Type checking for closures

### TODO
- [ ] LLVM codegen for attributes
- [ ] LLVM codegen for closures
- [ ] Capture environment struct generation
- [ ] Higher-order function support
- [ ] Test suite (20+ tests)

### Timeline
- **Day 1:** Parser implementation
- **Day 2:** Type checking
- **Day 3:** Code generation & testing

---

## Testing Plan

```python
# test_attributes.py
def test_inline_attribute():
    code = """
    #[inline]
    fn add(a: i32, b: i32) -> i32 {
        return a + b;
    }
    """
    # Verify attribute is parsed
    # Verify LLVM metadata is generated

def test_closure_basic():
    code = """
    let double = |x: i32| -> i32 { return x * 2; };
    """
    # Type: fn(i32) -> i32
    # Can be called as double(5)

def test_closure_capture():
    code = """
    let n = 10;
    let add_n = |x| { return x + n; };
    """
    # Captures n from outer scope
    # add_n(5) = 15
```

---

## References

- [Rust Closures](https://doc.rust-lang.org/book/ch13-01-closures.html)
- [Rust Attributes](https://doc.rust-lang.org/reference/attributes.html)
- [LLVM Function Attributes](https://llvm.org/docs/LangRef/#function-attributes)
- [C++ Lambda Expressions](https://en.cppreference.com/w/cpp/language/lambda)
