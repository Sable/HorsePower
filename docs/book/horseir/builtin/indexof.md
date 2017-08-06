## index_of

### Description

`index_of(x, y)`

- find items from y in x; and
- return indices, if found
- return the length of x, if not found

Note: index starts from 0

### Type rules

```
B, B -> L
H, H -> L
I, I -> L
L, L -> L
F, F -> L
E, E -> L
C, C -> L
S, S -> L
_, _ -> domain error
```

### Examples

```
index_of(4 9 16, 3 4)
> 3 0
```
