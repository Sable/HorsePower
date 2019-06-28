# outer

!!! Danger "Not Recommended"
    This is a naive function for outer product which may consume a lot of memory.

## Description

General outer product

```no-highlight
z:? = @outer(fn, x, y)
```

- `fn` is an arithmetic binary function (e.g. fn = @eq)

### Type Rules

```no-highlight
@eq, Real, Real -> list<bool>
_  , _   , _    -> domain error
```

!!! Danger "Future Work"
    1. Add more functions (`fn`) <br>
    2. Extend from the type Real to Basic

### Examples

- A general join is implemented with `@outer` and `@where`, see [join_index](join.md#join_index).

