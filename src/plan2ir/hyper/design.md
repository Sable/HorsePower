# Structure of Plan-to-HorseIR Translator

## Overview


## Basics

### Constant

### Variable

A variable in expression can be defined below.

```
{
    "iu"        : "l_discount",
    "expression": "iuref" 
}
```

### Expression

A valid expression consists of constants and variables.

```
expression = constant
           | variable ;
```


## Structure Design

### Main Body

The plan as an input with JSON format has two parts: **header** and **plan**.

- The "header" defines the names and variables of a return table
- The "plan" describes the details of the plan

```
{
    "header": {...}
    "plan"  : {...}
}
```

### Header

A header node consists of a list of strings, more precisely an even number of
strings.  It comes with pairs as `{name, variable}`. A pair means the name of a
column name and its column value.  All pairs make up of a return table.

```
"header": [
    "l_returnflag"  , "v", 
    "l_linestatus"  , "v2", 
    "sum_qty"       , "v6", 
    "sum_base_price", "v8", 
    "sum_disc_price", "v10", 
    "sum_charge"    , "v27", 
    "avg_qty"       , "v7", 
    "avg_price"     , "v9", 
    "avg_disc"      , "v29", 
    "count_order"   , "v30"
]
```

> See function `scanHeader`.

### Plan

The plan node is the body of the plan which contains the details of execution
in steps.  It is formed as a tree and its execution order is bottom-to-top and
right-to-left.  Therefore, the starting node is the right-most leaf node.

There are a couple of operators defined in non-leaf nodes.

- `tablescan`    : scan table with conditions
- `tempscan`     : scan a temporary table
- `groupbyscan`  : scan a table after groupby
- `groupby`      : a general group by operator
- `sort`         : a sorting operator
- `select`       : load a table
- `join`         : a typical database join for one or more columns
- `groupjoin`    : a mix of groupby and join
- `leftantijoin` : left antijoin
- `rightantijoin`: right antijoin
- `leftsemijoin` : left semijoin
- `rightsemijoin`: right semijoin
- `map`          : unclear
- `magic`        : unclear
- `earlyprobe`   : unclear

*Note: Extra operators are possible, such as leftouterjoin and leftmarkjoin.
However, they are only existed in non-optimal plans.*


### Tablescan

A tablescan node scans all loaded columns, applies all conditions or restrictions, and returns all qualified rows.  It provides input data for subsequent plan execution.

```
"plan":{
    "operator"    : "tablescan"
    "from"        : <a table>
    "values"      : <input columns>
    "operatorId"  : <order of execution>
    "restrictions": <conditions>
    "cardinality" : <an estimated size>
    "tid"         : <unclear>
    "tupleFlags   : <unclear>
    "tableOid"    : <unclear>
}
```

The `restrictions` node consists of a set of conditions.

```
"restricitons": [
    { condition1 },
    { condition2 },
    ...
]
```

A condition specifies which columns to read and what expressions to execute.


```
{
    "attribute": <index of columns in the 'values' node, starting from 0>
    "mode"     : <short alias functions>
    "value"    : <right operand if mode is a unary function else left operand>
    "value2"   : <right operand if mode is a binary function (optional)>
}
```

A table of 'mode' functions to HorseIR builtin functions.

```
'['  : 'geq'
'('  : 'gt'
']'  : 'leq'
')'  : 'lt'
'<'  : 'lt'
'>'  : 'gt'
'='  : 'eq'
'>=' : 'geq'
'<=' : 'leq'
'<>' : 'neq'
'is' : 'eq' 
```

> See function `scanTablescan`.

### Tempscan

A tempscan node

```
{
    "operator": "tempscan"
}
```

### Groupbyscan

```
{
    "operator": "groupbyscan"
}
```

### Groupby

A groupby node

```
{
    "operator"    : "groupby"
    "input"       : <input data>
    "values"      : [ value1, value2, ... ]
    "aggregates"  : [ aggregation operations ]
    "operatorId"  : <constant>
    "cardinality" : <constant>
}
```

A `value` node

```
"value": {
    "right"     : <expression>
    "expression": <function name>
    "left"      : <expression>
}
```

### Sort

A sort node 

```
{
    "operator"    : "sort"
    "cardinality" : <constant>
    "criterion"   : [ criteria1, criteria2, ... ]
}
```

A criteria

```
{
    "nullFirst" : <true / false>
    "descending": <true / false>
    "value"     : <constant / variable>
}
```

### Map

### Select

A select node 

```
```

### Earlyprobe

### Join

### Groupjoin

### Leftantijoin

### Rightantijoin

### Leftsemijoin

### Rightsemijoin


