## desc

### Description

`desc(x)`

- descending order of x, return index
- single element list
- see advanced sort: [order](order.md)

*Note:* `desc == order(0:false)`

### Type rules

```
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

```
desc(6 4 5)
> 0 2 1
```
