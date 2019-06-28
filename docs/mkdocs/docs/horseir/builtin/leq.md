# leq

### Description

`@leq(x,y)`

- `x <= y`, return boolean

### Type Rules

High-level (Same as [lt](lt.md))

```no-highlight
Real  , Real   -> bool
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

Table with details (See [type alias](../../../horseir/#types))

![lt](../types/lt.png)

### Shape Rules

[Dyadic elementwise shape rules](../../../horseir/#dyadic-elementwise)

### Examples

```no-highlight
    @leq((-1,0,3):i32, 0:i32)
(1,1,0):bool
```
