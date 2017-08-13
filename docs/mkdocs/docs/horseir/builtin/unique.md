## unique

### Description

`unique(x)`

- unique value of x (in indices)

### Type rules

```no-highlight
bool -> i64
char -> i64
i16  -> i64
i32  -> i64
i64  -> i64
f32  -> i64
f64  -> i64
sym  -> i64
str  -> i64
m    -> i64
d    -> i64
z    -> i64
u    -> i64
v    -> i64
t    -> i64
_    -> domain error
```

### Examples

```no-highlight
unique(-1 2 2 3 3)
> 0 1 3
```
