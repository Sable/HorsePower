## index_of

### Description

`index_of(x, y)`

- find items from y in x; and
- return indices, if found
- return the length of x, if not found

Note: index starts from 0

### Type Rules

High-level

```no-highlight
Real   , Real   -> I
char   , char   -> I
string , string -> I
symbol , symbol -> I
_, _ -> domain error
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
