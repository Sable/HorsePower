## unique

### Description

`unique(x)`

- unique value of x (in indices)

### Type rules

```no-highlight
bool   -> i64
i8     -> i64
i16    -> i64
i32    -> i64
i64    -> i64
f32    -> i64
f64    -> i64
sym    -> i64
str    -> i64
month  -> i64
date   -> i64
dt     -> i64
minute -> i64
second -> i64
time   -> i64
_      -> domain error
```

### Examples

```no-highlight
unique((-1,2,2,3,3):i16)
> (0,1,3):i64
```
