## leq

### Description

`leq(x,y)`

- `x <= y`, return boolean

### Type rules

Pre-requisite

```no-highlight
Real,Real -> bool
sym ,sym  -> bool
str ,str  -> bool
m   ,m    -> bool
d   ,d    -> bool
z   ,z    -> bool
u   ,u    -> bool
v   ,v    -> bool
t   ,t    -> bool
_         -> domain error
```

### Examples

```no-highlight
leq((-1,0,3), 0)
> (1,1,0):bool
```
