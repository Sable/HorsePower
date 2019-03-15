# len

### Description

```no-highlight
len(x)
```

- if x is a table, return the number of rows of the table;
  otherwise, return the length of x
- `x` can be any type

### Type Rules

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
complex-> i64
list   -> i64
enum   -> i64
dict   -> i64
table  -> i64
ktable -> i64
_      -> domain error
```

### Examples

```no-highlight
len((-1,2,3):f32)
> 3:i64
```
