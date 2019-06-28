# mod

### Description

Modulo

- `@mod(divisor, dividend)`  (dividend | divisor)

### Type Rules

High-level

```no-highlight
Real, Real -> MaxType (Except: bool, bool -> i16)
_          -> domain error
```

Table with details (See [type alias](../../../horseir/#types))

![mod](../types/mod.png)

### Shape Rules

[Dyadic elementwise shape rules](../../../horseir/#dyadic-elementwise)

### Examples

```no-highlight
    @mod(2:i32, (4,9,16):i32)
(0,1,0):i32
```
