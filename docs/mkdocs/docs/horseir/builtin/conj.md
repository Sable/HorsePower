# conj

### Description

`@conj(x)`

- conjugate value of x (complex numbers)

### Type Rules

```no-highlight
complex -> complex
_       -> domain error
```

### Examples

```no-highlight
    @conj((1+2i,2-3i):complex)
(1-2i,2+3i):complex
```
