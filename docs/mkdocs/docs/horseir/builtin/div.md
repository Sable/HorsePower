# div

### Description

`div(x,y)`

- `x / y`

### Type Rules

High-level (Same as [mul](mul.md))

```no-highlight
Real    ,Real    -> MaxType
complex ,Real    -> complex
Real    ,complex -> complex
_       ,_       -> domain error
```

!!! tip "Note"
    1) `bool` is promoted to `i16` before any operation. <br>
    2) `div(0,0)` is an error (in APL, it returns 1)

Table with details (See [type alias](../types.md))

![division](../types/mul.png)

### Shape Rules

[Dyadic elementwise shape rules](../shapes.md#dyadic-elementwise)


### Examples

```no-highlight
div((-1,2,3):i32, 0.5:f32)
> (-2,4,6):f32
```
