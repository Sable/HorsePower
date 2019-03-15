<!--
<table class="top_table">
<tr>
<td width=76%>
    <div class="left">
        <todo>To-do</todo>
    </div>
</td>
<td>
    <div class="right">
        <small> Hanfeng Chen, May 2017 <br> (Updated on July 2018) </small>
    </div>
</td>
</tr>
</table>
-->

## 1. Scanner

### 1.1 Source code representation

HorseIR supports ASCII charset, except for string encoding, i.e. strings and symbols.

!!! note "To-do"
    Support {++Unicode++} for strings and symbols.

### 1.2 Keywords

```no-highlight
def   import   return   goto   check_type   check_cast
```

### 1.3 Built-in functions

HorseIR has no operator, but a semantically equivalent name is assigned to
each operator.  For example, the addition function `add` for the operator `+`.
A leading character `@` is required for indicating the function in HorseIR.
These built-in functions are classified in the following groups.

- Arithmetic
- Logical
- Date and time-related
- Trigonometric
- Database-related
- General

See [built-in functions](functions.md).

### 1.4 Comments

Both block comments and line comments are supported.

```c
// this is a line comment (style 1)
...
/*
 * this is a block comment (style 2)
 */
```

!!! note "To-do"
    - Support the style 2
    - Unicode is allowed

### 1.5 Literals

HorseIR has a rich set of literals for each type.

- {++bit++}
- bool
- char
- string
- integer: {++i8++}, i16, i32, and i64
- real: f32 and f64
- {++complex++}
- symbol
- date & time
- {++function++}
- list
- dictionary
- table
- {++keyed table++}
- special: {++N/A++}, {++Inf++}, and {++NaN++}

See [all types](types.md).

!!! note "To-do"
    Support the types highlighted

### 1.6 Identifier

HorseIR only uses the ASCII charset.

```no-highlight
id ::= [a-zA-Z_][a-zA-Z0-9_]*
```

## 2. Parser

### 2.1 Program structure

A valid HorseIR program has the following parts.

1. Modules and methods
2. Import statements
3. Local and global variables

### 2.2 Module

A module is used to organize methods and variables in a specific namespace.  It
is allowed to declare two methods with the same name in different namespace.

#### Conventions

| Name            | Definition                                             |
|----------------:|:-------------------------------------------------------|
| module          | zero or more methods and global variables              |
| method          | zero or more parameters, 1 return, and local variables |
| entry method    | the method <u>main</u> defines the entry of a program  |
| global variable | declaration must be placed inside a module             |

<!--
<dl>
    <dt>module</dt>
    <dd>zero or more methods and global variables</dd>
    
    <dt>method</dt>
    <dd>zero or more parameters, 1 return, and local variables</dd>

    <dt>entry method</dt>
    <dd>the method <u>main</u> defines the entry of a program</dt>

    <dt>global variable</dt>
    <dd>declaration must be placed inside a module</dd>
</dl>
-->

*Note:* If there is no module declaration top of a method, this method is
included into a default module (i.e. the method name ^^default^^).

<!--
- A **module** has zero or more methods and global variables
- A **method** has zero or more parameters and local variables
- A **gloabl variable** declaration must be placed inside a module
- An **entry method** `main` defines the entry of a program
- If there is no module declaration top of a method, this method is included into a default module (i.e. `default`)
-->

^^Sample^^

```no-highlight
def default{      // a module 'default'
    import Builtin.*;
    def main(){   // an entry method
    }
}
```

<u>Name issues</u>

- Distinct name
    + A module name
    + A method name in a module
    + A global variable in a module
    + A local variable in a method
    + Method name with global name
- Same name
    + Module name with method / global / local variable name
    + Method name with local variable name
    + Global variable name with local variable name


#### Variables

Each variable has a type.
A variable declared inside a method is considered as a **local variable**.
If a variable is declared outside a method explicitly, it is a **global variable**.

```no-highlight
module default{             // a module 'default'
    import Builtin;
    def global_var:i64;     // a global variable
    def main(){             // entry method
       local_var:i64 = ...; // a local variable
    }
}
```

#### Methods

^^Declaration^^

A method takes zero or more parameters, and it may or may not have a return type.
A pair of parenthesis defines the method body.

```no-highlight
def foo(x:i64, y:i64) : i64{
	v:i64 = ...;
	return v;
}
```

^^Overloading^^

Two methods may share a same method name, but with different number of
arguments or different types.  An unknown type `?` covers all possible types,
so that it is not allowed to declare a same method with a specific type.

For example,

```no-highlight
def foo(x:?){
}

def foo(x:i64){ // not allowed
}

def foo(x:i64, y:f32){ // allowed
}
```

^^Unknown number of parameters (Optional)^^

