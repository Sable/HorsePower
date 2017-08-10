## member

### Description

`member(x,y)`

- $x \in y$
- if x in y, return true
- else return false

### Type rules

Pre-requisite
- Let TypeN = [bool, char, i16, i32, i64, f32, f64]; //a numerical type

```
TypeN,TypeN -> bool
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
member((-1,2,3), (1,3,-1))
> (1,0,1):bool
```
