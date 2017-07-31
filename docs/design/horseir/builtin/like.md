## like

### Description

`like(x,y)`

- find all matched items in x based on the pattern y
- x: a string, a list of strings, a symbol, or a list of symbols
- y: a string pattern

### Pattern conversion

In [SQL like](https://www.w3schools.com/sql/sql_like.asp),

- \% means zero or more
- \_ means a single character

In [PCRE](https://www.debuggex.com/cheatsheet/regex/pcre),

- \.\* mean zero or more
- \.  means a single character

A conversion is required to generate corresponding pattern string.
(The heading sign `^` and the last sign `$` should be appended.)

### Type rules

```
Str , Str -> Bool
Sym , Str -> Bool
List, Str -> Bool
_   , _   -> domain error
```

### Examples

```
like('hello world', '*lo')
> 1
```
