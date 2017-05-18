## eq

### Description

`eq(x,y)`

- `x = y`, return boolean

### Type rules

Pre-requisite
- Let TypeN = [bool, char, i16, i32, i64, f32, f64]; //a numerical type

```
TypeN,TypeN -> bool
sym,sym -> bool
str,str -> bool
m,m -> bool
d,d -> bool
z,z -> bool
u,u -> bool
v,v -> bool
t,t -> bool
_ -> domain error
```

### Examples

```
eq(-1 0 3, 0)
> 0 1 0:bool
```
