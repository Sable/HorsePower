## mod

### Description

Modulo

- `mod(divisor, dividend)`  (dividend | divisor)

### Type rules

```no-highlight
Real, Real -> maxType
_          -> domain error
```

### Examples

```no-highlight
mod(2, (4,9,16))
> (0,1,0)
```
