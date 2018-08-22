!!! note "Array Indexing"
    Include **index** and **index_a**

## index

### Description

```no-highlight
index(x,k)
```

- `x[k]`: fetch value from `x` with a given index `k`

### Type Rules

Basic types

```no-highlight
bool   , (Int|list<Int>) -> (bool   |list<bool   >)
i8     , (Int|list<Int>) -> (i8     |list<i8     >)
i16    , (Int|list<Int>) -> (i16    |list<i16    >)
i32    , (Int|list<Int>) -> (i32    |list<i32    >)
i64    , (Int|list<Int>) -> (i64    |list<i64    >)
f32    , (Int|list<Int>) -> (f32    |list<f32    >)
f64    , (Int|list<Int>) -> (f64    |list<f64    >)
complex, (Int|list<Int>) -> (complex|list<complex>)
sym    , (Int|list<Int>) -> (sym    |list<sym    >)
str    , (Int|list<Int>) -> (str    |list<str    >)
month  , (Int|list<Int>) -> (month  |list<month  >)
date   , (Int|list<Int>) -> (date   |list<date   >)
dt     , (Int|list<Int>) -> (dt     |list<dt     >)
minute , (Int|list<Int>) -> (minute |list<minute >)
second , (Int|list<Int>) -> (second |list<second >)
time   , (Int|list<Int>) -> (time   |list<time   >)
list<?>, Int             -> list<?>
_      , _               -> domain error
```


### Examples

```no-highlight
    index((-1,2,3):i32,(2,1):i32)
> (3,2):i32
```

## index_a

```no-highlight
index_a(x,k,m)
```

- is equivalent to `x[k]=m`
- returns the same type of m

### Type Rules

Basic types (x,k,m -> x)

```no-highlight
bool   , Int , bool 
i8     , Int , (bool|i8)
i16    , Int , (bool|i8|i16)
i32    , Int , (bool|i8|i16|i32)
i64    , Int , (bool|i8|i16|i32|64)
f32    , Int , (bool|i8|i16|i32|64|f32)
f64    , Int , (bool|i8|i16|i32|64|f32|f64)
complex, Int , (bool|i8|i16|i32|64|f32|f64|complex)
sym    , Int , sym    
str    , Int , str    
month  , Int , month  
date   , Int , date   
dt     , Int , dt     
minute , Int , minute 
second , Int , second 
time   , Int , time   
```

**Note:**

- Think about indexing with *lists*.
- IndexA can't be parallelized because of the duplicated values of the array indices

### Examples

```no-highlight
    x:i32 = (-1,2,3):i32
    index_a(x,(2,1):i32,0:i32)
> (-1,0,0):i32  // value: x
```

