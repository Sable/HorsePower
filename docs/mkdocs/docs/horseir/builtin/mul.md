# mul

### Description

`@mul(x,y)`

- `x * y`, return floating-point values

### Type Rules

High-level

```no-highlight
Real    ,Real    -> MaxType
complex ,Real    -> complex
Real    ,complex -> complex
_       ,_       -> domain error
``` 

!!! tip "Note"
    Type `bool` is promoted to `i16` before any operation.

Table with details (See [type alias](../../../horseir/#types))

![multiplication](../types/mul.png)

!!! danger "Future Work"
    1) mul(i32, i32) should return `i64` <br>
    2) mul(i64, i64) should return `f64`

### Shape Rules

[Dyadic elementwise shape rules](../../../horseir/#dyadic-elementwise)

### Examples

```no-highlight
    @mul((-1,2,3):i32, 0.5:f32)
(-0.5,1,1.5):f32
```
