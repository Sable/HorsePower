## vector

### Description

`vector(x,y)`

- initialize a vector based on y with the length x

### Type Rules

High-level

```no-highlight
Int, Basic -> Basic
Int, List  -> List
_  , _     -> domain error
```

!!! tip "Note"
    `Int` on the left side must be a non-negative number.

Table with details (See [type alias](../types.md))

![vector](../types/vector.png)

### Shape Rules

[Shape determined by left value rules](../shapes.md#shape-left-value)

### Examples

```no-highlight
vector(2,(-1,2,3))
> (-1,2)
vector(5,(-1,2,3))
> (-1,2,3,-1,2)
```
