### Name

There are a couple of senarios we may need different names.

1. [Module names](#module)
1. [Method names](#method)
1. [Variable names](#variable)
1. [Type names](#type)

#### <p id="module">Module names</p>

A module name consists of a standard name: starting with a letter; following
with letters, numbers, hyphens and underscores; ending with a letter or a
number. In EBNF, it can be described as follows.

```
name   = letter { {hyphen} alpha }
alpha  = letter | digit
hyphen = "-" | "_"
```

The default name of a module is `default`.

```
module default{
    ...
}
```

**Note:** any method declared without a module around it, is considered as a
member of the default module.

#### <p id="method">Method names</p>

A method name is the same as the module name in the method definition.
However, when it is invoked from a method outside the current module, a
compound name is used.  Since different modules may have the same method
names, using a compound name can resolve such problem by giving both module
and method names.

A compound name is concatenated by a dot (".").

```
compound_name = name "." name
```

#### <p id="variable">Variable names</p>

A variable name should follow the same definition of a module name.  In fact, it usually comes with an associated type in a variable assignment.

```
statement = name type assign expression
```

#### <p id="type">Type names</p>

There are two kinds of types: a fixed-size type and a dynamic-size type.

For a fixed-size type, it usually comes with a number which indicates how many bits should be allocated.

```
i8   //byte
i16  //short
i32  //int
i64  //long
f32  //float
f64  //double
``

Also, a fixed-size type can be a meaningful name.

```
bool         //boolean
complex      //complex number
sym          //symbol
m/d/z/u/v/t  //date time
```

For a dynamic-size type, it usually comes with a mixed type with unknown size statically.

```
type_list    ::= "list" "<" type ">"
type_dict    ::= "dict" "<" type, type ">"
type_enum    ::= "enum" "<" type ">"
type_table   ::= "table"
type_ktable  ::= "ktable"
```


