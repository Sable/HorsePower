# eq

### Description

`@eq(x,y)`

- `x = y`, return boolean

### Type Rules

High-level

```no-highlight
Real   , Real    -> bool
complex, complex -> bool
sym    , sym     -> bool
str    , str     -> bool
month  , month   -> bool
date   , date    -> bool
dt     , dt      -> bool
hour   , hour    -> bool
second , second  -> bool
time   , time    -> bool
_      , _       -> domain error
```

!!! danger "Future Work"
    Support: (1) sym, str; (2) str, sym

Table with details (See [type alias](../../../horseir/#types))

![lt](../types/eq.png)

### Shape Rules

[Dyadic elementwise shape rules](../../../horseir/#dyadic-elementwise)

### Examples

```
    @eq((-1,0,3):i32, 0:i32)
(0,1,0):bool
```
