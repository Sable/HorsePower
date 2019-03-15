# round

### Description

`round(x)`

- round the value of x

### Type Rules

```no-highlight
bool -> bool
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
round((5.4,5.6):f32)
> (5,6):f32
```

### External Links

- Function `round` in C, [link](http://www.cplusplus.com/reference/cmath/round/)
