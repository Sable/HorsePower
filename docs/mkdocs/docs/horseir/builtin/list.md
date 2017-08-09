# List related operations

## enlist

### Description

`enlist(x)`

- Return a list enclose `x`
- Increase the depth of `x` by 1

### Type rules

```no-highlight
bool   -> list
i16    -> list 
i32    -> list 
i64    -> list 
f32    -> list 
f64    -> list 
complex-> list 
sym    -> list 
m      -> list
d      -> list
z      -> list
u      -> list
v      -> list
t      -> list
list   -> list
dict   -> list
enum   -> list
table  -> list
ktable -> list
_      -> domain error
```

### Examples

```no-highlight
enlist(-1 2 3)
> (-1 2 3):list
```

## raze

### Description

`raze(x)`

- Return a vector if all items in `x` can be safely placed in a vector
- All items in the list `x` are "freed"

### Type rules

```no-highlight
list -> bool
list -> i16
list -> i32
list -> i64
list -> f32
list -> f64
list -> complex
list -> sym
list -> m
list -> d
list -> z
list -> u
list -> v
list -> t
_    -> domain error
```

### Examples

```no-highlight
raze(enlist(-1 2 3))
> -1 2 3
```

## each operations

- each(func, list): operate *func* on each element in *list*
- eachitem(func, list1, list2): operate *func* on each pair of elements in *list1* and *list2*
- eachleft(func, list1, list2): operate *func* on each pair of each element in *list1* and total of *list2*
- eachright(func, list1, list2): operate *func* on each pair of total of *list1* and each element in *list2*

### each

```no-highlight
each(func, [a0;a1;...])
```

Result:

```no-highlight
[func(a0);func(a1);...]
```

### eachitem

```no-highlight
eachitem(func,[a0;a1;...],[b0;b1;...])
```

Result:

```no-highlight
(func(a0,b0);func(a1,b1);...)
```

### eachleft

```no-highlight
func([a0;a1;...],[b0;b1;...])
```

Result:

```no-highlight
(func(a0,[b0;b1;...]),func(a1,[b0;b1;...]),...)
```

### eachright

```no-highlight
func([a0;a1;...],[b0;b1;...])
```

Result:

```no-highlight
(func([a0;a1;...],b0),func([a0;a1;...],b1),...)
```
