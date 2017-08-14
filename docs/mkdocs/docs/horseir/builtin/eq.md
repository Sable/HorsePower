## eq

### Description

`eq(x,y)`

- `x = y`, return boolean

### Type rules

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

```
eq((-1,0,3), 0)
> (0,1,0):bool
```
