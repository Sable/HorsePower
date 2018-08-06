## order

### Description

`oder(x,y)`

- ascending/descending (based on y) order of x, return index
- asc == true
- desc == false

### Type Rules

High-level

```no-highlight
Comparable, bool -> int
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
