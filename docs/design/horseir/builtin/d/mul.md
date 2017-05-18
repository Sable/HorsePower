## mul

### Description

`mul(x,y)`

- $x \times y$

### Type rules

Pre-requisite
- Let TypeN = [bool, char, i16, i32, i64, f32, f64]; //a numerical type

```
TypeN1,TypeN2 -> (f32==max(TypeN1,TypeN2)?f32:f64)
_ -> domain error
```

**Note:** `bool` and `char` are promoted to `f32` or `f64` before any operation.

### Examples

```
mul(-1 2 3:i32, 0.5:f32)
> -0.5 1 1.5:f32
```
