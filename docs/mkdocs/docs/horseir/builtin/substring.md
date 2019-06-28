# sub_string

### Description

```no-highlight
@sub_string(x, a, n);
```

- Return a substring with a given range [a,a+n)
- `n` is the length of the new substring

### Type Rules

```no-highlight
str, Int, Int -> str
_  , _  , _   -> domain error
```

!!! danger "Future Work"
    Support the symbol type, `sym`.

### Examples

- It is implemented with the combination of `@outer` and `@where`

