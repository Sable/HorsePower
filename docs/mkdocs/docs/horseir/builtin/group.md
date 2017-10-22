## group

### Description

`group(x)`

- group items/pairs in x
- return a dict: unique key indices and list key indices

### Type rules

```no-highlight
bool    -> dict<i64, list<i64>>
i16     -> dict<i64, list<i64>>
i32     -> dict<i64, list<i64>>
i64     -> dict<i64, list<i64>>
f32     -> dict<i64, list<i64>>
f64     -> dict<i64, list<i64>>
sym     -> dict<i64, list<i64>>
str     -> dict<i64, list<i64>>
complex -> dict<i64, list<i64>>
m       -> dict<i64, list<i64>>
d       -> dict<i64, list<i64>>
z       -> dict<i64, list<i64>>
u       -> dict<i64, list<i64>>
w       -> dict<i64, list<i64>>
t       -> dict<i64, list<i64>>
list    -> dict<i64, list<i64>>
_    -> domain error
```

### Examples

```no-highlight
group((-1,3,2,3))
> [0:0,1:(1,3),2:2]:dict
```
