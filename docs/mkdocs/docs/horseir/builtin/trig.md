# A Collection of trig and hyper Functions

### List of Functions

Trigonometric functions

- cos
- sin
- tan

Inverse trigonometric functions

- acos
- asin
- atan

Hyperbolic functions

- cosh
- sinh
- tanh

Inverse hyperbolic functions

- acosh
- asinh
- atanh

### Type Rules

Without complex numbers

```no-highlight
bool -> f32
i8   -> f32
i16  -> f32
i32  -> f32
i64  -> f64
f32  -> f32
f64  -> f64
_    -> domain error
```

With complex numbers

```no-highlight
bool    -> f32
i8      -> f32 / complex
i16     -> f32 / complex
i32     -> f32 / complex
i64     -> f64 / complex
f32     -> f32 / complex
f64     -> f64 / complex
complex -> f64 / complex
_       -> domain error
```

### Table of trig Functions (ELI)

`trig(n,x)`

- trigonometric functions
- `n` is an integer

| n              | x                | n              | x                              |
| :------------- | :--------------- | :------------- | :------------------------------|
|                |                  | 0              | \( \sqrt{1-x^2} \)             |
| -1             |  arcsin(x)       | 1              | sine(x)                        |
| -2             |  arccos(x)       | 2              | cosine(x)                      |
| -3             |  arctan(x)       | 3              | tangent(x)                     |
| -4             | \(\sqrt{-1+x^2}\)| 4              | \( \sqrt{1+x^2} \)             |
| -5             |  arcsinh(x)      | 5              | sinh(x)                        |
| -6             |  arccosh(x)      | 6              | cosh(x)                        |
| -7             |  arccosh(x)      | 7              | tanh(x)                        |
| -8             |  -trig(8,x)      | 8              | \(-\sqrt{-1-x^2}\) for \(x \ge 0\) |
|                |                  |                | \(\sqrt{-1-x^2}\) for \(x \lt 0\)  |
| -9             |  x               | 9              | real(x)                        |
| -10            |  +x              | 10             | absolute(x)                    |
| -11            |  \(x\cdot i\)    | 11             | imaginary(x)                   |
| -12            |  \(e^{x\cdot i}\)|                |                                |
