## where

### Description

`where(x)`

- return the indices of elements which have the value of 'truth' in x

### Type rules

```
bool -> H_L
_    -> domain error
```

### Examples

```
where(1 0 1 0)
> 0 2
```
