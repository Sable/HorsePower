## pi

### Description

`pi(x)`

- return `pi * x`
- `pi = acos(-1.0)` in C

### Type rules

```no-highlight
bool    -> f32
i16     -> f32
i32     -> f32
i64     -> f64
f32     -> f32
f64     -> f64
complex -> complex
_       -> domain error
```

### Examples

```no-highlight
pi((0,1):i32)
> (0,3.1415926):f32
```
