## range

### Description

`range(x)`

- from `0 to x-1`, inclusive

### Type rules

```no-highlight
Int -> i64
_   -> domain error
```

### Examples

```no-highlight
range(3:i16)
> (0,1,2):i64
```
