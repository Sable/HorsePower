## minus

### Description

`minus(x,y)`

- `x - y`

### Type rules

```no-highlight
Real  ,Real   -> MaxType
month ,Int    -> month
date  ,Int    -> date
dt    ,Int    -> dt
hour  ,Int    -> hour
second,Int    -> second
time  ,Int    -> time
month ,month  -> i32
date  ,date   -> i32
dt    ,dt     -> i64
hour  ,hour   -> i32
second,second -> i32
time  ,time   -> i32
_         -> domain error
```

!!! tip "Note"
    Types `bool` and `char` are promoted to `i16` before any operation.

Table with details (See [type alias](../types.md))

![minus](../types/minus.png)

### Shape Rules

[Dyadic elementwise shape rules](../shapes.md#dyadic-elementwise)

### Examples

```no-highlight
minus((-1,2,3), 1)
> (-2,1,2)
```
