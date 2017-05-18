## pi

### Description

`pi(x)`

- return $\pi \times x$
- $\pi = acos(-1.0)$ in C

### Type rules

```
bool -> f64
char -> f64
i16 -> f64
i32 -> f64
i64 -> f64
f32 -> f32
f64 -> f64
_ -> domain error
```

### Examples

```
pi(0 1)
> 0 3.1415...:f64
```
