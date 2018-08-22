## neq

### Description

`neq(x,y)`

- `x != y`, return boolean

### Type Rules

High-level (Same as [eq](eq.md))

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

Table with details (See [type alias](../types.md))

![lt](../types/eq.png)

### Shape Rules

[Dyadic elementwise shape rules](../shapes.md#dyadic-elementwise)

### Examples

```no-highlight
neq((-1,0,3), 0)
> (1,0,1):bool
```
