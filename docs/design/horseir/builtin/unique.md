## unique

### Description

`unique(x)`

- unique value of x

### Type rules

```
bool -> bool
char -> char
i16  -> i16
i32  -> i32
i64  -> i64
f32  -> f32
f64  -> f64
sym  -> sym
str  -> str
m    -> m
d    -> d
z    -> z
u    -> u
v    -> v
t    -> t
_    -> domain error
```

### Examples

```
unique(-1 2 2 3 3)
> -1 2 3
```
