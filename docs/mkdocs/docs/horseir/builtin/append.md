## append

### Description

`append(x,y)`

- append all items in y to x
- if x is a list and y is not a list, y is enlisted and later appended
- if x is a list and y is a list, all cells in x and y are formed into a new list

### Type rules

```no-highlight
Real , Real   -> Real
Clex , Clex   -> Clex
Sym  , Sym    -> Sym
Char , Char   -> Char
Str  , Str    -> Char
List , _      -> List
_    , List   -> List
Enum , Basic  -> Enum
Basic, Enum   -> Enum
_    , _      -> domain error
```

### Examples

```no-highlight
append(0, (-1,2,3))
> (0,-1,2,3):i64
```
