## power

### Description

Exponentiation

- `power(base, exponent)`

### Type rules

```no-highlight
Real, Real -> f32 / f64 / Complex
_          -> domain error
```

**Note**: The base and exponent can be either a real number or a complex number.

### Examples

```no-highlight
power((2,3,4), 2)
> (4,9,16):f64
```
