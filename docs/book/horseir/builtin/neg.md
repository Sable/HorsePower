## neg

### Description

`neg(x)`

- negative value of x

### Type rules

```
bool -> i64
i16 -> i16
i32 -> i32
i64 -> i64
f32 -> f32
f64 -> f64
_ -> domain error
```

### Examples

```
neg(-1 2 3)
> 1 -2 -3
```
