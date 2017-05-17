grammar MIR ;

// Export Grammar
program : (module | module_body)* ;

module : 'module' name '{' module_body* '}' ;
module_body : method
            | global_var
            ;
method : 'def' name  '(' method_decal_arg_list ')' ':' type '{' statement* '}' ;
method_decal_arg_list :
                      | name ':' type (',' name ':' type)* ;
global_var : 'def' name ':' type (';')? ;

statement : ('[' name ']')? statement_core ';' ;
statement_core : name ':' type '=' expression
               | compound_name ':' type '=' expression
               | 'nop'
               | 'return' (name | compound_name | literal)
               | 'goto' '[' name ']' (name | compound_name | literal_bool)?
               ;

name : (ID | 'i' | 'j' | 'm' | 'd' | 'z' | 'u' | 'v' | 't');
compound_name : COMPOUND_ID ;
operand : compound_name
        | name
        | literal
        ;

method_call : compound_name '(' method_call_arg_list ')'
            | name '(' method_call_arg_list ')'
            | literal_function '(' method_call_arg_list ')'
            ;
method_call_arg_list :
                     | operand (',' operand)* ;

expression : (method_call | operand)
           | '(' type ')' (method_call | operand)
           | 'checktype' '(' (method_call | operand) ',' type ')'
           | 'checkcast' '(' (method_call | operand) ',' type ')'
           ;

literal : literal_bool
        | literal_char
        | literal_integer
        | literal_float
        | literal_complex
        | literal_symbol
        | literal_time
        | literal_function
        | literal_table
        | literal_ktable
        | literal_string
        | literal_list
        | literal_dict
        ;

literal_list : '[' literal_list_internal ']' (':' listType=type_list)? ;
literal_list_internal :
                      | literal ((',' | ';')? literal)* ;

literal_dict : '{' literal_dict_internal '}' (':' dictType=type_dict)? ;
literal_dict_internal :
                      | literal_dict_pair ((',' | ';')? literal_dict_pair)* ;
literal_dict_pair : key=literal '->' value=literal ;

literal_complex : opReal=('+' | '-')? real=(LITERAL_FLOAT | LITERAL_INTEGER)
                  ':' 'complex'
                | opIm  =('+' | '-')? im  =(LITERAL_FLOAT | LITERAL_INTEGER)
                  ('i' | 'j') ':' 'complex'
                | opReal=('+' | '-')? real=(LITERAL_FLOAT | LITERAL_INTEGER)
                  opIm  =('+' | '-')  im  =(LITERAL_FLOAT | LITERAL_INTEGER)?
                  ('i' | 'j') ':' 'complex'
                | opIM  =('+' | '-')? ('i' | 'j') ':' 'complex'
                ;
literal_bool    : op=('+' | '-')? value=LITERAL_INTEGER ':' 'bool' ;
literal_char    : value=LITERAL_CHAR (':' 'char')? ;
literal_integer : op=('+' | '-')? value=LITERAL_INTEGER ':'
                  valueType=('i8'  | 'i16' | 'i32' | 'i64') ;
literal_float   : op=('+' | '-')? value=(LITERAL_FLOAT | LITERAL_INTEGER) ':'
                  valueType=('f32' | 'f64') ;
literal_symbol  : value=LITERAL_SYMBOL (':' 'sym')? ;
literal_time    : value=LITERAL_T_GROUP_2  ':' 'm'
                | value=LITERAL_T_GROUP_3  ':' 'd'
                | value=LITERAL_T_GROUP_7 (':' 'z')?
                | value=LITERAL_T_GROUP_2  ':' 'u'
                | value=LITERAL_T_GROUP_3  ':' 'v'
                | value=LITERAL_T_GROUP_4 (':' 't')?
                ;
literal_function: value=LITERAL_FUNCTION (':' type_func)? ;
literal_table   : value=ID ':' 'table' ;
literal_ktable  : value=ID ':' 'ktable' ;
literal_string  : value=LITERAL_STRING (':' 'str')? ;

