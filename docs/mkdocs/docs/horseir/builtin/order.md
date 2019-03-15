# order

### Description

```no-highlight
oder(x,b)
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

Table with details (See [type alias](../types.md))

![order](../types/order.png)

### Shape Rules

[Shape left rules](../shapes.md#shape-left)

### Examples

```no-highlight
order((32,12,7,78,23,45), 1:bool)
> (2,1,4,0,5,3):i64

order(((32,12,7,78,12,45),(56,34,32,45,78,23)), (1,0):bool)
> (2,1,4,0,5,3):i64
```
