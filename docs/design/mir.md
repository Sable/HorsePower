# Horse IR  - MIR

```java
mir_program      ::= mir_module
                   | mir_expression

mir_module       ::= "module" ' module_name ' "{" module_body  "}"

module_name      ::= identifier
module_body      ::= module_shapes
                   | module_methods
                   | module_expression

module_shapes    ::= {shape_id ":" shape_info}
shape_id         ::= identifier
shape_info       ::= "<" shape_dim {"," shape_dim} ">"
shape_dim        ::= literal_int
                   | identifier
                   | id_dot

module_methods   ::= "def" method_name parameter_list method_body

method_name      ::= identifier

parameter_list   ::= "(" parameter_name { "," parameter_name } ")"
parameter_name   ::= identifier

method_body      ::= "{" statement_list "}"

/* main */
statement_list    ::= { statement }
statement         ::= empty
                    | expression

statement_flow    ::= "return" | label
label             ::= id ":"
goto              ::= "goto" label

expression        ::= [statement_flow] core_expression
                    | goto
core_expression   ::= variable       [ assign sub_expression ]
                    | variable_index [ assign variable ]

sub_expression    ::= function_call
                    | primitive_call
                    | variable_index
                    | operand
                    | literal

function_call     ::= module_name "." id "(" argument_list ")"
primitive_call    ::= [operand] primitive operand
operand           ::= id

variable          ::= id { "." id }
variable_index    ::= variable { "[" index_cell  "]" }
index_cell        ::= empty
                    | operand
/*
 * literal includes:
 *   int, float, double, list ...
 * empty means:
 *   <empty string>
 */
```
