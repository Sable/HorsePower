# index_of

### Description

```no-highlight
@index_of(x, y)
```

find each item from x in y:

1. return index, if <blue>found</blue>
1. return the length of y, if <red>not found</red>

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

Table with details (See [type alias](../../../horseir/#types))

![index of](../types/indexof.png)

### Shape Rules

[Shape left rules](../../../horseir/#shape-left)

### Examples

```no-highlight
    @index_of((3,4):i32, (4,9,16):i32)
(3,0):i32
```
