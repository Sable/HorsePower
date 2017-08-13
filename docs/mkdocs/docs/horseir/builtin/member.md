## member

### Description

`member(x,y)`

- $x \in y$
- if x in y, return true
- else return false

### Type rules

```no-highlight
Real   , Real    -> bool
Char   , Char    -> bool
Sym    , Sym     -> bool
Complex, Complex -> bool
m      , m       -> bool
d      , d       -> bool
z      , z       -> bool
u      , u       -> bool
v      , v       -> bool
t      , t       -> bool
_      ,_        -> domain error
```

### Examples

```no-highlight
member((-1,2,3), (1,3,-1))
> (1,0,1):bool
```
