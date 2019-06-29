# minus

### Description

`@minus(x,y)`

- `x - y`

### Type Rules

```no-highlight
Real  ,Real   -> MaxType
month ,Int    -> month
date  ,Int    -> date
dt    ,Int    -> dt
hour  ,Int    -> hour
second,Int    -> second
time  ,Int    -> time
month ,month  -> i32
date  ,date   -> i32
dt    ,dt     -> i64
hour  ,hour   -> i32
second,second -> i32
time  ,time   -> i32
_     ,_      -> domain error
```

!!! tip "Note"
    Type `bool` is promoted to `i16` before any operation.

Table with details (See [type alias](../../../horseir/#types))

![minus](../types/minus.png)

### Shape Rules

[Dyadic elementwise shape rules](../../../horseir/#dyadic-elementwise)

### Examples

Real numbers

```no-highlight
    @minus((-1,2,3):i32, 1:i32)
(-2,1,2):i32
```

Date time

```no-highlight
    d1 = 2011.03.15:date
    d2 = 2011.03.16:date
    @minus(d1,d2)
1:i32
    t1 = 15:12:57.001:t
    t2 = 15:12:57.101:t
    @minus(t1,t2)
100:i32
```


