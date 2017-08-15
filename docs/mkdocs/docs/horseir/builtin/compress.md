## compress

### Description

`compress(bool_vector, values)`

- select from values based on the boolean vector (true values)
- the boolean vector and values have the same length

### Type rules

```no-highlight
bool, Basic -> Basic
_           -> domain error
```

### Examples

```no-highlight
compress((1,1,0):bool, (53,11,12))
> (53,11):i64
```

