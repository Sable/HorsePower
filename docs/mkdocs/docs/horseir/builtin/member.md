## member

### Description

```no-highlight
member(x,y)
```

Equivalent to \( x \in y \)

- if x in y, return true
- otherwise, return false

### Type Rules

High-level

```no-highlight
Real   , Real    -> bool
sym    , sym     -> bool
str    , str     -> bool
complex, complex -> bool
month  , month   -> bool
date   , date    -> bool
dt     , dt      -> bool
hour   , hour    -> bool
second , second  -> bool
time   , time    -> bool
_      , _       -> domain error
```

Table with details (See [type alias](../types.md))

![member](../types/member.png)

### Shape Rules

[Shape left rules](../shapes.md#shape-left)

### Examples

```no-highlight
member((-1,2,3):i32, (1,3,-1):i32)
> (1,0,1):bool
```
