# A Collection of List-related Functions

### list

**Description**

```no-highlight
list(...)
```

- Return a list with a given arbitrary number of arguments

**Type Rules**

```no-highlight
(Any;...) -> list
```

**Examples**

```no-highlight
list(`a`b`c:sym,(4,5,6):i32)
> (`a`b`c:sym, (-1,2,3):i32):list
```

### enlist

**Description**

```no-highlight
enlist(x)
```

- Return a list enclose `x`
- Increase the depth of `x` by 1

**Type Rules**

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

**Examples**

```no-highlight
enlist((-1,2,3):i32)
> ((-1,2,3):i32):list
```

### raze

**Description**

```no-highlight
raze(x)
```

- Return a vector if all items in `x` can be safely placed in a vector
- All items in the list `x` are "freed"

**Type Rules**

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

**Examples**

```no-highlight
raze(enlist((-1,2,3):i32))
> (-1,2,3):i32
```

### tolist

**Description**

`tolist(x)`

- Converting a vector to a list
- Each item in the vector becomes a cell in the list

**Type Rules**

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

**Examples**

```no-highlight
tolist((-1,2,3))
> ((-1), (2), (3)):list<i64>
```

## each operations

- each(func, list): operate *func* on each cell in *list*
- each_item(func, list1, list2): operate *func* on each pair of cells in *list1* and *list2*
- each_left(func, list, item): operate *func* on each pair of each cell in *list* and *item* (as a whole)
- each_right(func, item, list): operate *func* on each pair of *item* (as a whole) and each cell in *list*

### each

```no-highlight
each(func, (a0,a1,...):list)  // operate func on each cell in the list
```

Result:

```no-highlight
(func(a0),func(a1),...):list
```

### each_item

```no-highlight
each_item(func,(a0,a1,...):list,(b0,b1,...):list) // operate func on each pair
```

Result:

```no-highlight
(func(a0,b0),func(a1,b1),...):list
```

### each_left

```no-highlight
each_left(func,(a0,a1,...):list,B) // pairs: {a0,B}, {a1,B}, ...
```

Result:

```no-highlight
(func(a0,B),func(a1,B),...):list
```

### each_right

```no-highlight
each_right(func,A,(b0,b1,...):list) // pairs: {A,b0}, {A,b1}, ...
```

Result:

```no-highlight
(func(A,b0),func(A,b1),...):list
```
