## where

### Description

`where(x)`

- return the indices of elements which point to the 'true' values in x

### Type rules

```no-highlight
bool       -> i64
list<bool> -> list<i64>
_          -> domain error
```

!!! Note "Extension"
    Treat a list<bool> as a boolean matrix, then return two vectors which
    contain the x- and y-index of true values separately.

### Examples

```no-highlight
where((1,0,1,0):bool)
> (0,2):i64
```
