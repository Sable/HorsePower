# Logic

All logic operations are defined with the following type rules.

```no-highlight
// dyadic
bool,bool -> bool
_         -> domain error

// monadic
bool -> bool
_    -> domain error
```

## and

`and(x,y)`

```no-highlight
      and((0,0,1):bool, 0:bool)
(0,0,0):bool
```

## nand

`nand(x,y)`

```no-highlight
      nand((0,0,1):bool, 0:bool)
> (1,1,1):bool
```

## or

`or(x,y)`

```no-highlight
      or((0,0,1):bool, 0:bool)
> (0,0,1):bool
```

## nor

`nor(x,y)`

```no-highlight
      nor((0,0,1):bool, 0:bool)
> (1,1,0):bool
```

## not

`not(x)`

```no-highlight
      not((0,0,1):bool)
> (1,1,0):bool
```

## xor

`xor(x,y)`

```no-highlight
      xor((0,0,1):bool,0:bool)
> (0,0,1):bool
```

