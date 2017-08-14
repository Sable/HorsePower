## minus

### Description

`minus(x,y)`

- `x - y`

### Type rules

```no-highlight
Real,Real -> maxType
m,Int     -> m
d,Int     -> d
z,Int     -> z
u,Int     -> u
v,Int     -> v
t,Int     -> t
_         -> domain error
```

**Note:** `bool` and `char` are promoted to `i16` before any operation.

### Examples

```no-highlight
minus((-1,2,3), 1)
> (-2,1,2)
```
