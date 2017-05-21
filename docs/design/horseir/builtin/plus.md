## plus

### Description

`plus(x,y)`

- `x + y`

### Type rules

Pre-requisite
- Let TypeI = [bool, i16, i32, i64]; //an integer
- Let TypeN = [bool, char, i16, i32, i64, f32, f64]; //a numerical type

```
TypeN1,TypeN2 -> max(TypeN1, TypeN2)
m,TypeI | TypeI,m -> m
d,TypeI | TypeI,d -> d
z,TypeI | TypeI,z -> z
u,TypeI | TypeI,u -> u
v,TypeI | TypeI,v -> v
t,TypeI | TypeI,t -> t
_ -> domain error
```

**Note:** `bool` and `char` are promoted to `i16` before any operation.

### Examples

```
plus(-1 2 3, 1)
> 0 3 4
```
