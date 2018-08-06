## append

### Description

`append(x,y)`

- append all items in y to x
- if x is a list and y is not a list, y is enlisted and later appended
- if x is a list and y is a list, all cells in x and y are formed into a new list

### Type Rules

High-level

```no-highlight
Real   , Real   -> Real
complex, complex-> Clex
symbol , Sym    -> Sym
char   , char   -> Char
string , string -> Char
List   , Any    -> List
Any    , List   -> List
Enum   , Basic  -> Enum
Basic  , Enum   -> Enum
_      , _      -> domain error
```

Table with details (See [type alias](../types.md))

![any](../types/append.png)

### Shape Rules

[Shape append rules](../shapes.md#shape-append)

### Examples

```no-highlight
append(0, (-1,2,3))
> (0,-1,2,3):i32
```
