## minus

### Description

`minus(x,y)`

- `x - y`

### Type rules

Pre-requisite
- Let TypeI = [bool, i16, i32, i64]; //an integer
- Let TypeN = [bool, char, i16, i32, i64, f32, f64]; //a numerical type

```
TypeN1,TypeN2 -> max(TypeN1, TypeN2)
m,TypeI -> m
d,TypeI -> d
z,TypeI -> z
u,TypeI -> u
v,TypeI -> v
t,TypeI -> t
_ -> domain error
```

**Note:** `bool` and `char` are promoted to `i16` before any operation.

### Examples

```
minus(-1 2 3, 1)
> -2 1 2
```
