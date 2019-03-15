# append

### Description

```no-highlight
append(x,y)
```

Append all items in y to x and return a new list

- if x is a list and y is not a list, y is enlisted and later appended
- if x is a list and y is a list, all cells in y are concatenated to x

### Type Rules

High-level

```no-highlight
Real   , Real    -> MaxType
complex, complex -> complex
sym    , sym     -> sym
str    , str     -> str
list   , Any     -> list
Any    , list    -> list
enum   , Basic   -> enum
Basic  , enum    -> enum
_      , _       -> domain error
```

!!! note "Append with enum"
    append(enum<k0,v>, k1) is valid when 1) k0 and k1 are Real; or 2) k0 and k1 are the same type

Table with details (See [type alias](../types.md))

![any](../types/append.png)

### Shape Rules

[Shape append rules](../shapes.md#shape-append)

### Examples

```no-highlight
append(0:i32, (-1,2,3):i32)
> (0,-1,2,3):i32
```
