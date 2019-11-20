# reverse

### Description

Reverse a vector

### Type Rules

```no-highlight
bool    -> bool
i8      -> i8
i16     -> i16
i32     -> i32
i64     -> i64
f32     -> f32
f64     -> f64
str     -> str
sym     -> sym
complex -> complex
month   -> month
date    -> date
dt      -> dt
minute  -> minute
second  -> second
time    -> time
_       -> domain error
```

### Examples

```no-highlight
    @reverse((4,9,16):i32)
(16,9,4):i32
```

### See Also

- [@flip](flip.md): a reversing function for lists

### Note

Optimization opportunities

1. In-place reverse (e.g. `x = @reverse(x)`)

