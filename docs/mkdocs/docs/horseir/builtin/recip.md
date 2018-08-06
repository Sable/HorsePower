## recip

### Description

`recip(x)`

- reciprocal, divide(1,x)

### Type rules

```no-highlight
bool -> f32
char -> f32
i16  -> f32
i32  -> f32
i64  -> f64
f32  -> f32
f64  -> f64
_    -> domain error
```

### Examples

```no-highlight
recip((-2,4,10):i32)
> (-0.5,0.25,-0.1):f32
```
