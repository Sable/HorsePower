# floor

### Description

`floor(x)`

- floor the value of x

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
floor((-1.2,0,1.2):f32)
> (-2,0,1):f32
```

### External Links

- Function `floor` in C, [link](http://www.cplusplus.com/reference/cmath/floor/)

