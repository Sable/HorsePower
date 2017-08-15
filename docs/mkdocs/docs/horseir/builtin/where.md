## where

### Description

`where(x)`

- return the indices of elements which point to the 'true' values in x

### Type rules

```no-highlight
bool -> i64
_    -> domain error
```

### Examples

```no-highlight
where((1,0,1,0):bool)
> (0,2):i64
```
