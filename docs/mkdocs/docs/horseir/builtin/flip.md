# flip


### Type Rules

```
dict -> table
list -> list
_    -> domain error
```

!!! Note "Potential Extension"
    Support table to dict

### Shape Rules

- Return a `table` shape if its input is a `dict` shape
- Return a ^^flipped^^ `list` shape if its input is a `list` shape

