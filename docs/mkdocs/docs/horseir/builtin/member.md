## member

### Description

`member(x,y)`

- latex : `$x \in y$`
- if x in y, return true
- else return false

### Type Rules

High-level

```no-highlight
Real   , Real    -> bool
char   , char    -> bool
symbol , symbol  -> bool
string , string  -> bool
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
member((-1,2,3), (1,3,-1))
> (1,0,1):bool
```
