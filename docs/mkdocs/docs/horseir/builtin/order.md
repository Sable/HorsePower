# order

### Description

```no-highlight
@order(x,b)
```

- ascending/descending (based on b) order of x, return indices
- asc (true) / desc (false)

### Type Rules

High-level

```no-highlight
Comparable, bool -> i64
list      , bool -> i64
_         , _    -> domain error
```

Table with details (See [type alias](../../../horseir/#types))

![order](../types/order.png)

### Shape Rules

[Shape left rules](../../../horseir/#shape-left)

### Examples

```no-highlight
    @order((32,12,7,78,23,45):i32, 1:bool)
(2,1,4,0,5,3):i64

    x = @list((32,12,7,78,12,45):i32,(56,34,32,45,78,23):i32);
    @order(x, (1,0):bool)
(2,1,4,0,5,3):i64
```
