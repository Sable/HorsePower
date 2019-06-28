# Examples of HorseIR

## Data printing

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
```

#### Float

```no-highlight
(12.3,2.5):f32
```

#### Double

```no-highlight
(12.3,2.5):f64
```

#### Char

```no-highlight
'abcd':char
```

#### Complex numbers

```no-highlight
(1+2i,2,3i):complex
```

#### Symbols

```no-highlight
(`apple,`"my name"):sym
```


### Compound types

#### List

Input literal:

```no-highlight
@list((1,2,3):i32, (2,0.5):f32)
```

Output:

```no-highlight
((1,2,3):i32, (2,0.5):f32):list<?>
```

#### Dictionary

Input literal:

```no-highlight
key:sym = (`key1,`key2):sym;
value:sym = @list((1,2,3):i32, (2,0.5):f32);

@dict(key, value);
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
enum_u = `u:sym;
enum_v = (`a,`b,`c):sym;

@enum(enum_u, enum_v);
```

Output:

```no-highlight
<`u, (`a,`b,`c)>
```

#### Table

Input literal:

```no-highlight
table_names:sym = (`col1, `col2):sym;
table_values:list<?> = @list((`a,`b,`c):str, (1,2,3):i32);

@table(table_names, table_values);
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
key_table:table = @table(`col1:sym, (`a,`b,`c):sym);
value_table:table = @table(`col2:sym, (1,2,3):i32);

@ktable(key_table, value_table);
```

Output:

```no-highlight
col1 | col2
-----------
a    | 1   
b    | 2   
c    | 3   
```


