## ceil

### Description

`ceil(x)`

- ceiling the value of x

### Type rules

```no-highlight
bool -> bool
i16  -> i16
i32  -> i32
i64  -> i64
f32  -> f32
f64  -> f64
_    -> domain error
```

### Examples

```no-highlight
ceil((-1.2,0,1.2):f32)
> (-1,0,2):f32
```

### External Links

- Function `ceil` in C, [link](http://www.cplusplus.com/reference/cmath/ceil/)

