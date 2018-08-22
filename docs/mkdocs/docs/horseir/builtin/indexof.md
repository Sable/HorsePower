## index_of

### Description

```no-highlight
index_of(x, y)
```

find each item from y in x:

1. return index, if <blue>found</blue>
1. return the length of x, if <red>not found</red>

*Note: Indexing starts from 0*

### Type Rules

High-level

```no-highlight
Real , Real -> i64
str  , str  -> i64
sym  , sym  -> i64
_    , _    -> domain error
```

!!! danger "Future Work"
    Support the search between three string types: char, symbol, and string

Table with details (See [type alias](../types.md))

![index of](../types/indexof.png)

### Shape Rules

[Shape right rules](../shapes.md#shape-right)

### Examples

```no-highlight
index_of((4,9,16), (3,4))
> (3,0):i64
```
