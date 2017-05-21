## div

### Description

`div(x,y)`

- $x \div y$

### Type rules

Pre-requisite
- Let TypeN = [bool, char, i16, i32, i64, f32, f64]; //a numerical type

```
TypeN1,TypeN2 -> (f32==max(TypeN1,TypeN2)?f32:f64)
_ -> domain error
```

**Note:**

1. `bool` and `char` are promoted to `f32` or `f64` before any operation.
2. `div(0,0)` is an error (in APL, it returns 1)

### Examples

```
div(-1 2 3:i32, 0.5:f32)
> -2 4 6:f32
```
