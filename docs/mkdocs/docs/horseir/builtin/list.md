# List related operations

## list

### Description

```no-highlight
list(...)
```

- Return a list with a given arbitrary number of arguments

### Type Rules

```no-highlight
(Any;...) -> list
```

### Examples

```no-highlight
list(`a`b`c:sym,(4,5,6):i32)
> (`a`b`c:sym, (-1,2,3):i32):list
```

## enlist

### Description

```no-highlight
enlist(x)
```

- Return a list enclose `x`
- Increase the depth of `x` by 1

### Type Rules

```no-highlight
bool   -> list<bool>
i8     -> list<i8>
i16    -> list<i16>     
i32    -> list<i32>
i64    -> list<i64>
f32    -> list<f32>
f64    -> list<f64>
complex-> list<complex>
sym    -> list<sym>
str    -> list<str>
month  -> list<month>
date   -> list<date>
dt     -> list<dt>
minute -> list<minute>
second -> list<second>
time   -> list<time>
list   -> list<list>
dict   -> list<dict>
enum   -> list<enum>
table  -> list<table>
ktable -> list<ktable>
_      -> domain error
```

### Examples

```no-highlight
enlist((-1,2,3):i32)
> ((-1,2,3):i32):list
```

## raze

### Description

```no-highlight
raze(x)
```

- Return a vector if all items in `x` can be safely placed in a vector
- All items in the list `x` are "freed"

### Type Rules

```no-highlight
list<bool>    -> bool
list<i8>      -> i8
list<i16>     -> i16
list<i32>     -> i32
list<i64>     -> i64
list<f32>     -> f32
list<f64>     -> f64
list<complex> -> complex
list<sym>     -> sym
list<str>     -> str
list<month>   -> month
list<date>    -> date
list<dt>      -> dt
list<minute>  -> minute
list<second>  -> second
list<time>    -> time
list<?>       -> ?
_    -> domain error
```

### Examples

```no-highlight
raze(enlist((-1,2,3):i32))
> (-1,2,3):i32
```

## tolist

### Description

`tolist(x)`

- Converting a vector to a list
- Each item in the vector becomes a cell in the list

### Type Rules

```no-highlight
bool   -> list<bool>
i8     -> list<i8>
i16    -> list<i16>     
i32    -> list<i32>
i64    -> list<i64>
f32    -> list<f32>
f64    -> list<f64>
complex-> list<complex>
sym    -> list<sym>
str    -> list<str>
month  -> list<month>
date   -> list<date>
dt     -> list<dt>
minute -> list<minute>
second -> list<second>
time   -> list<time>
_      -> domain error
```

### Examples

```no-highlight
tolist((-1,2,3))
> ((-1), (2), (3)):list<i64>
```

## each operations

- each(func, list): operate *func* on each element in *list*
- each_item(func, list1, list2): operate *func* on each pair of elements in *list1* and *list2*
- each_left(func, list1, list2): operate *func* on each pair of each element in *list1* and total of *list2*
- each_right(func, list1, list2): operate *func* on each pair of total of *list1* and each element in *list2*

### each

```no-highlight
each(func, [a0;a1;...])
```

Result:

```no-highlight
[func(a0);func(a1);...]
```

### each_item

```no-highlight
each_item(func,[a0;a1;...],[b0;b1;...])
```

Result:

```no-highlight
(func(a0,b0);func(a1,b1);...)
```

### each_left

```no-highlight
each_left(func,[a0;a1;...],[b0;b1;...])
```

Result:

```no-highlight
(func(a0,[b0;b1;...]),func(a1,[b0;b1;...]),...)
```

### each_right

```no-highlight
each_right(func,[a0;a1;...],[b0;b1;...])
```

Result:

```no-highlight
(func([a0;a1;...],b0),func([a0;a1;...],b1),...)
```
