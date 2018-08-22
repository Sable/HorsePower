## plus

### Description

`plus(x,y)`

- `x + y`

### Type Rules

High-level

```no-highlight
Real  ,Real   -> MaxType
month ,Int    -> month
date  ,Int    -> date
dt    ,Int    -> dt
hour  ,Int    -> hour
second,Int    -> second
time  ,Int    -> time
Int   ,month  -> month
Int   ,date   -> date
Int   ,dt     -> dt
Int   ,hour   -> hour
Int   ,second -> second
Int   ,time   -> time
_             -> domain error
```

!!! tip "Note"
    Type `bool` is promoted to `i16` before any operation.

Table with details (See [type alias](../types.md))

![plus](../types/plus.png)

### Shape Rules

[Dyadic elementwise shape rules](../shapes.md#dyadic-elementwise)

### Examples

```no-highlight
plus((-1,2,3), 1)
> (0,3,4)
```
