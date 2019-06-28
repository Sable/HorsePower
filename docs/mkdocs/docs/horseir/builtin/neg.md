# neg

### Description

`@neg(x)`

- negative value of x

### Type Rules

```no-highlight
bool -> i16
i8   -> i8
i16  -> i16
i32  -> i32
i64  -> i64
f32  -> f32
f64  -> f64
_    -> domain error
```

### Examples

```no-highlight
    @neg((-1,2,3):i32)
(1,-2,-3):i32
```
