# A Collection of Array Indexing Functions

!!! note "Array Indexing"
    Include **index** and **index_a**

## index

### Description

```no-highlight
@index(x,k)
```

- `x[k]`: fetch value from `x` with a given index `k`

### Type Rules

Basic types

```no-highlight
bool   , Int -> bool   
i8     , Int -> i8     
i16    , Int -> i16    
i32    , Int -> i32    
i64    , Int -> i64    
f32    , Int -> f32    
f64    , Int -> f64    
complex, Int -> complex
sym    , Int -> sym    
str    , Int -> str    
month  , Int -> month  
date   , Int -> date   
dt     , Int -> dt     
minute , Int -> minute 
second , Int -> second 
time   , Int -> time   
_      , _   -> domain error
```


### Examples

```no-highlight
    @index((-1,2,3):i32,(2,1):i32)
(3,2):i32
```

## index_a

```no-highlight
@index_a(x,k,m)
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
    @index_a(x,(2,1):i32,0:i32)
> (-1,0,0):i32  // value: x
```