A method may not decide the number of parameters at compile time.
[In C](https://stackoverflow.com/questions/10071186/function-with-unknown-number-of-parameters-in-c),
a type `va\_list` is used to handle unknown number of parameters.

```no-highlight
def foo(x:i64, ...){
}
```

### 2.3 Types

See all available [types](types.md).

#### Basic types

HorseIR supports basic types as follows.

^^Integers^^

```no-highlight
Types  | (bool)   (i8)   (i16)  (i32)  (i64)
Names  | boolean  small  short  int    long 
```

^^Floating numbers^^

```no-highlight
Types  | (f32)  (f64) 
Names  | float  double
```

^^Real numbers^^

Real numbers are the combination of integers and floating numbers.

^^Complex numbers^^

A complex number consists of two parts: real numbers and imaginary unit.
For example, `2 + 3i`, where `2` is the real number and `3i` is the imagninary unit.

!!! note "To-do"
    Support complex numbers.

^^String types^^

Three different string types: char, string, and symbol, have different delimiters as follows.

```no-highlight
'char'    // char
"string"  // string
`symbol   // symbol
```

It should be noted that a char type should not be treated as an integer (ASCII value) as it is in C.

!!! note "To-do"

    About Unicode

    - Unicode is not supported currently, but we will add it later.
    - See [escape sequences in C](https://en.wikipedia.org/wiki/Escape_sequences_in_C)
    - Discussions about [wchar vs. icu](https://stackoverflow.com/questions/4507222/inconsistency-in-unicode-with-wchar-t-vs-icu-in-c)

#### Compund Types

^^List^^

A list is a collection of heterogeneous data.  It consists of cells and each
cell has homogeneous data or another list.  Thus, it is possible to have
generic types associated with a list, such as `list<i64>`.

```no-highlight
x0:i64 = 0:i64;
x1:i64 = 1:i64;
x2:list<i64> = @list(x0,x1); // a list of integers
```

^^Dictionary^^

A dictionary is a key to value pair.  Given a key, the dictionary is able to
fetch its stored value directly.

```no-highlight
dict<sym, list<i64>>  // a mapping from symbol to a list of integers
```

^^Table^^

A table is a list of columns.  A column can be represented by a special
dictionary whose key is a symbol (i.e. a column name).

```no-highlight
column_key:sym = `d0`d1...`dn:sym;
column_value:list<?> = @list(d0,d1,...,dn);
t:table = @table(column_key, column_value);
```

^^Keyed table^^

A keyed table consists of two normal tables (non-keyed).  The two tables must
have the same number of rows.

```no-highlight
kt:ktable = @ktable(t0,t1); // t0 and t1 are tables
```

### 2.4 Statements

| Statement Types  | Description             |
|:----------------:|:------------------------|
| Empty      | i.e. `;` |
| Expression | Statement without assignment, e.g. `check_cast`   |
| Assignment | An assignment statement has an assignment `=`     |
| Return     | A return statement only accepts zero or one expression |
| Goto       | A goto statement takes one valid label name, i.e.  `goto [label_name];` |

<u><b>No</b> break, continue, if, while or switch.</u>

!!! note "Semicolon is mandotary"
    A semicolon must be found at the end of a statement.

### 2.5 Expressions

#### Identifiers

- Starting with a letter;
- Ending with a letter or a number;
- Allowing letters, numbers, '-'s and '_'s in between.

#### Literals

See [examples](types.md) about literals.

#### Function calls

A function call as an expression should start with a leading `@`.  For example,
`@add` or `@Builtin.add` refers to a builtin function `add`.

#### Type and cast checks

- The `check_type` is used to guarantee the types from both sides, lvalue and
rvalue, agree.
- The `check_cast` checks whether a designated type casting is allowed.

## 3. Database operations

Build a normal table

```no-highlight
my_meta:list<sym> = @list(`employee`department:sym);
my_table:table    = @table(my_meta);
```

Build a keyed table

```no-highlight
my_meta_key:list<sym> = @list(`id:i64);
my_meta_val:list<sym> = @list(`employee`department:`sym);
my_table_key:table    = @table(my_meta_key);
my_table_val:table    = @table(my_meta_val);
    my_table:table    = @ktable(my_table_key, my_table_val);
```

!!! note "To-do"

    - A function for loading data from external files directly, e.g. `load_csv`
    - A way for creating an empty table

## Appendix

Grammar

- [EBNF](https://github.com/Sable/HorsePower/blob/master/src/HorseIR/optimizer/opt.y)

Basics

- [Names](naming.md)
- [Types](types.md)
- [Date time](builtin/date.md)
- [Built-in functions](functions.md)

Database operations

- [Clauses](database.md#clause)
- [Operations](database.md#operations)
- [References](database.md#reference)
- [Examples](database.md#examples)

Additional features

- Data streaming
- Server and client modes

