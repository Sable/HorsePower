# logic

All logic operations are defined with the following type rules.

### Type Rules

High-level

```no-highlight
// dyadic
bool,bool -> bool
_   ,_    -> domain error

// monadic
bool -> bool
_    -> domain error
```

Table with details (See [type alias](../types.md))

![logic](../types/logic.png)

### Shape Rules

[Dyadic elementwise shape rules](../shapes.md#dyadic-elementwise)


### List of Functions

#### and

`and(x,y)`

```no-highlight
      and((0,0,1):bool, 0:bool)
(0,0,0):bool
```

#### nand

`nand(x,y)`

```no-highlight
      nand((0,0,1):bool, 0:bool)
> (1,1,1):bool
```

#### or

`or(x,y)`

```no-highlight
      or((0,0,1):bool, 0:bool)
> (0,0,1):bool
```

#### nor

`nor(x,y)`

```no-highlight
      nor((0,0,1):bool, 0:bool)
> (1,1,0):bool
```

#### not

`not(x)`

```no-highlight
      not((0,0,1):bool)
> (1,1,0):bool
```

#### xor

`xor(x,y)`

```no-highlight
      xor((0,0,1):bool,0:bool)
> (0,0,1):bool
```

