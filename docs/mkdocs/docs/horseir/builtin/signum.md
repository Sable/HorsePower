# signum

### Description

`@signum(x)`

- if `x < 0`, return `-1`
- else if `x > 0`, return `1`
- else return 0

### Type Rules

```no-highlight
bool -> i8
i8   -> i8
i16  -> i8
i32  -> i8
i64  -> i8
f32  -> i8
f64  -> i8
_    -> domain error
```

### Examples

```no-highlight
    @signum((-2,0,3):i32)
(-1,0,1):i8
```
