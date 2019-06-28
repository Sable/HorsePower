# A Collection of Database Help Functions

!!! note "Datatbase Helper Functions"
    A collection of database related functions

| Function                      | Code                   | Description                            |
|:-----------------------------:|:-----------------------|:---------------------------------------|
| [keys](#keys)                 | `@keys(x)`             | Return key                             | 
| [values](#values)             | `@values(x)`           | Return value                           | 
| [meta](#values)               | `@meta(x)`             | Return meta information                | 
| [fetch](#fetch)               | `@fetch(x)`            | Fetch actual value                     | 
| [column_value](#column_value) | `@column_value(x, n)`  | Load the column 'n' from the table 'x' | 
| [load_table](#load_table)     | `@load_table(n)`       | Load a table 'n'                       | 

### keys

**Description**

- Return the key value of a variable with a compound type

**Type Rules**

```no-highlight
dict   -> Any
enum   -> Any
table  -> sym
ktable -> table
_      -> domain error
```

### values

**Description**

- Return the value of a variable with a compound type
- For enumeration, it returns the stored indices which have the type `i64`

**Type Rules**

```no-highlight
dict   -> Any
enum   -> i64
table  -> list
ktable -> table
_      -> domain error
```

### fetch

**Description**

- Return the actual value stored in an enumeration

**Type Rules**

```no-highlight
enum   -> Any
_      -> domain error
```

### meta

**Description**

- Show a keyed table which represents the meta information of a table or a keyed table

**Type Rules**

```no-highlight
table  -> ktable
ktable -> ktable
_      -> domain error
```

**Examples**

```no-highlight
    // Table D
sym  price hq
-------------
appl 449.1 CA
ibm  108.2 NY
hp   24.5  CA
goog 890.3 CA

    // meta(D)
c    | t f a
-----|------
sym  | S    
price| E    
hq   | C    
```

where,

- `c` means column names;
- `t` means column types;
- `f` means the names of tables with foreign keys;
- `a` means column attributes.

### column_value

**Description**

- Load the column value by a given column name

**Type Rules**

```no-highlight
table  , sym -> Basic or enum
ktable , sym -> Basic or enum
_      , _   -> domain error
```

### load_table

**Description**

- Load a table from memory by a given table name

**Type Rules**

```no-highlight
sym -> table or ktable
 _  -> domain error
```
