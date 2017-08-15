## order

### Description

`oder(x,y)`

- ascending/descending (based on y) order of x, return index
- asc == true
- desc == false

### Type rules

```no-highlight
bool, bool -> i64
i16 , bool -> i64
i32 , bool -> i64
i64 , bool -> i64
f32 , bool -> i64
f64 , bool -> i64
char, bool -> i64
sym , bool -> i64
str , bool -> i64
m   , bool -> i64
d   , bool -> i64
z   , bool -> i64
u   , bool -> i64
v   , bool -> i64
t   , bool -> i64
_   , _    -> domain error
```

### Examples

```no-highlight
order((32,12,7,78,23,45), 1:bool)
> (2,1,4,0,5,3):i64

order(((32,12,7,78,12,45),(56,34,32,45,78,23)), (1,0):bool)
> (2,1,4,0,5,3):i64
```
