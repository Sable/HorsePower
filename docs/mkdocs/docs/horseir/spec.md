<table class="top_table">
<tr>
<td width=76%>
    <div class="left">
        <todo>To-do</todo>
        <!--<done>Completed</done>-->
    </div>
</td>
<td>
    <div class="right">
        <small> Hanfeng Chen, May 2017 <br> (Updated on July 2018) </small>
    </div>
</td>
</tr>
</table>

## 1. Scanner

### 1.1 Source code representation

HorseIR supports ASCII charset, except for string encoding, i.e. strings and symbols.

<todo>
Support Unicode for strings and symbols.
</todo>

### 1.2 Keywords

```no-highlight
def   import   return   goto   check_type   check_cast
```

<todo>
Support goto and check_type
</todo>

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

```no-highlight
// this is a line comment (style 1)
...
/*
 * this is a block comment (style 2)
 */
```

<todo> Support the style 2 </todo>
<todo> Unicode is allowed </todo>

### 1.5 Literals

HorseIR has a rich set of literals for each type.

- <blue>bit</blue>
- bool
- char
- string
- integer: <blue>i8</blue>, i16, i32, and i64
- real: f32 and f64
- <blue>complex</blue>
- symbol
- date & time
- <blue>function</blue>
- list
- dictionary
- table
- <blue>keyed table</blue>
- special: <blue>N/A</blue>, <blue>Inf</blue>, and <blue>NaN</blue>

See [all types](types.md).

<todo> Support the types highlighted. </todo>

### 1.6 Identifier

HorseIR only uses the ASCII charset.

```no-highlight
id ::= [a-zA-Z_][a-zA-Z0-9_]*
```

### 1.7 Semicolons

A semicolon must be found at the end of a statement.

## 2. Parser

### 2.1 Program structure

A valid HorseIR program has the following parts.

1. Modules and methods
2. Import statements
3. Local and global variables

### 2.2 Module

A module is used to organize methods and variables in a specific namespace.  It
is allowed to declare two methods with the same name in different namespace.

<u>Conventions</u>

- A **module** has zero or more methods and global variables
- A **method** has zero or more parameters and local variables
- A **gloabl variable** declaration must be placed inside a module
- An **entry method** `main` defines the entry of a program
- If there is no module declaration top of a method, this method is included into a default module (i.e. `default`)

<u>Sample</u>

```no-highlight
def default{      // a module 'default'
    import Builtin;
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


#### Variable declaration

Each variable declaration in a program is associated with a type.  A variable
declared inside a method is considered as a **local variable**.  If a variable
is declared outside a method explicitly, it is a **global variables**.

```no-highlight
module default{             // a module 'default'
    import Builtin;
    def global_var:i64;     // a global variable
    def main(){             // entry method
       local_var:i64 = ...; // a local variable
    }
}
```

#### Method declaration

A method takes zero or more parameters, and it may or may not have a return
type.  A pair of parenthesis defines the method body.

```no-highlight
def foo(x:i64, y:i64) : i64{
	v:i64 = ...;
	return v;
}
```

#### Method overloading

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

#### Unknown number of parameters (Optional)

A method may not decide the number of parameters at compile time.  [In
C](https://stackoverflow.com/questions/10071186/function-with-unknown-number-of-parameters-in-c),
a type `va\_list` is used to handle unknown number of parameters.

```no-highlight
def foo(x:i64, ...){
}
```

### 2.3 Types

See [types](types.md).

#### Basic types

HorseIR supports basic types as follows.

```no-highlight
boolean, short, int, long, float, double, complex numbers.
```

#### Real numbers

```no-highlight
bool, short, int, long, float, double, char (ascii value)
```

#### Complex numbers

A complex number consists of two parts: real numbers and imaginary unit.

For example,

```no-highlight
2 + 3i
```

where `2` is the real number and `3i` is the imagninary unit.

<todo> Not supported yet </todo>

#### String

```no-highlight
"string"  // string
`symbol   // symbol
```

About Unicode

- Unicode is not supported currently, but we will add it later.
- See [escape sequences in C](https://en.wikipedia.org/wiki/Escape_sequences_in_C)
- Discussions about [wchar vs. icu](https://stackoverflow.com/questions/4507222/inconsistency-in-unicode-with-wchar-t-vs-icu-in-c)

#### List

A list is a collection of heterogeneous data.  It consists of cells and each
cell has homogeneous data or another list.  Thus, it is possible to have
generic types associated with a list, such as `list<i64>`.

```no-highlight
x0:i64 = 0:i64;
x1:i64 = 1:i64;
x2:list<i64> = @list(x0,x1); // a list of integers
```

#### Dictionary

A dictionary is a key to value pair.  Given a key, the dictionary is able to
fetch its stored value directly.

```no-highlight
dict<sym, list<i64>>  // a mapping from symbol to a list of integers
```

#### Table

A table is a list of columns.  A column can be represented by a special
dictionary whose key is a symbol (i.e. a column name).

```no-highlight
column_key:sym = `d0`d1...`dn:sym;
column_value:list<?> = @list(d0,d1,...,dn);
t:table = @table(column_key, column_value);
```

#### Keyed table

A keyed table consists of two normal tables (non-keyed).  The two tables must
have the same number of rows.

```no-highlight
kt:ktable = @ktable(t0,t1); // t0 and t1 are tables
```

### 2.4 Statements

#### Empty statements

Support.

#### Expression statements

Support.

#### Assignment statements

An assignment statement has an assignment `=`.

#### Return statements

A return statement only accepts zero or one expression.

#### Goto statements

A goto statement takes one valid label name.

```no-highlight
goto [label_name];
```

<u><b>No</b> break, continue, if, while or switch.</u>


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

### Table

#### Create an empty table

A normal table

```no-highlight
my_meta:list<sym> = {`employee `department; `str`sym}
my_table:table    = @table(my_meta);
```

A keyed table

```no-highlight
my_meta_key:list<sym> = {`id; `i64}
my_meta_val:list<sym> = {`employee `department; `str`sym}
my_table_key:table    = @table(my_meta_key);
my_table_val:table    = @table(my_meta_val);
    my_table:table    = @ktable(my_table_key, my_table_val);
```

<todo> Not supported yet </todo>

#### Load data to a table

The data to be loaded can be stored in a regular file format. A built-in
function (e.g. load_csv) loads the data with given file specifications.

```no-highlight
r:i64 = @load_csv(table_name, file_name, file_format);
 ...    @load_txt  ...
```

<todo> Not supported yet </todo>

## Appendix

Grammar

- [EBNF](https://github.com/Sable/HorsePower/blob/master/src/HorseIR/optimizer/opt.y)

Basics

- [Names](naming.md)
- [Types](types.md)
- [Date time](date.md)
- [Built-in functions](functions.md)

Database operations

- [Clauses](database.md#clause)
- [Operations](database.md#operations)
- [References](database.md#reference)
- [Examples](database.md#examples)

Additional features

- Data streaming
- Server and client modes

