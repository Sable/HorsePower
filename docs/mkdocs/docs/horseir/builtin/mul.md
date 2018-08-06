## mul

### Description

`mul(x,y)`

- `x * y`, return floating-point values

### Type Rules

High-level

```no-highlight
Real_x,Real_y -> (f32==max(Real_x,Real_y)?f32:f64)
_             -> domain error
``` 

!!! tip "Note"
    Types `bool` and `char` are promoted to `f32` or `f64` before any operation.

Table with details (See [type alias](../types.md))

![multiplication](../types/mul.png)

### Shape Rules

[Dyadic elementwise shape rules](../shapes.md#dyadic-elementwise)

### Examples

```no-highlight
mul((-1,2,3):i32, 0.5:f32)
> (-0.5,1,1.5):f32
```
