## mul

### Description

`mul(x,y)`

- `x * y`, return floating-point values

### Type rules

```no-highlight
Realx,Realy -> (f32==max(Realx,Realy)?f32:f64)
_           -> domain error
```

**Note:** `bool` and `char` are promoted to `f32` or `f64` before any operation.

### Examples

```no-highlight
mul((-1,2,3):i32, 0.5:f32)
> (-0.5,1,1.5):f32
```
