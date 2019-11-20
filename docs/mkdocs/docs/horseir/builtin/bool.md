# A Collection of Boolean Functions

## any

```
    @any((0,1,0,1):bool)
1:bool
    @any((0,0,0,0):bool)
0:bool
```

Function `any` returns 

- `true` if any boolean item is true
- `false` if all boolean items are false

### Type Rules

```
bool -> bool
_    -> domain error
```

### Shape Rules

Return a scalar.


## all

```
    @all((0,1,0,1):bool)
0:bool
    @all((0,0,0,0):bool)
1:bool
```

Function `all` returns 

- `true` if all boolean item are true
- `false` if any boolean item is false

### Type Rules

```
bool -> bool
_    -> domain error
```

### Shape Rules

Return a scalar.


