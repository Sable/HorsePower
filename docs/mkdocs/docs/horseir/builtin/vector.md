# vector

### Description

```no-highlight
@vector(n,x)
```

- initialize a vector based on `x` with the length `n`

### Type Rules

High-level

```no-highlight
Int, Basic -> Basic
Int, list  -> list
_  , _     -> domain error
```

!!! tip "Note"
    `Int` on the left side must be a non-negative number.

Table with details (See [type alias](../../../horseir/#types))

![vector](../types/vector.png)

### Shape Rules

[Shape determined by left value rules](../../../horseir/#shape-left-value)

### Examples

```no-highlight
    @vector(2:i32,(-1,2,3):i32)
(-1,2):i32
    @vector(5:i32,(-1,2,3):i32)
(-1,2,3,-1,2):i32
```
