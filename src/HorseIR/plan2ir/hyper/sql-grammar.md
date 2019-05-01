
## SELECT Expression

```nohighlight
select_statement   ::= "SELECT" [ set_operation ] select_expr_list
                        "FROM" table_expression
                        [ "WHERE"    expression ]
                        [ "GROUP BY" expression_list ]
                        [ "ORDER BY" expression_list ]
                        [ "HAVING"   expression ]
                        [ "LIMIT"    literal_int ]

/* SELECT */
set_operation      ::= "DISTINCT"

select_expr_list   ::= select_expression { "," select_expression }

select_expression  ::= "*"
                     | term [ "AS" column_alias ]

column_alias       ::= name

/* FROM */
table_expression   ::= table_name_expr { "," table_name_expr }

table_name_expr    ::= table_name [ "AS" table_alias ]

table_name         ::= name

expression_list    ::= expression { "," expression }

/* WHERE / GROUP BY / HAVING */
expression         ::= and_condition { "OR" and_condition }

and_condition      ::= condition { "AND" condition }

condition          ::= operand
                     | operand compare_op operand
                     | operand [ "NOT" ] "IN" operand     /* constant_operand */
                     | operand [ "NOT" ] "LIKE" operand
                     | operand [ "NOT" ] "BETWEEN" operand "AND" operand
                     | operand "IS" [ "NOT" ] "NULL"
                     | "(" expression ")"
                     | "NOT" expression

operand            ::= term [ arith_op term ]

arith_op           ::= "+" | "-" | "*" | "/"

compare_op         ::= "<>"
                     | "<="
                     | ">="
                     | "="
                     | "<"
                     | ">"
                     | "!="

expr_stmt          ::= term
                     | const_value

const_value        ::= literal_int
                     | literal_float
                     | literal_date
                     | literal_string  /* todo */
                     | literal_boolean /* todo */
                     | literal_null    /* todo */

literal_date       ::= "date" "'" string "'" arith_op "'" literal_int "'" "day" "(" literal_int ")"

literal_int        ::= digit1 {digit}  /* need to check if 01 is valid */

literal_float      ::= literal_int "." {digit}  /* add scientific notation ? */

term               ::= name_id
                     | const_value
                     | case_stmt
                     | case_when_stmt
                     | function_stmt
                     | "(" operand ")"

function_stmt      ::= basic_func_op term

basic_func_op      ::= "AVG"
                     | "SUM"
                     | "COUNT"
                     | "MIN"
                     | "MAX"

cast_stmt          ::= "CASE" term "WHEN" expression "THEN" term
                       [ "ELSE" expression ] "END"

cast_when_stmt     ::= "CASE" "WHEN" expression "THEN" term
                       [ "ELSE" expression ] "END"

name               ::= name_id
                     | "\"" string "\""

name_id            ::= letter { letter | "_" | digit }

digit1             ::= [1-9]

digit              ::= [0-9]

string             ::= anything
```

### Sources

- SQL 1992 standard
- SQL 1999 standard
- TPC-H queries (e.g. [q1](https://github.com/Sable/HorsePower/blob/master/docs/tpch/q1.md))
- [SQL grammar reference](https://forcedotcom.github.io/phoenix/) (maybe not SQL-92 compliant)




