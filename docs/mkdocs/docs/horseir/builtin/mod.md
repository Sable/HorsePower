## mod

### Description

Modulo

- `mod(divisor, dividend)`  (dividend | divisor)

### Type Rules

High-level

```no-highlight
Real, Real -> MaxType (Except: bool, bool -> i16)
_          -> domain error
```

Table with details (See [type alias](../types.md))

![mod](../types/mod.png)

### Shape Rules

[Dyadic elementwise shape rules](../shapes.md#dyadic-elementwise)

### Examples

```no-highlight
mod(2, (4,9,16))
> (0,1,0)
```
