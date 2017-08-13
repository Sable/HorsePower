## round

### Description

`round(x)`

- round the value of x

### Type rules

```no-highlight
bool -> bool
i16  -> i16
i32  -> i32
i64  -> i64
f32  -> i64
f64  -> i64
_ -> domain error
```

### Examples

```no-highlight
round((5.4,5.6))
> (5,6):i64
```
