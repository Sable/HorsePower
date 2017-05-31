# HorseIR Syntax Specification

Hanfeng Chen, May 2017

## 1. Scanner

### 1.1 Source code representation

HorseIR supports ASCII charset, except for string encoding, i.e. strings and symbols.

### 1.2 Keywords

```
def   import   return   goto   check_type   check_cast   nil
```

### 1.3 Built-in functions

There is no operator in HorseIR, instead, an equivalent name is assigned to
each operator.  For example, `add` for `+`.  In our convention, a leading `@`
is required for  These built-in functions are classified in the following
groups.

- Arithmetic
- Logic
- Datetime
- Trigonometric
- General
- Database related

See [built-in functions](primitives.md).

### 1.4 Comments

Both block comments and line comments are supported.

```
// this is a line comment
...
/*
 * this is a block comment
 */
```

### 1.5 Literals

HorseIR has a rich set of literals for each type.

- nil
- bool
- char
- integer
- real
- complex
- symbol
- date & time
- function
- list
- dictionary
- table & keyed table

### 1.6 Identifier

HorseIR only uses the ASCII charset.

```
id     ::= letter { { "_" } (letter | digit) }
letter ::= [a-zA-Z]
digit  ::= [0-9]
```

### 1.7 Semicolons

A semicolon must be found at the end of a statement.

## 2. Parser

### 2.1 Program structure

A valid HorseIR program has the following parts.

1. a list of import statements
2. a module declarations

### 2.2 Module

A module is used to organize methods and variables in a specific namespace.  It
is flexible to declare two methods with the same name in different namespace.

<u>Conventions</u>

- A **module** has zero or more methods and global variables
- A **method** has zero or more parameters and local variables
- A **gloabl variable** declaration must be placed inside a module
- If there is no module declaration top of a method, this method is included into a default module (i.e. `default`)
- An entry method (i.e. `main`) should be provided.

<u>Sample</u>

```
import Builtin;

def default{      // a module 'default'
    def main(){   // entry method
    }
}
```

#### Variable declaration

Each variable declaration in a program is associated with a type.  A variable
declared inside a method is considered as a **local variable**.  If a variable
is declared outside a method explicitly, it is a **global variables**.

```
import Builtin;

def default{                // a module 'default'
    def global_var:i64;     // a global variable
    def main(){             // entry method
       local_var:i64 = ...; // a local variable
    }
}
```

#### Method declaration

A method takes zero or more parameters, and it may or may not have a return
type.  A pair of parenthesis defines the method body.

```
def foo(x:i64, y:i64) : i64{
	v:i64 = ...;
	return v;
}
```

### 2.3 Types

See [types](types.md).

#### Basic types

HorseIR supports basic types as follows.

```
boolean, byte, short, int, long, float, double and complex numbers.
```

#### String

```
string and symbol
```

#### List

A list is a collection of heterogeneous data.  It consists of cells and each
cell has homogeneous data or another list.  Thus, it is possible to have
generic types associated with a list, such as `list<i64>`.

```
x0:i64 = 0:i64;
x1:i64 = 1:i64;
x2:list<i64> = list(x0,x1); // a list of integers
```

#### Dictionary

A dictionary is a key to value pair.  Given a key, the dictionary could fetch
its corresponding value easily.

```
dict<sym, list<i64>>  // a mapping from symbol to a list of integers
```

#### Table

A table is a list of columns.  A column can be represented by a special
dictionary whose key is a symbol (i.e. a column name).

```
t:table = list(d0,d1,...,dn);
```

#### Keyed table

A keyed table consists of two normal tables (non-keyed).  The two tables must
have the same number of rows.

```
kt:ktable = list(t0,t1);
```


### 2.4 Statements

#### Empty statements

Support.

#### Expression statements

Support.

#### Assignment statements

An assignment statement has an assignment operator `=`.

#### Return statements

A return statement only accepts zero or one expression.

#### Goto statements

A goto statement takes one valid label name.

```
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

The `check_type` is used to guarantee the types from both sides, lvalue and
rvalue, agree.

The `check_cast` checks whether a designated type casting is allowed.


## 3. References

### 3.1 Grammar

- [EBNF](../../../src/HorseIR/HorseIR.txt)

### 3.2 Basics

- [Names](names.md)
- [Types](types.md)
- [Date time](date.md)
- [Builtins](primitives.md)

### 3.3 Database operations

- [Clauses](db_clause.md)
- [Operations](db_ops.md)
- [References](db_ref.md)

### 3.4 Additional features

- Data streaming
- Server and client modes


