# A Collection of Functions for Advanced Types

!!! note "Advanced Types"
    Include **enum**, **dictionary**, **table**, and **ktable**.

## enum

An enumeration consists of two parts: a list of unique items (denoted **P**)
and a list of non-unique items (denoted **F**).  Any item in F can be found in
P.

### Description

```no-highlight
pValue:? = `chen`hendren`lavoie:sym;
fValue:? = `chen`chen`lavoie`hendren:sym);
eItem:?  = @enum(pValue, fValue);
```

- `pValue` stores three unique names
- `fValue` enumerates the values from `pValue`
- `eItem` saves the enum created by the function `enum`

### Type Rules

```no-highlight
Basic, Basic -> enum
list , list  -> enum
_    , _     -> domain error
```

### Examples

Format

```no-highlight
<`pValue, (0,0,2,1)>
```

Advanced: single key in a keyed table

```no-highlight
// kt is a keyed table
z:? = @enum(`kt, value);
```

- A single key is used by default

Compound keys in a keyed table

```no-highlight
// kt is a keyed table
z:? = @enum((`kt,`kcol), value);
```

- The keyed column `kcol` in table `kt` is specified

## dict

A dictionary consists of mappings between keys and values.
The function `dict` returns a dictionary by given two parameters: key and value.

### Description

```no-highlight
dItem:?  = @dict(key, value);
```

### Type Rules

```no-highlight
Any, Any -> enum
_  , _   -> domain error
```

### Examples

```no-highlight
key:?   = `a`b`c:sym;
val:?   = (1,2,3):i32;
dItem:? = @dict(key, val);
```

## table

A table is a special case of dictionary, i.e. a collection of mappings from a
string (column name) maps to a vector (column value).
The function `table` returns a table.

### Description

```no-highlight
tItem:?  = @table(name, value);
```

- `name`: a list of symbols
- `value`: a list of values
- The length of `name` and `value` must agree

### Type Rules

```no-highlight
sym, list -> enum
_    , _  -> domain error
```

### Examples

```no-highlight
name:?  = `number:sym;
value:? = (1,2,3):i32;
tItem:? = @table(name, value);
```

## ktable

A key table is a special case of table which internally represents two tables:
one table as a keyed part and another table as a non-keyed part.
The function `ktable` returns a key table.

### Description

```no-highlight
kItem:?  = @ktable(table_key, table_value);
```

- `table_key`: a table 
- `table_value`: a table
- No duplicated records (in-row) exist in `table_key`

### Type Rules

```no-highlight
table, table -> ktable
_    , _     -> domain error
```

### Examples

See description.


