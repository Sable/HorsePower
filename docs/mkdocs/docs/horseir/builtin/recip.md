## recip

### Description

`recip(x)`

- reciprocal, divide(1,x)

### Type rules

```no-highlight
bool -> f64
char -> f64
i16  -> f64
i32  -> f64
i64  -> f64
f32  -> f32
f64  -> f64
_    -> domain error
```

### Examples

```no-highlight
recip((-2,4,10))
> (-0.5,0.25,-0.1)
```
