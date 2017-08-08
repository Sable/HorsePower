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

<hr>

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


