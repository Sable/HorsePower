## Types

HorseIR has a rich set of types, including basic and advanced types.  In order
to exploit precise types information, it adopts explicit types with a strict
type system.

### Type specifications

Basic types

|  Name     |  Type     | Byte   |  Description                  |
| :---------| :---------| :------| :-----------------------------|
| boolean   | bool      | 1 (Bit)| `0` (false) and `1` (true)    |
| byte      | i8        | 1      | Basic unit                    |
| string    | str       | 2      | Unicode                       |
| short     | i16       | 2      | Short integer                 |
| int       | i32       | 4      | Integer                       |
| long      | i64       | 8      | Long integer (default, x64)   |
| float     | f32       | 4      | Single precision              |
| double    | f64       | 8      | Double precision              |
| complex   | complex   | 16     | Complex number                |
| symbol    | sym       | 8      | Symbol, but stored in integer |
| function  | func      | 8      | Function literal              |
| month     | m         | 4      | Month (YYYY.MM)               |
| date      | d         | 4      | Date (YYYY.MM.DD)             |
| date time | z         | 8      | Date time                     |
| minute    | u         | 4      | Minute (hh.mm)                |
| second    | v         | 4      | Second (hh.mm.ss)             |
| time      | t         | 4      | Time (hh.mm.ss.ll)            |


Advanced types

|  Name      |  Type     |  Description                  |
| :----------| :---------| :-----------------------------|
| list       | list      | Collection of items           |
| dictionary | dict      | Key to value                  |
| enumeration| enum      | Mapping                       |
| table      | table     | Collection of columns         |
| keyed table| ktable    | Two normal tables             |

See more about [type examples](type_examples.md).


### Type casting

Widening primitive conversions

```
x:i32 = (i32) 0:i16;
```

A tentative table (**Incomplete**)

| left   |  i8      | i16     | i32     | i64     |
| :------| :--------| :-------| :-------| :-------|
| i32    | &#10003; | &#10003;| &#10003;| &#10008;|


Narrowing primitive conversions

- Not allowed.


### Type checking

### Examples

bool

```
0:b                     //false
1:b                     //true
0100001:b               //list of T/F
```

string

```
'abc':str               //string 'abc'
```

short/int/long/float/double/complex

```
12:i16                  //short 12
12:i32                  //int 12
12:i64                  //long 12
3.14:f32                //float 3.14
3.14:f64                //double 3.14
2.3+4.5i:complex        //complex
```

symbol

```
`IBM                    //symbol
`AAPL`GOOGL             //list of symbols
```

function

```
@add                    //function literal
@substract:func         //'-'
```

month/date/datetime/minute/second/time

```
2012.11:m                 //month
2012.11.02:d              //date
2012.11.02T10.05.12.001:z //date time
10.05:u                   //minute
10.05.12:v                //second
10.05.12.001:t            //time
```

list

```
list(1 2 3; 4 5 6)        //list<i64>
list(1 2 3; 'abc')        //list<?>
```

dict/table

```
dict(`FirstName, list(`hf`hongji)) //dictionary
list(dict0,dict1,...)              //table
list(table0, table1)               //keyed table
```

