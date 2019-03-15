!!! tip "Pretty printer for HorseIR programs"
    Pretty printer.

### Principles

We propose a couple of pretty printing modes as follows.

- Print all elements (real numbers, complex numbers and symbols)
- Print abbreviations for advanced types (nested lists, dictionary,
  enumeration, tables and keyed tables)
- Print customized sizes (e.g. 20 rows and 5 columns max)
- Print within current window (Optional)


### Basic types

#### Bool

```no-highlight
(0,1):bool
```

#### Short

```no-highlight
(10,20):i16
```

#### Int

```no-highlight
(1234567,2):i32
```

#### Long

```no-highlight
(123456789012345,123):i64
(123456789012345,123)
```

#### Float

```no-highlight
(12.3,2.5):f32
```

#### Double

```no-highlight
(12.3,2.5)
(12.3,2.5):f64
```

#### Char

```no-highlight
'abcd'
'abcd':char
```

#### Complex numbers

```no-highlight
(1+2i,2,3i)
(1+2i,2,3i):complex
```

#### Symbols

```no-highlight
(`apple,`"my name"):sym
(`apple,`"my name")
```


### Compound types

#### List

Input literal:

```no-highlight
[(1,2,3):i32, (2,0.5):f32]
```

Output:

```no-highlight
[(1,2,3):i32, (2,0.5):f32]
```

#### Dictionary

Input literal:

```no-highlight
{`key1 -> (1,2,3):i32, `key2 -> (2,0.5):f32}
```

Output:

```no-highlight
{
    `key1 -> (1,2,3):i32,
    `key2 -> (2,0.5):f32
}
```

#### Enumeration

Input literal:

```no-highlight
<`u, (`a,`b,`c):sym>
```

Output:

```no-highlight
<`u, (`a,`b,`c)>
```

#### Table

Input literal:

```no-highlight
{`col1 -> (`a,`b,`c), `col2 -> (1,2,3):i32}
```

Output:

```no-highlight
col1 col2
---------
a    1   
b    2   
c    3   
```

#### Keyed table

Input literal:

```no-highlight
{[`col1 -> (`a,`b,`c)], `col2 -> (1,2,3):i32}
```

Output:

```no-highlight
col1 | col2
-----------
a    | 1   
b    | 2   
c    | 3   
```


