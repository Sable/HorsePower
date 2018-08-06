## geq

### Description

`geq(x,y)`

- `x >= y`, return boolean

### Type Rules

High-level (Same as [lt](lt.md))

```no-highlight
Real  , Real   -> bool
char  , char   -> bool
sym   , sym    -> bool
str   , str    -> bool
month , month  -> bool
date  , date   -> bool
dt    , dt     -> bool
hour  , hour   -> bool
second, second -> bool
time  , time   -> bool
_     , _      -> domain error
```

Table with details (See [type alias](../types.md))

![lt](../types/lt.png)

### Shape Rules

[Dyadic elementwise shape rules](../shapes.md#dyadic-elementwise)

### Examples

```no-highlight
geq((-1,0,3):i32, 0:i32)
> (0,1,1):bool
```
