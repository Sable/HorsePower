## log

### Description

Natural logarithm

- `log(e, number)`

### Type rules

```no-highlight
Real -> f32 / f64 / Complex
_    -> domain error
```


## log2

### Description

Logarithm

- `log(base, number)`

### Type rules

```no-highlight
Real, Real -> f32 / f64 / Complex
_          -> domain error
```

### Examples

```no-highlight
log(2, (4,9,16))
> (2,3,4):f64
```
