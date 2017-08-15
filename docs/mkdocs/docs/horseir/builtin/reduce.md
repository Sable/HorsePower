## reduce

### Description

Reduction

- min(x)
- max(x)

### Type rules

Case 1: size(x) == 0,

```no-highlight
B -> L (max/min value)
H -> L
I -> L
L -> L
F -> F
E -> E
_ -> domain error
```

Case 2: size(x) > 0

```no-highlight
B -> B
H -> H
I -> I
L -> L
F -> F
E -> E
_ -> domain error
```

### Examples

#### min

```no-highlight
min((4,9,16))
> 4
```

#### max

```no-highlight
min((4,9,16))
> 16
```
