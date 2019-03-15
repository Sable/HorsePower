!!! note "HorseIR types"
    HorseIR has a rich set of types, including basic and advanced types.  In order
    to exploit precise types information, it adopts explicit types within a strict
    type system.

### Type Specifications

^^Basic types^^

| Name      | Alias    | Short | Byte   | Description                   |
| :---------| :--------| :-----| :------| :-----------------------------|
| boolean   | bool     | B     | 1\*    | `0` (false) and `1` (true)    |
| small     | i8       | J     | 1      | Half short integer or char    |
| short     | i16      | H     | 2      | Short integer                 |
| int       | i32      | I     | 4      | Integer                       |
| long      | i64      | L     | 8      | Long integer (default, x64)   |
| float     | f32      | F     | 4      | Single precision              |
| double    | f64      | E     | 8      | Double precision              |
| complex   | complex  | X     | 16     | Complex number                |
| char      | char     | C     | 1      | Half short integer or char    |
| symbol    | sym      | Q     | 8      | Symbol, but stored in integer |
| string    | str      | S     | 8      | String                        |
| month     | month    | M     | 4      | Month (YYYY.MM)               |
| date      | date     | D     | 4      | Date (YYYY.MM.DD)             |
| date time | dt       | Z     | 8      | Date time                     |
| minute    | minute   | W     | 4      | Minute (hh:mm)                |
| second    | second   | V     | 4      | Second (hh:mm:ss)             |
| time      | time     | T     | 4      | Time (hh:mm:ss.ll)            |
| function  | func     |       | 8      | Function literal              |

<small>\* To-do: 1-bit bool will be supported.</small>

^^Compound types^^

| Name       | Alias     | Short | Description                   |
| :----------| :---------| :-----| :-----------------------------|
| list       | list      | G     | Collection of items           |
| dictionary | dict      | N     | Key to value                  |
| enumeration| enum      | Y     | Mapping                       |
| table      | table     | A     | Collection of columns         |
| keyed table| ktable    | K     | Two normal tables             |

See more about [type examples](#examples).


### Type Casting

Widening primitive conversions

```no-highlight
x:i32 = check_cast(0:i16, i32);
```

A tentative table (**Incomplete**)

| left   |  i8      | i16     | i32     | i64     |
| :------| :--------| :-------| :-------| :-------|
| i32    | &#10003; | &#10003;| &#10003;| &#10008;|


Narrowing primitive conversions

- Not allowed.

### Type Checking

!!! note "To-do"

    Type checking has been implemented.  The update of this section will be coming soon.

### Examples

**bool**

```no-highlight
0:bool                  //false
1:bool                  //true
(0,1,0,0,0,0,1):bool    //list of T/F
```

**char**

```no-highlight
'abc':char              //char 'abc'
```

**string**

```no-highlight
"abc":str               //single string, 'abc'
("a","b","c"):str       //3 strings
```

**short/int/long/float/double/complex**

```no-highlight
12:i16                  //short 12
12:i32                  //int 12
12:i64                  //long 12
3.14:f32                //float 3.14
3.14:f64                //double 3.14
2.3+4.5i:complex        //complex
```

**symbol**

```no-highlight
`IBM                    //symbol
(`AAPL,`GOOGL)          //list of symbols
```

**function**

```no-highlight
@add                    //function literal
@substract:func         //'-'
```

**month/date/datetime/minute/second/time**

```no-highlight
2012.11:m                 //month
2012.11.02:d              //date
2012.11.02T10:05:12.001:z //date time
10:05:u                   //minute
10:05:12:v                //second
10:05:12.001:t            //time
```

**list**

```no-highlight
list((1,2,3); (4,5,6))    //list<i64>
list((1,2,3); 'abc')      //list<?>
```

**dict/table**

```no-highlight
dict(`FirstName, list((`hf,`hongji))) //dictionary
list(dict0,dict1,...)                 //table
list(table0, table1)                  //keyed table
```


