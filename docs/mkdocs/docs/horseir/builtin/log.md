## log

### Description

Natural logarithm

- `log(value)`
- base is `e`

### Type rules

```no-highlight
bool -> f32
i8   -> f32 / complex
i16  -> f32 / complex
i32  -> f32 / complex
i64  -> f64 / complex
f32  -> f32 / complex
f64  -> f64 / complex
_    -> domain error
```


## log2

### Description

Logarithm

- `log(value)`
- base is 2

### Type rules

```no-highlight
bool -> f32
i8   -> f32 / complex
i16  -> f32 / complex
i32  -> f32 / complex
i64  -> f64 / complex
f32  -> f32 / complex
f64  -> f64 / complex
_    -> domain error
```

### Examples

```no-highlight
log2((4,9,16):i32)
> (2,3,4):f32
```

## log10

### Description

Logarithm

- `log10(value)`
- base is 10

### Type rules

```no-highlight
bool -> f32
i8   -> f32 / complex
i16  -> f32 / complex
i32  -> f32 / complex
i64  -> f64 / complex
f32  -> f32 / complex
f64  -> f64 / complex
```

### Examples

```no-highlight
log10((10,100,1000):i32)
> (1,2,3):f32
```

## logb

### Description

Natural logarithm

- `logb(base, number)`

### Type rules

High-level (without complex)

```no-highlight
Real , Real -> MaxType -> bool -> f32
                         i8   -> f32
                         i16  -> f32
                         i32  -> f32
                         i64  -> f64
                         f32  -> f32
                         f64  -> f64
_    , _    -> domain error
```

!!! note "Related"
    The same rules in the function [power](power.md#power).


