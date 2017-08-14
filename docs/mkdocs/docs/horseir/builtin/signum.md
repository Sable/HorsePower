## signum

### Description

`signum(x)`

- if `x < 0`, return `-1`
- else if `x > 0`, return `1`
- else return 0

### Type rules

```no-highlight
bool -> i64
i16  -> i16
i32  -> i32
i64  -> i64
f32  -> i64
f64  -> i64
_    -> domain error
```

### Examples

```no-highlight
signum((-2,0,3))
> (-1,0,1)
```
