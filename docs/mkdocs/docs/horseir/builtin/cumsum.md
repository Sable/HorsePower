# cumsum

### Description

`@cumsum(x)`

- cumulative result

### Type Rules

```no-highlight
bool -> i64
i8   -> i64
i16  -> i64
i32  -> i64
i64  -> i64
f32  -> f64
f64  -> f64
_    -> domain error
```

### Examples

```no-highlight
    @cumsum((1,2,3):i64)
(1,3,6):i64
```
