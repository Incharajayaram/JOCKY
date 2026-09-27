# Generics/Templates Implementation Plan

## Current Status
✅ **Done:**
- Lexer support for `<` and `>` tokens
- Parser support for generic parameter parsing
- AST nodes with generic_params fields
- TypeVar class defined

❌ **TODO:**
1. Type checker - Handle generic type checking and monomorphization resolution
2. Code generator - Implement monomorphization (generate concrete versions)
3. Function call handling - Support `func::<T>(args)` syntax
4. Generic struct instantiation - Support `Pair::<i32>` syntax
5. Generic enum handling - Support `Result::<i32, string>`
6. Tests - Comprehensive test suite

---

## Implementation Phases

### Phase 1: Type Checker Enhancement (1 day)
**Goal:** Handle generic types during type checking

**Tasks:**
1. Add generic context tracking to type checker
   - Track in-scope type variables: `{T -> i32, U -> string}`
   - Maintain during function/struct checking

2. Implement `substitute_type(type: JType, context: Dict) -> JType`
   - Replace T with i32 when checking `max::<i32>(1, 2)`

3. Add `resolve_generic_function` to find matching overload
   - `max::<i32>` → find `max<T>` and create `max_i32` version

4. Track all monomorphizations for codegen

**Key Functions:**
```python
class GenericContext:
    type_bindings: Dict[str, JType]  # T -> i32
    
def substitute_type(jtype: JType, context: GenericContext) -> JType:
    if jtype.is_generic:
        return context.type_bindings.get(jtype.name, jtype)
    return jtype

def resolve_generic_call(name: str, type_args: List[JType]) -> str:
    # max::<i32> -> max_i32 (mangled name)
    return f"{name}_{'_'.join(t.name for t in type_args)}"
```

### Phase 2: Code Generator Enhancement (1-2 days)
**Goal:** Generate specialized code for each type instantiation

**Tasks:**
1. Implement monomorphization pass
   - For each `max::<i32>` call, generate specialized version
   - Generate `void max_i32(i32 a, i32 b) { ... }`

2. Update function codegen
   - When generating `max<T>`, track type bindings
   - Replace T references with actual type

3. Update struct codegen
   - For `Pair<i32>`, generate concrete struct definition
   - Handle field type substitution

4. Enum codegen for generics
   - For `Result<i32, string>`, generate concrete enum

**Key Functions:**
```python
def codegen_monomorphized_function(
    func_decl: FuncDecl,
    type_bindings: Dict[str, JType]
) -> str:
    # Generate specialized version with actual types
    mangled_name = resolve_generic_call(func_decl.name, type_bindings.values())
    # ... generate LLVM IR with substituted types

def monomorphize_all(ast: Program) -> List[Tuple[str, str]]:
    # Returns list of (mangled_name, llvm_ir) for all instantiations
```

### Phase 3: Parser Enhancement (1 day)
**Goal:** Support generic function/struct calls with type arguments

**Current:** Parser already handles `<T, U, V>` in declarations
**TODO:** 
1. Support `max::<i32>(1, 2)` syntax
2. Support `Pair::<i32> { first: 1, second: 2 }` syntax
3. Handle nested generics: `Option<Result<i32, string>>`

**Key Changes in Parser:**
```python
def parse_type_with_args(self) -> JType:
    # name<T1, T2, ...>
    base_type = self.parse_type()
    if self.peek().type == TokenType.LT:
        type_args = self.parse_type_args()  # <- new
        base_type.generic_args = type_args
    return base_type

def parse_type_args(self) -> List[JType]:
    # <i32, string, i64>
    args = []
    self.consume(TokenType.LT)
    while self.peek().type != TokenType.GT:
        args.append(self.parse_type())
        if self.peek().type != TokenType.GT:
            self.consume(TokenType.COMMA)
    self.consume(TokenType.GT)
    return args
```

### Phase 4: Testing (1 day)
**Goal:** Comprehensive test coverage

**Test Cases:**
1. Generic function with single type variable
   ```jocky
   fn id<T>(x: T) -> T { x }
   let a = id::<i32>(42);
   let b = id::<string>("hello");
   ```

2. Generic function with multiple type variables
   ```jocky
   fn swap<T, U>(a: T, b: U) -> Pair<U, T> { ... }
   ```

3. Generic structs
   ```jocky
   struct Pair<T> { first: T, second: T }
   let p = Pair::<i32> { first: 1, second: 2 };
   ```

4. Generic enums
   ```jocky
   enum Option<T> { Some(T), None }
   let x = Option::<i32>::Some(42);
   ```

5. Nested generics
   ```jocky
   fn contains<T>(list: Pair<Option<T>>, val: T) -> bool { ... }
   ```

---

## Type Binding Resolution Algorithm

```
When checking function call: max::<i32>(1, 2)

1. Find generic function: max<T>(a: T, b: T) -> T
2. Create type context: {T -> i32}
3. Check argument types:
   - arg1: 1 (literal i32) ✓
   - arg2: 2 (literal i32) ✓
4. Substitute return type:
   - T -> i32 ✓
5. Generate monomorphized name: max_i32
6. Schedule codegen for max_i32 version
```

## Monomorphization Strategy

### Option A: Eager (Early) Monomorphization
- When parsing generic calls, immediately generate concrete versions
- Pro: Simple, deterministic
- Con: May generate unused code

### Option B: Lazy Monomorphization
- Collect all generic call sites
- At codegen time, generate only needed versions
- Pro: No unused code
- Con: Requires separate pass

**Chosen:** Option B (Lazy) - Better code generation quality

## Implementation Order

1. **Day 1:** Type checker generic support
   - Generic context tracking
   - Type substitution algorithm
   - Monomorphization resolution

2. **Day 2:** Parser enhancements + codegen
   - Parse generic type arguments
   - Implement monomorphization codegen
   - LLVM IR generation for specialized functions

3. **Day 3:** Testing & debugging
   - Unit tests for each component
   - End-to-end tests
   - Edge cases and error handling

---

## Files to Modify

- `src/jocky/language/ast.py` - Add generic_args to JType
- `src/jocky/language/parser.py` - Parse generic type arguments
- `src/jocky/language/checker.py` - Generic type checking
- `src/jocky/language/codegen.py` - Monomorphization
- `tests/language/test_generics.py` - New test file

---

## Success Criteria

✅ Parse `fn max<T>(a: T, b: T) -> T { if a > b { a } else { b } }`  
✅ Call `max::<i32>(1, 2)` and get i32 result  
✅ Call `max::<i64>(1, 2)` and get i64 result  
✅ Generate different LLVM IR for each specialization  
✅ Handle generic structs: `Pair<T> { first: T, second: T }`  
✅ Handle generic enums: `Option<T> { Some(T), None }`  
✅ All tests passing  
✅ Feature count: 110+ / 129 (85%+)
