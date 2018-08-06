## reduction

### Overview

|Function| Initial Value | Type    |
|:------:|:-------------:|:-------:|
| sum    | 0             | Various |
| avg    | 0             | Float   |
| min    | Int/Float_MAX | Various |
| max    | Int/Float_MIN | Various |

## sum

### Description

- Sum up all numerical values in a vector: `z = sum(x)`

$$ \sum_{i=0}^{n-1} x_i $$

### Type Rules

```no-highlight
B -> I
H -> I
I -> I
C -> I
L -> L
F -> F
E -> E
_ -> domain error
```

## avg

### Description

- Average all numerical values in a vector: `z = avg(x)`

$$ \frac{1}{n} * \sum_{i=0}^{n-1} x_i $$

### Type Rules

```no-highlight
B -> F
H -> F
I -> F
C -> F
L -> E
F -> F
E -> E
_ -> domain error
```

## min

### Description

- Minimal values in a vector: `z = min(x)`
- Initial value is determined by the maximal value of the type of `x`.

### Type Rules

```no-highlight
B -> B
H -> H
I -> I
C -> C
L -> L
F -> F
E -> E
_ -> domain error
```

### Examples

```no-highlight
min((4,9,16):i32)
> 4:i32
```

## max

### Description

- Maximal values in a vector: `z = max(x)`
- Initial value is determined by the minimal value of the type of `x`.

### Type Rules

```no-highlight
B -> B
H -> H
I -> I
C -> C
L -> L
F -> F
E -> E
_ -> domain error
```

### Examples

```no-highlight
min((4,9,16):i32)
> 16:i32
```
