# HorseIR Syntax Specification

Hanfeng Chen, May 2017

## 1. Scanner

### 1.1 Source code representation

HorseIR supports ASCII charset, except for string encoding, i.e. strings and symbols.

### 1.2 Keywords

```
def   import   nop   return   goto   check_type   check_cast   nil
```

### 1.3 Built-in functions

There is no operator in HorseIR, instead, an equivalent name is assigned to each operator.  For example, `add` for `+`.  In our convention, a leading `@` is required for  These built-in functions are classified in the following groups.

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

### 2.2 Module

### 2.3 Variable declaration

### 2.4 Function declaration

### 2.5 Types

### 2.6 Statements

### 2.7 Expressions

## 3. IR design

### [Grammar](../MIR/MIR.txt)

- [Names](names.md)
- [Types](types.md)

### Primitives

- [Builtins](primitives.md)

### Database operations

- [Operations](db_ops.md)
- [References](db_ref.md)


