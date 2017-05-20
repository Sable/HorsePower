## Examples of types

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


