### HyPer Execution Plans

In this page we document HyPer's execution plans in the following sections

### Fetch a Plan

Access HyPer online, type your query, and press F12 (inspect)

[http://hyper-db.de/interface.html#](http://hyper-db.de/interface.html#)

Select the `Internet` tab to download a JSON file which contains the execution plan.

### SQL Plans in JSON

A plan can be optimized through several steps.  A JSON file contains all steps
from an unoptimized plan to an optimized plan as follows.

1. Unoptimized Plan
1. Unnesting
1. Predicate Pushdown
1. Operator Reordering
1. Physical Operator Mapping
1. Optimized Plan

### HorseIR Translator

**JSON attributes**

- `opt` for 'optimized plan', and `unopt` for 'unoptimized plan'

**Convensions**

- `{}`: repeat, similar to [] in JSON
- `[]`: option
- `|` : or
- `"string"`: a literal string token `string`
- `("token")next`: if the `token` is matched, then match `next`

**LL-based grammar**

```java
/*
 * terminals: integer, string_list, id, string
 * *_id = id
 */
plan          ::= ("plan")plan_body ("header")header
;
plan_body     ::= input
;
header        ::= { name_id alias_id }
;
plan_common   ::= ("operatorId")integer [ ("cardinality")integer ]
;
input         ::= plan_common sub_input
;
sub_input     ::= ("operator")"tablescan"     scanTableScan 
                | ("operator")"tempscan"      scanTempScan
                | ("operator")"groupbyscan"   scanGroupbyScan
                | ("operator")"groupby"       scanGroupby       ("input")input
                | ("operator")"sort"          scanSort          ("input")input
                | ("operator")"map"           scanMap           ("input")input
                | ("operator")"select"        scanSelect        ("input")input
                | ("operator")"earlyprobe"    scanEarlyProbe    ("input")input
                | ("operator")"temp"                            ("input")input
                | ("operator")"join"          scanJoin          ("left")input ("right")input
                | ("operator")"groupjoin"     scanGroupJoin     ("left")input ("right")input
                | ("operator")"leftantijoin"  scanLeftAntiJoin  ("left")input ("right")input  /* q21 */
                | ("operator")"rightantijoin" scanRightAntiJoin ("left")input ("right")input  /* q16 */
                | ("operator")"leftsemijoin"  scanLeftSemiJoin  ("left")input ("right")input
                | ("operator")"rightsemijoin" scanRightSemiJoin ("left")input ("right")input
;
scanGroupby   ::= ("values")values ("aggregates")aggregates
;
scanSelect    ::= ("condition")condition
;
scanTableScan ::= ("segment")integer ("from")table_id \
                  ("values")values ("tid")iu ("tableOid")iu_special \
                  ("tupleFlags")string_list ("restrictions")restrictions \
                  [ ("residuals")residuals ]
;
scanSort      ::= ("criterion")criterion [ ("count")count ]
;
scanMap       ::= ("values")values
;
scanJoin         ::= ("method")method \
                     ("singleMatch")single_match ("condition")condition [ ("magic")magic ]
;
scanLeftSemiJoin ::= scanJoin
;
scanRightSemiJoin::= scanJoin
;
scanLeftAntiJoin ::= scanJoin
;
scanRightAntiJoin::= scanJoin
;
scanGroupJoin    ::= ("leftKey")expressions ("rightKey")expressions ("compareTypes")types \
                     ("leftExpressions")expressions ("rightExpressions")expressions \
                     ("leftCollates")string_list ("rightCollates")string_list \
                     ("leftAggregates")aggregates ("rightAggregates")aggregates \
                     ("semantic")"outer"
;
commonScan       ::= ("source")integer ("output")output
;
scanTempScan     ::= commonScan
;
scanGroupbyScan  ::= commonScan
;
scanLeftOuterJoin::= ("condition")condition ("magic")magic
;
scanLeftMarkJoin ::= ("condition")condition
;
scanEarlyProbe   ::= ("values")string_list ("builder")integer
;

/* expressions */
values        ::= { value }
;
value         ::= expressions
                | ("iu")iu ("name")target_id
                | ("iu")iu ("value")expressions   /* q14 */
;
iu            ::= id [ type ]
;
iu_special    ::= id "RegClass"
;
aggregates    ::= { aggr_item }
;
aggr_item     ::= ("source")integer ("operation")operation ("iu")iu
;
restrictions  ::= { restrict_cell }
;
restrict_cell ::= ("attribute")integer ("mode")mode ("value")value [ ("value2")value ]
;
expressions   ::= ("expression")"comparison" ("mode")mode ("left")expressions ("right")expressions
                | ("expression")"quantor"    ("mode")"=some" ("arguments"){ expressions }
                | ("expression")expr_argn    ("arguments"){ expressions }
                | ("expression")expr_arg2    ("left")expressions ("right")expressions
                | ("expression")"const"      ("value")const_value
                | ("expression")"lookup"     ("input")var_value ("values")const_value
                | ("expression")"isnotnull"  ("input")var_value
                | var_value
;
const_value   ::= ("type")type ("value")string
;
var_value     ::= ("expression")"iuref" ("iu")iu
;
criterion     ::= ("nullFirst")isNullFirst ("descending")isDesc ("value")value
;
residuals     ::= { expressions }
;
condition     ::= expressions
;
magic         ::= input
;
output        ::= { source_target }
;
source_target ::= ("source")id ("target")iu
;

/* constants */
types         ::= { type }
;
type          ::= type_basic [ "nullable" ]
;
type_basic    ::= "Char1"
                | "Char" integer 
                | "BigInt"
                | "Varchar" [ integer ]
                | "Integer"
                | "Date"
                | "Numeric" integer integer
                | "Bool"
;
operation     ::= "keep"
                | "count"
                | "sum"
                | "avg"
                | "min"
                | "max"
                | "any"           /* q18 */
                | "countdistinct" /* q16 */
;
method        ::= "indexnl"
                | "hash"
                | "bnl"           /* q22 */
;
mode          ::= "="
                | "[)"
                | "[]"
                | "(]"
                | ">"
                | "<"
                | "<>"
                | "=some"  /* q19 */
                | ">="
                | "<="
;
expr_argn     ::= "and"
                | "or"
                | "like"
                | "substring"
                | "between"
;
expr_arg2     ::= "mul"
                | "sub"
                | "div"
;
```

### Pre-compute Information

char to symbol

```no-highlight
nation
    - n_name: char(25)

region
    - r_name: char(25)

part
    - p_brand: char(10)
    - p_type: varchar(25)
    - p_container: char(10)

supplier
    - <None>

customer
    - c_name: varchar(25)
    - c_mktsegment: char(10)

orders
    - o_orderpriority: char(15)

lineitem
    - l_shipinstruct: char(25)
    - l_shipmode: char(10)

```


### Rules for Operations

#### Join

Join condition

    (A op B) join_logic (C op D)

equi_join

    - one-column or muli-column join
    - op: '=', '<>', '<', '<=', '>', '>=', or 'is'
    - join_logic: 'and' or 'or'

left_anti_join

    - one-column or muli-column join
    - op: '=' and '<>'
    - join_logic: 'and'

