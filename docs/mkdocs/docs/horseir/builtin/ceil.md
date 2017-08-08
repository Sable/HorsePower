## ceil

### Description

`ceil(x)`

- ceiling the value of x

### Type rules

```
bool -> bool
i16 -> i16
i32 -> i32
i64 -> i64
f32 -> i64
f64 -> i64
_ -> domain error
```

### Examples

```
ceil(-1.2 0 1.2)
> -1 0 2
```
