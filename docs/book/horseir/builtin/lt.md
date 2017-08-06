## lt

### Description

`lt(x,y)`

- `x < y`, return boolean

### Type rules

```
Real,Real  -> bool
c,c        -> bool
sym,sym    -> bool
str,str    -> bool
m,m        -> bool
d,d        -> bool
z,z        -> bool
u,u        -> bool
v,v        -> bool
t,t        -> bool
_          -> domain error
```

### Examples

```
lt(-1 2 3, 0)
> 1 0 0:bool
```
