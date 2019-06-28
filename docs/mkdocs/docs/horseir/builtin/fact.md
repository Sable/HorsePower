# fact

### Description

`fact(n)`

- Factorial

### Type Rules

See [type alias](../../functions/#1-type-alias)

```no-highlight
Int -> i64
_   -> domain error
```

### Shape Rules

[Monadic elementwise shape rules](../../../horseir/#monadic-elementwise)

### Examples

```no-highlight
    @fact((2,3,4):i16)
(2,6,24):i64
```
