# HorseIR Language Specification

## Overview

HorseIR is an array-based programming language inspired by [APL](https://en.wikipedia.org/wiki/APL_(programming_language)) and [ELI](http://fastarray.appspot.com). Designed as a non-SSA intermediate representation for compiler writers, it supports both scientific computation and relational database queries. This is achieved by combining a rich, SQL-inspired type system with structured control flow and typical operators found in most programming languages as a collection of built-in functions.

## Lexical Elements

A HorseIR program is written in the ASCII character set, with the exception of string data (i.e. characters, strings, and symbols) which may contain Unicode characters. Whitespace outside of string literals is ignored.

```
letter           = 'a' ... 'z' | 'A' ... 'Z' ;
digit            = '0' ... '9' ;
nzdigit          = '1' ... '9' ;
digits           = digit { digit } ;
ascii_character  = /* All valid ASCII */ ;
escape_sequence  = "\a" | "\b" | "\f" | "\n" | "\r" | "\t" | "\v" ;
```

### Comments

There are 2 styles of comments for programmer documentation, both of which are ignored.
1. Line comments start with `//` and ignore all text until the end of the line (or EOF)
2. Block comments start with `/*` and end with `*/` and ignore all text in-between. Block comments do not nest and must be terminated.

```
// Line comment

/* Block comments
 * may be multiline
 */
```

### Identifiers

Identifiers define program elements (e.g. variables, modules, and functions) and consist of letters, digits and the underscore. The first character must not be a digit.

```
Identifier  = ( letter | '_' ) { letter | digit | '_' } ;
```

The sink identifier, `_`, may be used as a `/dev/null` of any type. It does not introduce a binding on declaration.

### Keywords

Keywords are special character sequences which may not be used as identifiers (with exceptions, see below). Keywords define syntactic elements as well as types.

```
module      repeat      i32         month       ktable
import      var         i64         minute
global      return      f32         second
def         break       f64         time
kernel      continue    complex     func
check_cast  bool        str         list
if          char        sym         dict
else        i8          dt          enum
while       i16         date        table
```

!!! note

    Type keywords may be used as identifiers if they are part of a function literal (e.g. `@list`).

### Operators

There are no explicit operators for data manipulation. Instead, each operation is defined as a function, with [built-in functions](http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/functions/) providing functionality commonly found in other languages.

### Punctuation

Punctuation symbols give structure to programs and are part of the language syntax.

```
(     )     =     ,     /
[     ]     :     @
{     }     ?     .
<     >     ;     *
```


### Values

Each [basic type](#Basic-Types) has an associated literal value given below. 

```
Values        = ValueList ':' Type
ValueList     = Value | '(' Value { ',' Value } ')'
Value         = IntValue  | FloatValue  | BoolValue   | ComplexValue |
                CharValue | StringValue | SymbolValue | CalendarValue ;

Sign          = '+' | '-'
Integer       = '0' | nzdigit { digit } ;
Float         = Integer '.' [ digits ] | '.' digits ;
 
IntValue      = [ Sign ] Integer ;
FloatValue    = [ Sign ] Float ;
BoolValue     = '0' | '1' ;
 
CharValue     = "'" ascii_character "'" ;
StringValue   = '"' { ascii_character } '"' ;
SymbolValue   = '`' ( Identifier | StringValue ) ;
ComplexValue  = FloatValue [ Sign Float ] 'i' ;
                
CalendarValue = DateTimeValue | MonthValue  | DateValue |
                MinuteValue   | SecondValue | TimeValue ;

// Must correspond to valid dates within range, see the type declarations
MonthValue    = Integer '-' Integer ;
DateValue     = Integer '-' Integer '-' Integer ;
 
MinuteValue   = Integer ':' Integer ;
SecondValue   = Integer ':' Integer ':' Integer ;
TimeValue     = Integer ':' Integer ':' Integer '.' Integer ;

DateTimeValue = DateValue 'T' TimeValue ;
```

Examples

```
// bool
b0:bool = 0:bool;
b1:bool = (0):bool;
b2:bool = (0,1,1,0,1):bool;

// small
i8_0:i8 = 105:i8;
i8_1:i8 = (105):i8;
i8_2:i8 = (10, 100, 200):i8;

// short
i16_0:i16 = 1050:i16;
i16_1:i16 = (1050):i16;
i16_2:i16 = (100, 1000, 200):i16;

// int
i32_0:i32 = 10500:i32;
i32_1:i32 = (10500):i32;
i32_2:i32 = (1000, 10000, 20000):i32;

// long long
i64_0:i64 = 105000:i64;
i64_1:i64 = (105000):i64;
i64_2:i64 = (10000, 100000, 200000):i64;

// float
f32_0:f32 = 3.1415926:f32;
f32_1:f32 = (3.1415926):f32;
f32_2:f32 = (3.1415926, 1.1415926, 2):f32;

// double
f64_0:f64 = 3.1415926:f64;
f64_1:f64 = (3.1415926):f64;
f64_2:f64 = (3.1415926, 2, 2.0):f64;

// date
d0:d = 2010-09-01:d;     // 1 date
d1:d = (2010-09-01):d;   // 1 date
d2:d = (2010-09-01, 1010-01-31, 3019-12-29):d; // 3 dates

// month
m0:m = 2010-09:m;
m1:m = (2010-09):m;
m2:m = (2010-09, 2010-10, 2010-11):m;

//  minute
w0:w = 20:15:w;
w1:w = (20:15):w;
w2:w = (20:15, 21:00, 01:59):w;

// second
v0:v = 17:06:20:v;
v1:v = (17:06:20):v;
v2:v = (17:06:20, 12:10:01, 09:10:12):v;

// time
t0:t = 11:22:33.001:t;
t1:t = (11:22:33.001):t;
t2:t = (11:22:33.001, 22:33:11.999, 01:02:03.123):t;

// datetime
z0:z = 2019-01-02T17:10:21.001:z;
z1:z = (2019-01-02T17:10:21.001):z;
z2:z = (2019-01-02T17:10:21.001, 2019-12-02T17:01:21.001):z;

```

## Modules

A program consists of 1-or-more modules which define a collection of imports, functions and global variables. Modules may be combined into a single program and compiled together.

```
Module          = "module" Identifier '{' ModuleContents '}' ;
ModuleContents  = { ImportDirective | FunctionDeclaration | GlobalDeclaration } ;
```

### Built-in Modules

HorseIR provides a built-in module "Builtin" that implements the basic mathematical and database operations. It exists as a *pseudo*-module and its implementation is provided by the compiler. As there are no operators (see [Operators](#Operators)), this forms the core of the language functionality. Defining the built-in set as a regular module provides shadowing behaviour consistent with user-defined code.

HorseIR provides a second built-in module "System" which defines system variables and their respective default values. For example, `pp` controls the precision of output, with default value `10`. Importing the system module allows programs to customize their local system environment.


### Import Directives

Modules may be composed into larger programs, either by importing all module contents (`.*`), a specific element (`.Identifier`), or a list of elements (`.{Identifier, ...}`). Imports may include functions or global variables, however, they are not transitive.

```
ImportDirective  = "import" Identifier '.' ImportList ';' ;
ImportList = '*' | Identifier | '{' Identifier { ',' Identifier } '}' ;
```

### Function Declarations

Functions define a collection of statements with 0-or-more input parameters and 0-or-more return types (supporting multiple returns). Each input parameter defines its name and type.

```
FunctionDeclaration  = FunctionKind Identifier '(' Parameters ')'
                           [ ':' ReturnTypes ] Block ;

FunctionKind  = "def" | "kernel" ;

Parameters    = [ Parameter { ',' Parameter } ] ;
Parameter     = Identifier ':' Type ;

ReturnTypes   = Type { ',' Type } ;
```

If the function specifies a return type, then the body must return on all paths.

The function kind specifies its intended execution target. `def` indicates a generic function while `kernel` directs the runtime system to use connected GPUs.

### Entry Function

Execution begins with an entry function `main` with optional input parameter `args:List<?>` and any return type. When invoking a program, the entry module to search must be specified.

### Global Declarations

Global variables belong to modules, and may be shared through the import directive. Each global variable consists of a name and associated type.

```
GlobalDeclaration  = "global" Identifier ':' Type '=' Operand ';' ;
```

### Scope Rules

There are following scopes in a program:

1. Program scope: All modules in the compilation unit.
2. Module scopes: Functions and global variables in a module. Contents may be declared in any order.
3. Function scopes: Parameters and local variables in a function. Variables must be declared before use.
4. Block scopes: Blocks defined as part of control-flow structures define new scopes.

While there may be multiple module and function scopes, there is only a single program scope.

### Name Uniqueness

Declarations within a scope must be unique:

- A module name in the program scope
- A method name in a module
- A global variable in a module

### Name Resolution

To resolve the use of an identifier, the compiler checks:

1. Block scopes (if any)
2. Function scope
3. Module scope
4. Imported content

Local variables shadow global declarations, and global declarations shadow imported content.

Imported module content may optionally be used without the module name (i.e. as `sum` instead of `Bultin.sum`) if they have not been shadowed. In the case of shadowing, the fully qualified name is required. Both global variables and functions follow the same rules. Local variables cannot be imported.

```
module A {
    def x() : i32 { ... }
    def y() : i32 { ... }
}

module main {
    import A.*;
    
    def x() : i32 { ... }
    
    def main() {
        a:i32 = @x();    // Resolves to main.x
        b:i32 = @y();    // Resolves to A.y
        
        c:i32 = @A.x();  // Resolves to A.x
    }
}
```

When two imported modules contain an element of the same name, the last import shadows the earlier import.

## Types

```
Type  = Wildcard | BasicTypes | ListType | DictType | EnumType | TableTypes ;
```

### Wildcard Type

The wildcard type `?` is used to represent either heterogeneous data or dynamically-typed data of any type (including compound types). The compiler must resolve all statically-determined wildcards prior to generating code and ensure type correctness.

```
Wildcard  = '?' ;
```

Declarations may either specify the exact type, or use the wildcard to use compile-time resolution.

```
t:? = @sum(a);

/* Equivalent to the following, assuming @sum returns i32 */
t:i32 = @sum(a);
```

### Basic Types

The basic unit of data in HorseIR is a vector (i.e. an array), consisting of data of the same type.

| Name      | Alias    | Byte   | Description                   |
| :---------| :-------:| :-----:| :-----------------------------|
| boolean   | bool     | 1\*    | `0` (false) and `1` (true)    |
| small     | i8       | 1      | Half short integer or char    |
| short     | i16      | 2      | Short integer                 |
| int       | i32      | 4      | Integer                       |
| long      | i64      | 8      | Long integer (default, x64)   |
| float     | f32      | 4      | Single precision              |
| double    | f64      | 8      | Double precision              |
| complex   | complex  | 8      | Complex number (real+imaginary single precision floats)     |
| char      | char     | 1      | Half short integer or char    |
| symbol    | sym      | 8      | Symbol, but stored in integer |
| string    | str      | 8      | String                        |
| month     | month    | 4      | Month (<span>YYYY.MM</span>)  |
| date      | date     | 4      | Date (YYYY.MM.DD)             |
| date time | dt       | 8      | Date time                     |
| minute    | minute   | 4      | Minute (hh:mm)                |
| second    | second   | 4      | Second (hh:mm:ss)             |
| time      | time     | 4      | Time (hh:mm:ss.ll)            |
| function  | N/A      | 8      | Function literal (Starting with @) |

Syntactically, the type alias is used to refer to the type. The short name is used internally.

```
BasicTypes  = "bool" | "i8"   | "i16"     | "i32"    | "i64"    |
              "f32"  | "f64"  | "complex" | "char"   | "str"    | "sym"  |
              "dt"   | "date" | "month"   | "minute" | "second" | "time" ;
```

!!! note

    - Vectors of function literals are currently not supported
    - Plan: store boolean values as 1-bit

==TODO: A vector of function literals is necessary for join operations as join operators, for example, join_index(@lt@eq, columns_a, columns_b).==

### Compound Types

Compound types store more complex structures allowing heterogeneity and mappings.

| Name       | Alias     | Short | Description                   |
| :----------| :---------| :----:| :-----------------------------|
| list       | list      | G     | Collection of items           |
| dictionary | dict      | N     | Key-value mapping             |
| enumeration| enum      | Y     | Mapping (i.e. foreign key)    |
| table      | table     | A     | Collection of columns         |
| keyed table| ktable    | K     | Two normal tables             |

#### List Type

A list defines a variable length container consisting of *cells*, each cell containing data of any type. Nested lists are permitted.

```
ListType  = "list" '<' Type { ',' Type } '>' ;
```

For a list type, either a single type may be specified for the entire list, or a type for each cell. In the case of a single type, the list has an unbounded number of cells, all with the same type. If more than one type is specified, the list corresponds to a  tuple.

```
// Defines a list with 1+ cells of i32 type
list<i32>

// Defines a list with exactly 3 cells of explicit types
list<i32, i64, i32>
```

##### Wildcard Cell Type

List cell types may also be specified using the [wilcard type](#Wildcard-Type) according to the following rules:

1. If there is only 1 cell type given, the wildcard may resolve to a type list with *any number* of elements of *any type*
2. If there is more than 1 cell type given, the wildcard must resolve to a single type

```
// Equivalent to: list<i32>, list<i32, i64, i8>, etc.
t:list<?> = ...;

// Equivalent to: list<i32, i32>, list<i32, list<i32>>, etc.
// Error to: list<i32, i32, i32>
t:list<i32, ?> = ...;
```

#### Dictionary Type

A dictionary stores key-value pairs, mapping keys to values.

- If key/value is a basic type, each element in the vector should be considered as a single key/value
- If key/value is a list type, each cell in the list should be considered as a single key/value
- If key/value is a compound type other than list, its entirety is considered as a single key/value

```
DictType  = "dict" '<' Type ',' Type '>' ;
```

Dictionaries are formed from collections of keys and values using built-in function `@dict`.

```
a:str = ("a", "b", "c"):str;
b:str = ("Montreal", "Toronto", "Vancouver"):str;
c:dict<str, str> = @dict(a, b);
```

The above creates mappings: $a \rightarrow Montreal$, $b \rightarrow Toronto$, and $c \rightarrow Vancouver$.

#### Enumeration Type

An enumeration represents the indexing relationship between two vectors: enum keys and enum values. For each value, the enumeration stores the index of the same element in the keys vector. If no corresponding element is found, the length of the enum keys is stored.

```
EnumType  = "enum" '<' Type '>' ;
```

An enumeration type specifies the key and value type. 

```
// Defines an enumeration of i32 keys and values
enum<i32>
```

Enumerations are formed from vectors of keys and values using built-in function `@enum`.

```
a:i32 = (1, 2, 3):i32;
b:i32 = (3, 3, 1,2):i32;

// Forms an enumeration with internal index vector (2,2,0,1):i32
c:enum<i32> = @enum(a, b);
```

!!! note

    An enumeration type is commonly found in key/fkey schemas in relational database systems.

#### Table Types

A table consists of a non-empty list of columns. Each column has a name and a vector of homogeneous values. There are two kinds of tables: `table` and `ktable` (keyed table).

```
TableTypes  = "table" | "ktable" ;
```

1. Table: A normal table of columns
2. Keyed table: A table with key columns

Tables are formed from a vector of column symbols and a list of column values. The number of symbols and list cells must be equal.

```
student_id:i32 = (1, 2, 3):i32;
student_age:i8 = (10, 11, 9):i8;
student_grade:i8 = (9, 9, 9):i8;

tab_cols:sym = (`id, `age, `grade):sym;
tab_vals:list<?> = @list(student_id, student_age, student_grade);

tab:table = @table(tab_cols, tab_vals);
```

The resulting table `tab` can be viewed as follows:

| id | age | grade |
| :--| :---| :-----|
| 1  | 10  | 9     |
| 2  | 11  | 9     |
| 3  | 9   | 9     |

A keyed table is similar to a normal table and consists of a set of key columns and a set of non-key columns. A key column, as in relational databases, must have unique non-null values. The function `@ktable` creates a keyed table from two tables with the same number of rows. The columns from the first table become the key columns, and columns from the second table become the non-key columns.

```
a:table = ...;
b:table = ...;
c:ktable = @ktable(a, b);
```

| id (key) | age | grade |
| :--| :---| :-----|
| 1  | 10  | 9     |
| 2  | 11  | 9     |
| 3  | 9   | 9     |


!!! note

    - An empty table has no rows, but must have at least one column.
    - A keyed table must have at least one key column.
    - A keyed table with multiple key columns has *compound keys*.
    - The conversion between tables and keyed tables uses two built-in functions:
        - `@add_key`: Designates columns as keys.
        - `@remove_key`: Removes columns from keys. If all keys are removed, a normal table is returned.

### Value Ranges

Basic types have value ranges based on standard C conventions.  Numeric types are always signed.  

- `bool`: 0 or 1
- Numeric types depend on the number of bits
    + `i8`: -2^7^ to 2^7^-1
    + `i16`: -2^15^ to 2^15^-1
    + `i32`: -2^31^ to 2^31^-1
    + `i64`: -2^63^ to 2^63^-1
    + `f32`: 1.2E-38 to 3.4E+38 (precision: 6 decimal places)
    + `f64`: 2.3E-308 to 1.7E+308 (precision: 15 decimal places)
- A complex number is the combination of two floating point numbers (`f32`)
- Each date type has its own date-specific format (`YYYY-MM-DD T hh:mm:ss.ll`) and value range
    + `YYYY` (year): 1000 to 9999
    + `MM` (month): 01 to 12 (two digits required)
    + `DD` (day): 01 to 28/29/30/31
        * January - 31 days
        * February - 28 days (common year) or 29 days (leap year)
        * March - 31 days
        * April - 30 days
        * May - 31 days
        * June - 30 days
        * July - 31 days
        * August - 31 days
        * September - 30 days
        * October - 31 days
        * November - 30 days
        * December - 31 days
    + `hh` (hour): 00 to 23
    + `mm` (minute): 00 to 59
    + `ss` (second): 00 to 59
    + `ll` (millisecond): 000 to 999

Noted that an error occurs when a number exceeds the range of its  type.

```
2:bool    // Error
999:i8    // Error
```

### Type Conversions

Type conversions are performed with explicit casting. Only the following type conversions are permitted (any conversion not-listed is disallowed).

#### Integer

- An integer of narrower width may be cast to an integer of wider width (e.g. `i32` to `i64`).
- An integer of wider width may not be cast to a integer of narrower width.

#### Integer and Float

- An integer may be cast to a float. Loss of precision may occur (e.g. `i64` to `f32`).
- A float value may be cast to an integer by truncating it's decimal part.
    - Only `f32` to `i32`/`i64`, and `f64` to `i64` is allowed.

#### Integer and Char

- Integer and character values may not be cast to each other.

#### Float

- A float with lower precision may be cast to a float of higher precision.
- A float with higher precision may not be cast to a float of lower precision.

#### Boolean

- A boolean value may be cast to an integer and vice-versa.
    - Zero is false, non-zero is true

#### String and Symbol

- Strings may be cast to symbols and vice-versa.

### Illustrations

#### Basic types

```
                     contiguous
                      /
+-------+-------+-------+
|  35   |  79   | ...   |   (integers, for example)
+-------+-------+-------+
```

#### List

```
        +------+
        | list |
        +------+
         /  |    ... 
    +----+ +----+
    | c0 | | c1 |  ...   (cells)
    +----+ +----+
```

#### Dictionary

```
+-----+  map  +-------------+
| "a" |  -->  | "Montreal"  |
+-----+       +-------------+
| "b" |  -->  | "Toronto"   |
+-----+       +-------------+
| "c" |  -->  | "Vancouver" |
+-----+       +-------------+
| ... |  -->  | ........... |   (key --> value)
+-----+       +-------------+
```

#### Enumeration

```
        +---+---+---+
key     | 7 | 3 | 6 |                                  key
        +---+---+---+                                     \ 
                                                            enum
                                                          / 
        +---+---+---+---+---+         +---+---+---+---+---+
value   | 3 | 3 | 6 | 6 | 7 |  ---->  | 1 | 1 | 2 | 2 | 0 |  (indices)
        +---+---+---+---+---+         +---+---+---+---+---+
```

#### Table

```
+----+-----+-------+
|"Id"|"Age"|"Grade"|
+----+-----+-------+
  |      |       |
+---+  +----+  +---+
| 1 |  | 10 |  | 9 |
+---+  +----+  +---+
| 2 |  | 11 |  | 9 |
+---+  +----+  +---+
| 3 |  | 9  |  | 9 |   (columnar store)
+---+  +----+  +---+
```

#### Keyed table

```
+-----------ktable-----------+
|  +----+   +-----+-------+  |
|  |"Id"|   |"Age"|"Grade"|  |
|  +----+   +-----+-------+  |
|    |        |       |      |
|  +---+    +----+  +---+    |
|  | 1 |    | 10 |  | 9 |    |
|  +---+    +----+  +---+    |
|  | 2 |    | 11 |  | 9 |    |
|  +---+    +----+  +---+    |
|  | 3 |    | 9  |  | 9 |    |
|  +---+    +----+  +---+    |
| /* key */  /* non-keys */  |
+----------------------------+
```

## Shapes

HorseIR has two kinds of types: [basic types](#basic-types) and [compound types](#compound-types), which correspond to different shapes.

#### Basic types

A basic type is a simple type representing a collection of items with a
homogeneous type.  Therefore, the data of a basic type is stored in sequential
memory, called ^^vector^^.

An example of an integer vector which contains three integers: `(1,2,3):i64`:

```no-highligh
+-----+-----+-----+
|  1  |  2  |  3  |
+-----+-----+-----+
```

#### Compound Types

A compound type is a list-based type representing a collection of items which
may have non-homogeneous types.  A list consists of many cells.
A cell can be either a vector or another list.

An example of a list with two items `"abc":str` and `(1,2,3):i32` below:

```no-highligh
+--------+--------+
| cell 0 | cell 1 |
+--------+--------+
  /         \
+-----+  +-----+-----+-----+
|"abc"|  |  1  |  2  |  3  |
+-----+  +-----+-----+-----+
```

The type of the list can be `list<str,i32>` or `list<?>`.

### Shape Rules

Conventions

| Name       | Alias | Type     | Notes              |
|:----------:|:-----:|:--------:|:------------------:|
| scalar     | S     | vector   |length \( = \) 1    |
| vector     | V     | vector   |length \( \neq \) 1 |
| list       | L     | list     |two lists: L1, L2   |
| primitive  | op    | operator |builtin functions   |


#### <span id="monadic-elementwise">Monadic Elementwise</span>

```no-highlight
op(S) => S
op(V) => V
```
- Return the same length as the input operand

#### <span id="reduction">Reduction</span>

```no-highlight
op(S) => 1
op(V) => 1
```

- Reduction functions, such as `sum` and `min`, always return a scalar.

#### <span id="dyadic-elementwise">Dyadic Elementwise</span>

```no-highlight
op(S, S) => S
op(S, V) => V
op(V, S) => V
op(V, V) => V  //length agree on both sides
```

- Both operands operate on vector types

#### <span id="dyadic-list-based">Dyadic List Based</span>

Input with two lists: `L1` and `L2`.

```no-highlight
each_left (L1, L2) => L1
each_right(L1, L2) => L2
each_item (L1, L2) => L1  //length agree on both sides
```

- If neither L1 nor L2 is list, the list-based operation is ignored;
- If one side (left/right) is chosen and the corresponding operand is a list,
  the first-level items of the list are iterated in order;
- If one side (left/right) is chosen and the corresponding operand is **not** a list,
  the operand is processed as a whole.

#### <span id="shape-append">Shape Append</span>

```no-highlight
append(S, S) => V
append(S, V) => V
append(V, S) => V
append(V, V) => V
append(L, S) => L
append(L, V) => L
append(S, L) => L
append(V, L) => L
append(L, L) => L
```

- [append](/horseir/builtin/append): concatenate two items

#### <span id="shape-left">Shape Left</span>

```
op(LeftShape, RightShape) => LeftShape
```

- Return left shape

#### <span id="shape-right">Shape Right</span>

```
op(LeftShape, RightShape) => RightShape
```

- Return right shape

#### <span id="shape-left-value">Shape Left Value</span>

```
op(LeftShape, RightShape) => ReturnShape
```

- ReturnShape is determined by the value of the **left** parameter.





## Statements

A function or control structure body consists of a potentially empty list of statements. There are 2 kinds of statements, and one statement modifier (labels).

```
Block         = '{' { Statement } '}'
ControlBlock  = Statement | Block

Statement     = AssignStmt | ControlStmt | ExpressionStmt | VarDecl ;
```

### Assignments

Assign statements consist of left-hand side target variables and a right-hand side expression. In the case of multiple return values, more than one target variable must be present. Target variable may either be declared with their respective types or assigned.

```
AssignStmt  = VarList '=' Expression ';' ;

VarList     = Var { ',' Var } ;
Var         = Identifier [ ':' Type ] | Identifier '.' Identifier ;
```

Assignments copy the right-hand side if necessary (may be omitted if the expression is a function call).

### Control Statements

Both structured an unstructured control-flow are supported for conditional execution and jumps.

```
ControlStmt  = IfStmt    | WhileStmt | RepeatStmt | ReturnStmt |
               BreakStmt | ContinueStmt ;

Condition    = Operand ;
```

The condition of a control-flow statement must be a scalar value (i.e. a vector of 1 element). For convenience, two built-in functions `@any` and `@all` reduce a vector to a single value for conditional execution:

- `@any` returns true if any value in a vector is true
- `@all` returns true only if all values in a vector are true

#### If-ElseIf-Else Statements

An if-elseif-else statement provides conditional execution of blocks of code. The condition must be a boolean scalar, and the else-if and else clauses are optional.

```
IfStmt = "if" '(' Condition ')' ControlBlock [ "else" ControlBlock ]
```

#### While and Repeat Statements

A while loop executes its body until its condition evaluates to false. The condition must be a boolean scalar. A repeat statement executes the body a fixed number of iterations. The condition must be an integer scalar.

```
WhileStmt   = "while" '(' Condition ')' ControlBlock ;
RepeatStmt  = "repeat" '(' Condition ')' ControlBlock ;
```

<!--
### Labelled Statements

A labelled statement is used as a target for explicit unstructured control-flow.

```
LabelledStmt  = Identifier '>' Statement ;
```

!!! note

    - A label cannot be placed at the end of a block without an associated statement (see [stackoverflow](https://stackoverflow.com/questions/18535283)).

#### Goto Statements

There are two flavours of goto statements:

1. Standard goto with a single target
2. Conditional goto with a list of conditions. This corresponds to a switch statement.

```
GotoStmt = "goto" ( Identifier | '(' IdentifierList ')' '/' TargetList )

IdentifierList  = Identifier [ '.' Identifier ] { ',' Identifier [ '.' Identifier ] }
TargetList      = Identifier { ',' Identifier }
```

A conditional goto is given in the form `goto (A, B, C) / (LA, LB, LC, END)`. It execute conditions `A`, `B`, `C` until one evaluates to true and executes the corresponding label after the `/`. If all conditions are false, the `END` label is used.

```
module default {
    def main() {
        goto (A, B, C) / (LA, LB, LC, END);
     LA> ...  // if A is true
         goto END;
     LB> ...  // if A is false and B is true
         goto END;
     LC> ...  // if A,B are false and C is true
         goto END;
    END> ...  // if A,B,C are false
    }
}
```
-->

#### Return Statements

A return statement exits the function and optionally returns a list of values to its calling context.

```
ReturnStmt  = "return" [ { Operand { ',' Operand } ] ';' ;
```

#### Break and Continue Statements

A break statement exits a loop, whereas a continue statement jumps to the next iteration. Break and continue statements may optionally specify a label corresponding to a conditional statement.

```
BreakStmt     = "break" ';' ;
ContinueStmt  = "continue" ';' ;
```

!!! note

    `break` or `continue` can only appear inside a loop body (i.e. while and repeat)

<!-- - See [Java jump with a label](https://stackoverflow.com/a/19836620) -->

### Expression Statements

An expression statement evaluates a non-value expression. Either a built-in or user-defined function that has return value.

```
ExpressionStmt = Expression ';'
```

### Variable Declarations

A variable declaration binds variable names to their associated type without an initialization expression.

```
VarDecl = "var" Identifier [ { ',' Identifier } ] ':' Type ';' ;
```

## Expressions

Expressions evaluate to a value and may either be function calls, literals or identifiers.

```
Expression = FunctionCall | Operand | Cast ;
```

### Function Calls

Function calls consist of the function name (optionally fully qualified) and operand arguments. Function calls may not be nested.

```
FunctionCall  = FunctionId '(' [ Operands ] ')' ';'
FunctionId    = '@' Identifier [ '.' Identifier  ] ;
```

Function calls are pass-by-value and return by value.

### Operands

Operands, used either as parameters or return values, may only be identifiers or literals.

```
Operands  = Operand { ',' Operand } ;
Operand   = Identifier [ '.' Identifier ] | Literal ;
```

### Literals

Literals may either be function names, or vectors and consist of the value and associated type. Function literals may omit the declared type.

```
Literal  = FunctionLiteral | VectorLiteral ;

FunctionLiteral  = FunctionId [ ':' "func" ] ;
VectorLiteral    = Value ':' Type | '(' Value { ',' Value } ')' ':' Type ;
```

Values within a vector literal must correspond to the declared type (or be compatible, see [type conversions](#Type-Conversions)).

### Cast Expression

A type cast changes the type of the expression to the destination type. See [type conversions](#Type-Conversions) for more details.
```
Cast = "check_cast" '(' Expression ',' Type ')' ;
```

<!--
## Built-in Functions

Built-in functions are predefined under a built-in module, `Builtin`.  They can be used after the module is imported.

```
module default{
    import Builtin.*;
    def main():i32{
        ...
        A:i32 = @plus(...); // Builtin.plus
    }
}
```

### Length

A function `len` takes one argument and return its length.

```
len(i32)    // length for basic types
len(list)   // # of cells (1st level)
len(dict)   // # of mappings
len(enum)   // # of indices
len(table)  // # of rows
len(ktable) // # of rows
```

### List and Raze

A function `list` takes arbitrary number of arguments and returns one list, while another function `raze` destroys all cells and merges all elements with the same type into a new vector if possible.

Examples

```
A:list<?> = @list((1,2,3):i32, "hello":str);
B:list<?> = @list((1,2,3):i32, 5:i32);

a:? = @raze(A);  // returns an error
b:? = @raze(B);  // returns (1,2,3,5):i32
````

A similar function is `tolist` which converts a vector to a list with every single element in the vector become a cell in the returned list.

```
A:list<i32> = @tolist((1,2,3):i32);
// returns ((1:i32),(2:i32),(3:i32)):list<i32>
```

Note: A list with numeric types, such as `list<i32,f32>`, can be razed to `f32`.

### Print and Format

A function `print` takes one argument and prints the content of the argument to standard output.  A function `format` converts the content of its argument to a string.  The format of output is currently fixed without any possible customized formats.

- [x] support all types
- [x] precision control (`System.pp`)

```
System.pp:i32 = 10:i32;
@print(PI);  // 3.141592654

System.pp:i32 = 15:i32;
@print(PI);  // 3.14159265358979
```


### Arithmetic Functions

An arithmetic function takes one or two arguments.  The one with one argument is called unary function, and the one with two argument is called binary function.

Unary functions

<pre>
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/abs">abs</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/neg">neg</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/ceil">ceil</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/floor">floor</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/round">round</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/conj">conj</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/recip">recip</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/signum">signum</a>   
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/pi">pi</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/logic/#not">not</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/logic/#log">log</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/logic/#log2">log2</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/logic/#log10">log10</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/power/#sqrt">sqrt</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/power/#exp">exp</a>      
</pre>

Binary functions

<pre>
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/lt">lt</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/gt">gt</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/leq">leq</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/eq">eq</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/neq">neq</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/plus">plus</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/minus">minus</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/mul">mul</a>      
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/div">div</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/power#power">power</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/log#logb">logb</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/mod">mod</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/log#and">and</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/log#or">or</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/nand">nand</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/nor">nor</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/xor">xor</a>
</pre>



### Math Functions

Math functions operate on both real and complex numbers.

<pre>
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#cos">cos</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#sin">sin</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#tan">tan</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#acos">acos</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#asin">asin</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#atan">atan</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#cosh">cosh</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#sinh">sinh</a>
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#tanh">tanh</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#acosh">acosh</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#asinh">asinh</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/trig/#atanh">atanh</a>
</pre>

### Database-related Functions

HorseIR provides a set of database-related functions which designed for database queries, such as `join_index` for a join operation.

<pre>
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/group">group</a>           <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/join/#join_index">join_index</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/like">like</a>            <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/compress">compress</a>        <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/indexof">index_of</a>        
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/order">order</a>           <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/member">member</a>          <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/vector">vector</a>          <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/unique">unique</a>          <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/where">where</a>           
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/len">count</a>           <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/reduce/#sum">sum</a>             <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/reduce/#avg">avg</a>             <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/reduce/#min">min</a>             <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/reduce/#max">max</a>             
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/advanced/#enum">enum</a>            <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/advanced/#dict">dict</a>            <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/advanced/#table">table</a>           <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/advanced/#ktable">ktable</a>          <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/dbhelper/#keys">keys</a>            
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/dbhelper/#values">values</a>          <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/dbhelper/#meta">meta</a>            <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/dbhelper/#fetch">fetch</a>           <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/dbhelper/#column_value">column_value</a>    <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/dbhelper/#load_table">load_table</a>      
</pre>

### Helper Functions

A set of helper functions can be found below.

<pre>
<a href="#">print</a>           <a href="#">format</a>          <a href="#">match</a>           <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/indexing/#index">index</a>           <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/indexing/#indexa">index_a</a>         
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/substring">sub_string</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/list/#each">each</a>            <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/list/#each_item">each_item</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/list/#each_left">each_left</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/list/#each_right">each_right</a>      
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/append">append</a>          <a href="#">any</a>             <a href="#">all</a>             
</pre>


Date and time

<pre>
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#date">date</a>            <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#date-year">date_year</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#date-month">date_month</a>      <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#date-day">date_day</a>        <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#time">time</a>            
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#time-hour">time_hour</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#time-minute">time_minute</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#time-second">time_second</a>     <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#time-mill">time_mill</a>       <a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#datetime-add">datetime_add</a>    
<a href="http://www.sable.mcgill.ca/~hanfeng.c/horse/docs/horseir/builtin/date/#datetime-sub">datetime_sub</a>    
</pre>

==TODO: this needs to be added to the function descriptions, not in this doc==
==TODO: add two built-in functions: real (return real part of complex number) and imag (return imaginary part of complex numer)==

++Complex and real++

- If any input is a real number, the result must be either a real or an error (e.g. `asin(2:i32)` returns an error).
- If any input is a complex number, the result is a complex number (e.g. `asin(2+0i)` returns a complex number `1.570796327-1.316957897i`).

----

<!-- comment below
## Notes for the discussion on May 6, 2019

- Types
    + If there is one type in the list, `?` means anything
    + If there are more than one type, `?` means one type
- mutltiple returns
- date's range should be larger
- no SSA
- no free blocks (`{}`)
- literals: 
- break goto 
- no label at the end of a method (see [stackoverflow](https://stackoverflow.com/questions/18535283))
- `@eq[:func]`: func is optional

LiteralBool   = Bools     ':' "bool" ;
LiteralI8     = Integers  ':' "i8"   ;
LiteralI16    = Integers  ':' "i16"  ;
LiteralI32    = Integers  ':' "i32"  ;
LiteralI64    = Integers  ':' "i32"  ;
LiteralF32    = Floats    ':' "f32"  ;
LiteralF64    = Floats    ':' "f64"  ;
LiteralClex   = Complexes ':' "complex" ;
LiteralChar   = Chars     ':' "char" ;
LiteralStr    = Strings   ':' "str"  ;
LiteralSym    = Symbols   ':' "sym"  ;
LiteralDt     = Datetimes ':' "dt"   ;
LiteralDate   = Dates     ':' "d"    ;
LiteralMonth  = Months    ':' "m"    ;
LiteralMinute = Minutes   ':' "w"    ;
LiteralSecond = Seconds   ':' "v"    ;
LiteralTime   = Times     ':' "t"    ;

Bools     = Bool    | '(' [ Bool    { ',' Bool    } ] ')' ;
Integers  = Integer | '(' [ Integer { ',' Integer } ] ')' ;
Floats    = Float   | '(' [ Float   { ',' Float   } ] ')' ;
Complexes = Complex | '(' [ Complex { ',' Complex } ] ')' ;
Symbols   = { Symbol } ;
Datetimes = Datetime | '(' [ Datetime { ',' Datetime } ] ')' ;
Months    = Month    | '(' [ Month    { ',' Month    } ] ')' ;
Minutes   = Minute   | '(' [ Minute   { ',' Minute   } ] ')' ;
Seconds   = Second   | '(' [ Second   { ',' Second   } ] ')' ;
Times     = Time     | '(' [ Time     { ',' Time     } ] ')' ;
-->

