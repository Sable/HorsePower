# str

### Description

```no-highlight
@str(x)
```

- input with the value `x`, output string
- default format: `json`

### Type Rules

```no-highlight
Any -> str
_   -> domain error
```

### Examples

```no-highlight
    @str((-1,2,3):i32)
"{"type": "i32", "value": [-1,2,3]}":str
```
