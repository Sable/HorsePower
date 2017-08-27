## group

### Description

`group(x)`

- group items/pairs in x
- return indices

### Type rules

```no-highlight
bool    -> list<i64>
i16     -> list<i64>
i32     -> list<i64>
i64     -> list<i64>
f32     -> list<i64>
f64     -> list<i64>
sym     -> list<i64>
str     -> list<i64>
complex -> list<i64>
m       -> list<i64>
d       -> list<i64>
z       -> list<i64>
u       -> list<i64>
w       -> list<i64>
t       -> list<i64>
list    -> list<i64>
_    -> domain error
```

### Examples

```no-highlight
group((-1,3,2,3))
> [(0),(2),(1,3)]:list
```
