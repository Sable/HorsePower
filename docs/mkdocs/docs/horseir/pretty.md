## Pretty printing

We propose a couple of pretty printing modes as follows.

- Print all elements (real numbers, complex numbers and symbols)
- Print abbreviations for advanced types (nested lists, dictionary,
  enumeration, tables and keyed tables)
- Print customized sizes (e.g. 20 rows and 5 columns max)
- Print within current window (Optional)

### Real numbers

#### Bool

```
0 1:b
```

#### Short

```
10 20:h
```


#### Int

```
1234567 2:i32
```

#### Long

```
123456789012345 123:i64
123456789012345 123
```

#### Float

```
12.3 2.5:f32
```

#### Double

```
12.3 2.5:f64
12.3 2.5
```

#### Char

```
abcd
abcd:c
```

### Complex numbers

```
1+2i 2 3i
1+2i 2 3i:x
```

### Symbols

```
`apple `"my name":q
`apple `"my name"
```

### Dictionary

```
// dict<key, value>
dict<`name, 1 2 3:i32>
```

### Enumeration
### Table
### Keyed table



