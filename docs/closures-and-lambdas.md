# Closures & Lambdas in JOCKY

## Overview

JOCKY now supports closures (anonymous functions) and lambda expressions for functional programming patterns.

## Syntax

### Basic Lambda

```jocky
let add = lambda(x: i32, y: i32) -> i32 {
    x + y
};

let result = add(5, 3);  // 8
```

### With Capture

Closures can capture variables from their enclosing scope:

```jocky
let factor = 2;
let multiply = lambda(x: i32) -> i32 {
    x * factor
};

let result = multiply(5);  // 10
```

### Nested Closures

```jocky
let outer = lambda(x: i32) -> fn(i32)->i32 {
    lambda(y: i32) -> i32 {
        x + y
    }
};

let add5 = outer(5);
let result = add5(3);  // 8
```

## Capture Modes

### By Value (Copy)
```jocky
let x = 10;
let closure = lambda() -> i32 { x };  // Captures x by value
x = 20;  // Doesn't affect closure
```

### By Reference
```jocky
let x: i32* = malloc(sizeof(i32));
let closure = lambda() -> i32 { *x };  // Captures x by reference
```

## Advanced Examples

### Higher-Order Functions

```jocky
fn map(arr: i32[10], f: fn(i32)->i32) -> i32[10] {
    let result: i32[10];
    for i in 0..10 {
        result[i] = f(arr[i]);
    }
    return result;
}

let numbers: i32[10] = [1, 2, 3, 4, 5];
let doubled = map(numbers, lambda(x: i32) -> i32 { x * 2 });
```

### Callbacks

```jocky
fn on_event(callback: fn(i32)->void) {
    callback(42);
}

on_event(lambda(x: i32) -> void {
    printf("Event: %d\n", x);
});
```

## Type Inference

JOCKY infers closure types from context:

```jocky
// Type is inferred as fn(i32, i32)->i32
let add = lambda(x, y) { x + y };

// Explicit type annotation
let add: fn(i32, i32)->i32 = lambda(x, y) { x + y };
```

## Performance

- **Zero-cost abstractions**: Closures are inferred to regular function calls when possible
- **Inlining**: Monomorphic closures are automatically inlined
- **No heap allocation** for stack-only captures

## Limitations

Current limitations:

- ⏳ Variadic closures (with `...`) - coming soon
- ⏳ Generic closures (`lambda<T>`) - coming soon
- ⏳ Method closures - coming soon
- ✅ Capture list customization (always by value or by reference)

## Compilation

Closures are compiled to:

1. **Regular functions** (when non-capturing)
2. **Nested function pointers** (when capturing by value)
3. **Closure structs** (when capturing by reference)

The JOCKY compiler optimizes these automatically.

---

See also: [Pattern Matching](./language-spec.md#pattern-matching), [Type System](./language-spec.md#type-system)
