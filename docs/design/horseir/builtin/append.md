## append

### Description

`append(x,y)`

- append all items in y to x
- if x is a list and y is not a list, y is enlisted and later appended
- if x is a list and y is a list, all cells in x and y are formed into a new list

### Type rules

Order: **vice versa**

```
Real, Real   -> Real
Clex, Clex   -> Clex
Sym , Sym    -> Sym
Char, Char   -> Char
List, _      -> List
Enum, Number -> Enum
_   , _      -> domain error
```

### Examples

```
append(0, -1 2 3)
> 0 -1 2 3
```
