# like

### Description

`like(x,y)`

- find all matched items in x based on the pattern y
- x: a string, a list of strings, a symbol, or a list of symbols
- y: a string pattern

### Pattern Conversion

In [SQL like](https://www.w3schools.com/sql/sql_like.asp),

- `%` means zero or more
- `_` means a single character

In [PCRE](https://www.debuggex.com/cheatsheet/regex/pcre),

- `.*` mean zero or more
- `.`  means a single character

A conversion is required to generate corresponding pattern string.
(The heading sign `^` and the last sign `$` should be appended.)

### Type Rules

High-level

```no-highlight
str , (str,sym) -> bool
sym , (str,sym) -> bool
_   , _         -> domain error
```

Table with details (See [type alias](../types.md))

![any](../types/like.png)

### Shape Rules

[Shape left rules](../shapes.md#shape-left)


### Examples

```no-highlight
like('hello world', '*lo')
> 1:bool
```