type : scalarType=( 'bool'    |
                    'char'    |
                    'i8'      | 'i16'     | 'i32'     | 'i64'     |
                    'f32'     | 'f64'     |
                    'complex' |
                    'sym'     |
                    'm' | 'd' | 'z' | 'u' | 'v' | 't' |
                    'str'     |
                    'table'   | 'ktbale'                                       )
     | wildcardType='?'
     | listType=type_list
     | dictType=type_dict
     | enumType=type_enum
     | funcType=type_func
     ;

type_list : 'list' '<' interal=type '>' ;
type_dict : 'dict' '<' key=type ',' value=type '>'
          ;
type_enum : 'enum' '<' interal=type '>' ;
type_func : 'func' '<' ':' type '>'
          | 'func' '<' '...' ':' type '>'
          | 'func' '<' type (',' type)* ':' type '>'
          | 'func' '<' type (',' type)* ',' '...' ':' type '>'
          ;

fragment OCT_CHARACTER     : [0-7] ;
fragment OCT_GROUP_3       : OCT_CHARACTER OCT_CHARACTER OCT_CHARACTER ;
fragment HEX_CHARACTER     : [0-9a-fA-F] ;
fragment HEX_GROUP_2       : HEX_CHARACTER HEX_CHARACTER  ;
fragment HEX_GROUP_4       : HEX_GROUP_2 HEX_GROUP_2 ;
fragment HEX_GROUP_8       : HEX_GROUP_4 HEX_GROUP_4 ;
fragment ESCAPE_CHARACTERS : ('a'  | 'b' | 'f' | 'n' | 'r' | 't' | 'v' | '\\' |
                              '\'' | '?' | '"' ) ;
fragment ESCAPE_SEQUENCE   : '\\' (ESCAPE_CHARACTERS              |
                                   OCT_GROUP_3                    |
                                   'x' HEX_GROUP_2                |
                                   'U' HEX_GROUP_8                |
                                   'u' HEX_GROUP_4                )
                           ;
fragment FRAGMENT_ID       : [a-zA-Z_][a-zA-Z0-9_]* ;

fragment FRAGMENT_INTEGER  : ('0' | [1-9][0-9]*) ;
fragment FRAGMENT_FLOAT    : (FRAGMENT_INTEGER)? '.' [0-9]+ ;

LITERAL_T_GROUP_2    : [0-9]+ '.' [0-9]+ ;
LITERAL_T_GROUP_3    : [0-9]+ '.' [0-9]+ '.' [0-9]+ ;
LITERAL_T_GROUP_4    : [0-9]+ '.' [0-9]+ '.' [0-9]+ '.' [0-9]+ ;
LITERAL_T_GROUP_7    : [0-9]+ '.' [0-9]+ '.' [0-9]+ 'T'
                       [0-9]+ '.' [0-9]+ '.' [0-9]+ '.' [0-9]+ ;

LITERAL_INTEGER : FRAGMENT_INTEGER ;
LITERAL_FLOAT   : FRAGMENT_FLOAT ;
LITERAL_STRING  : '"' (~('"' | '\\' | '\r' | '\n') | ESCAPE_SEQUENCE )* '"' ;
LITERAL_CHAR    : '\'' (~('\'' | '\\' | '\r' | '\n') | ESCAPE_SEQUENCE) '\'' ;
LITERAL_SYMBOL  : '`' FRAGMENT_ID ;
LITERAL_FUNCTION: '@' FRAGMENT_ID ('.' FRAGMENT_ID)* ;

COMPOUND_ID : FRAGMENT_ID ('.' FRAGMENT_ID)+ ;
ID          : FRAGMENT_ID ;

WHITE_SPACE : [ \t\n\r]+      -> channel(HIDDEN) ;
COMMENT     : '//' (~[\n\r])* -> channel(HIDDEN) ;
