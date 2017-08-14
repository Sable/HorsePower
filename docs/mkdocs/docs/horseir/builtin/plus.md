## plus

### Description

`plus(x,y)`

- `x + y`

### Type rules

Pre-requisite
- Let TypeI = [bool, i16, i32, i64]; //an integer
- Let TypeN = [bool, char, i16, i32, i64, f32, f64]; //a numerical type

```no-highlight
Real,Real     -> maxType
m,Int | Int,m -> m
m,Int | Int,m -> m
d,Int | Int,d -> d
z,Int | Int,z -> z
u,Int | Int,u -> u
v,Int | Int,v -> v
t,Int | Int,t -> t
_             -> domain error
```

**Note:** `bool` and `char` are promoted to `i16` before any operation.

### Examples

```no-highlight
plus((-1,2,3), 1)
> (0,3,4)
```
