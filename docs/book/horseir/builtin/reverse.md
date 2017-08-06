## reverse

### Description

Reverse a list

### Type rules

```
B -> B
H -> H
I -> I
L -> L
F -> F
E -> E
C -> C
S -> S
X -> X
M -> M
D -> D
Z -> Z
U -> U
W -> W
T -> T
_ -> domain error
```

### Examples

```
reverse(4 9 16)
> 16 9 4
```

### Note

Optimization opportunities

1. In-place reverse (e.g. `x = reverse(x)`)

