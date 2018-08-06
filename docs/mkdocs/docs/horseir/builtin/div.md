## div

### Description

`div(x,y)`

- `x / y`

### Type Rules

High-level (Same as [mul](mul.md))

```no-highlight
Realx,Realy -> (f32==max(Realx,Realy)?f32:f64)
_           -> domain error
```

!!! tip "Note"
    1) `bool` and `char` are promoted to `f32` or `f64` before any operation. <br>
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
