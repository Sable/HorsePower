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