grammar HorseIR;
program          : (moudle | moduleContent)* ;
moudle           : 'module' name '{' moduleContent* '}' ;
moduleContent    : method
                 | globalVar
                 | importModule
                 ;

method           : 'def' name '(' ( | (name ':' type (',' name ':' type)*)) ')' ':' type '{' statement* '}' ;
globalVar        : 'def' name ':' type ';' ;
importModule     : 'import' COMPOUND_ID ';' ;

statement        : '[' name ']'
                 | name ':' type '=' operand ';'
                 | name ':' type '=' '(' type ')' operand ';'
                 | name ':' type '=' 'check_type' '(' operand ',' type ')' ';'
                 | name ':' type '=' 'check_cast' '(' operand ',' type ')' ';'
                 | name ':' type '=' LITERAL_FUNCTION '(' ( | operand (',' operand)*) ')' ';'
                 | name ':' type '=' '(' type ')' LITERAL_FUNCTION '(' ( | operand (',' operand)*) ')' ';'
                 | name ':' type '=' 'check_type' '(' LITERAL_FUNCTION '(' ( | operand (',' operand)*) ')' ',' type ')' ';'
                 | name ':' type '=' 'check_cast' '(' LITERAL_FUNCTION '(' ( | operand (',' operand)*) ')' ',' type ')' ';'
                 | name ':' type '=' 'phi' '(' '[' name ']' name (',' '[' name ']' name)* ')' ';'
                 | 'return' name ';'
                 | 'goto' '[' name ']' (name)? ';'
                 ;

operand          : name
                 | literal
                 ;

name             : id=(ID | 'i' | 'j' | 'm' | 'd' | 'z' | 'u' | 'v' | 't') ;

literal          : literalBool
                 | literalChar
                 | literalString
                 | literalInteger
                 | literalFloat
                 | literalComplex
                 | literalSymbol
                 | literalTDate
                 | literalTDateTime
                 | literalTMinute
                 | literalTMonth
                 | literalTSecond
                 | literalTTime
                 | literalFunction
                 | literalList
                 | literalDict
                 | literalEnum
                 | literalTable
                 | literalKTable
                 ;

boolValueN       : ('+' | '-')? LITERAL_INTEGER | NULL ;
literalBool      : boolValueN ':' 'bool'
                 | '(' (boolValueN (',' boolValueN)*)? ')' ':' 'bool'
                 ;
charValue        : LITERAL_CHAR ;
charValueN       : LITERAL_CHAR | NULL ;
literalChar      : charValue (':' 'char')?
                 | LITERAL_CHAR_VECTOR (':' 'char')?
                 | '(' charValue (',' charValue)* ')' (':' 'char')?
                 | '(' ')' ':' 'char'
                 | '(' NULL (',' charValueN)* (',' charValue) (',' charValueN)* ')' (':' 'char')?
                 | '(' charValue (',' charValueN)* (',' NULL) (',' charValueN)* ')' (':' 'char')?
                 | '(' NULL (',' NULL)* ')' ':' 'char'
                 | NULL ':' 'char'
                 ;
stringValue      : LITERAL_STRING ;
stringValueN     : LITERAL_STRING | NULL ;
literalString    : stringValue (':' 'str')?
                 | '(' stringValue (',' stringValue)* ')' (':' 'str')?
                 | '(' ')' ':' 'str'
                 | '(' NULL (',' stringValueN)* (',' stringValue) (',' stringValueN)* ')' (':' 'str')?
                 | '(' stringValue (',' stringValueN)* (',' NULL) (',' stringValueN)* ')' (':' 'str')?
                 | '(' NULL (',' NULL)* ')' ':' 'str'
                 | NULL ':' 'str'
                 ;
intValueN        : ('+' | '-')? LITERAL_INTEGER | NULL ;
literalInteger   : intValueN ':' ('i8' | 'i16' | 'i32' | 'i64')
                 | '(' (intValueN (',' intValueN)*)? ')' ':' ('i8' | 'i16' | 'i32' | 'i64')
                 ;
floatValueN      : ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) | NULL ;
literalFloat     : floatValueN ':' ('f32' | 'f64')
                 | '(' (floatValueN (',' floatValueN)*)? ')' ':' ('f32' | 'f64')
                 ;
complexValueN    : ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER)
                 | ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) ('i' | 'j')
                 | ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) ('+' | '-') (LITERAL_FLOAT | LITERAL_INTEGER)? ('i' | 'j')
                 | ('+' | '-')? ('i' | 'j')
                 | NULL
                 ;
literalComplex   : complexValueN ':' 'complex'
                 | '(' (complexValueN (',' complexValueN)*)? ')' ':' 'complex'
                 ;
symbolValue      : LITERAL_SYMBOL ;
symbolValueN     : LITERAL_SYMBOL | NULL ;
literalSymbol    : symbolValue (':' 'sym')?
                 | '(' symbolValue (',' symbolValue)* ')' (':' 'sym')?
                 | '(' ')' ':' 'sym'
                 | '(' NULL (',' symbolValueN)* (',' symbolValue) (',' symbolValueN)* ')' (':' 'sym')?
                 | '(' symbolValue (',' symbolValueN)* (',' NULL) (',' symbolValueN)* ')' (':' 'sym')?
                 | '(' NULL (',' NULL)* ')' ':' 'sym'
                 | NULL ':' 'sym'
                 ;
tMonthValueN     : LITERAL_FLOAT | NULL ;
literalTMonth    : tMonthValueN ':' 'm'
                 | '(' (tMonthValueN (',' tMonthValueN)*)? ')' ':' 'm'
                 ;
tDateValueN      : LITERAL_GROUP_3 | NULL ;
literalTDate     : tDateValueN ':' 'd'
                 | '(' (tDateValueN (',' tDateValueN)*)? ')' ':' 'd'
                 ;
tDateTimeValue   : LITERAL_GROUP_7 ;
tDateTimeValueN  : LITERAL_GROUP_7 | NULL ;
literalTDateTime : tDateTimeValue (':' 'z')?
                 | '(' tDateTimeValue (',' tDateTimeValue)* ')' (':' 'z')?
                 | '(' ')' ':' 'z'
                 | '(' NULL (',' tDateTimeValueN)* (',' tDateTimeValue) (',' tDateTimeValueN)* ')' (':' 'z')?
                 | '(' tDateTimeValue (',' tDateTimeValueN)* (',' NULL) (',' tDateTimeValueN)* ')' (':' 'z')?
                 | '(' NULL (',' NULL)* ')' ':' 'z'
                 | NULL ':' 'z'
                 ;
tMinuteValueN    : LITERAL_FLOAT | NULL ;
literalTMinute   : tMinuteValueN ':' 'u'
                 | '(' (tMinuteValueN (',' tMinuteValueN)*)? ')' ':' 'u'
                 ;
tSecondValueN    : LITERAL_GROUP_3 | NULL ;
literalTSecond   : tSecondValueN ':' 'v'
                 | '(' (tSecondValueN (',' tSecondValueN)*)? ')' ':' 'v'
                 ;
tTimeValue       : LITERAL_GROUP_4 ;
tTimeValueN      : LITERAL_GROUP_4 | NULL ;
literalTTime     : tTimeValue (':' 't')?
                 | '(' tTimeValue (',' tTimeValue)* ')' (':' 't')?
                 | '(' ')' ':' 't'
                 | '(' NULL (',' tTimeValueN)* (',' tTimeValue) (',' tTimeValueN)* ')' (':' 't')?
                 | '(' tTimeValue (',' tTimeValueN)* (',' NULL) (',' tTimeValueN)* ')' (':' 't')?
                 | '(' NULL (',' NULL)* ')' ':' 'z'
                 | NULL ':' 'z'
                 ;
functionValue    : LITERAL_FUNCTION ;
functionValueN   : LITERAL_FUNCTION | NULL ;
literalFunction  : functionValue (':' typeFunc)?
                 | '(' functionValue (',' functionValue) ')' (':' typeFunc)?
                 | '(' ')' ':' typeFunc
                 | '(' NULL (',' functionValueN)* (',' functionValue) (',' functionValueN)* ')' (':' typeFunc)?
                 | '(' functionValue (',' functionValueN)* (',' NULL) (',' functionValueN)* ')' (':' typeFunc)?
                 | '(' NULL (',' NULL)* ')' ':' typeFunc
                 | NULL ':' typeFunc
                 ;
literalList      : '[' ']' ':' typeList
                 | '[' literal (',' literal)* ']' (typeList)?
                 | NULL ':' typeList
                 ;
literalDict      : '{' '}' ':' typeDict
                 | '{' (literal '->' literal) (',' literal '->' literal)* '}' (':' typeDict)?
                 | NULL ':' typeDict
                 ;
tableHeader      : name | LITERAL_SYMBOL ;
dbContent        : literalBool    | literalChar    | literalInteger | literalFloat
                 | literalComplex | literalSymbol  | literalTTime   | literalTSecond
                 | literalTMonth  | literalTMinute | literalTDate   | literalTDateTime
                 | literalString
                 ;

tableColumn      : (tableHeader) '->' dbContent ;
tableKeyedColumn : '[' (tableHeader) '->' dbContent ']' ;
literalTable     : '{' '}' ':' 'table'
                 | '{' tableColumn (',' tableColumn)* '}' ':' 'table'
                 | NULL ':' 'table'
                 ;
literalKTable    : '{' tableKeyedColumn (',' tableKeyedColumn)* (',' tableColumn)* '}' ':' 'ktable'
                 | NULL ':' 'ktable' ;
literalEnum      : '<' tableColumn '>' (':' typeEnum)?
                 | NULL ':' typeEnum
                 ;


/*
  ScalarMonth = 'm'  ScalarDate = 'd'  ScalarDateTime = 'z'  ScalarMinute = 'u'
  ScalarSecond = 'v' ScalarTime = 't'
*/
type            : token=( 'bool'    |
                          'char'    |
                          'i8'      | 'i16' | 'i32' | 'i64' |
                          'f32'     | 'f64' |
                          'complex' |
                          'sym'     |
                          'm' | 'd' | 'z' | 'u' | 'v' | 't' |
                          'table'   | 'ktable'              )

                | '?'
                | typeList
                | typeDict
                | typeEnum
                | typeFunc
                ;

typeList        : 'list' '<' element=type '>' ;

typeDict        : 'dict' '<' key=type ',' value=type '>' ;

typeEnum        : 'enum' '<' element=type '>' ;

typeFunc        : 'func' '<' ':' type '>'
                | 'func' '<' '...' ':' type '>'
                | 'func' '<' type (',' type)* ':' type '>'
                | 'func' '<' type (',' type)* ',' '...' ':' type '>'
                ;

NULL            : 'null' ;
LITERAL_GROUP_3 : [0-9]+ '.' [0-9]+ '.' [0-9]+ ;

LITERAL_GROUP_4 : [0-9]+ '.' [0-9]+ '.' [0-9]+ '.' [0-9]+ ;

LITERAL_GROUP_7 : [0-9]+ '.' [0-9]+ '.' [0-9]+ 'T'
                  [0-9]+ '.' [0-9]+ '.' [0-9]+ '.' [0-9]+ ;

LITERAL_INTEGER : FRAGMENT_INTEGER ;

LITERAL_FLOAT   : FRAGMENT_FLOAT ;

LITERAL_STRING  : '"' (~('"' | '\\' | '\r' | '\n') | ESCAPE_SEQUENCE )* '"' ;

LITERAL_CHAR        : '\'' (~('\'' | '\\' | '\r' | '\n') | ESCAPE_SEQUENCE) '\'' ;
LITERAL_CHAR_VECTOR : '\'' (~('\'' | '\\' | '\r' | '\n') | ESCAPE_SEQUENCE)* '\'' ;

LITERAL_SYMBOL  : '`' FRAGMENT_ID ;

LITERAL_FUNCTION: '@' FRAGMENT_ID ('.' FRAGMENT_ID)? ;


COMPOUND_ID     : FRAGMENT_ID '.' FRAGMENT_ID
                | FRAGMENT_ID '.' '*' ;

ID              : FRAGMENT_ID ;

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
                                   'x' HEX_GROUP_2                )
                           ;
fragment FRAGMENT_ID       : [a-zA-Z_][a-zA-Z0-9_]* ;
fragment FRAGMENT_INTEGER  : ('0' | [1-9][0-9]*) ;
fragment FRAGMENT_FLOAT    : FRAGMENT_INTEGER '.' [0-9]+ ;

WHITE_SPACE                : [ \t\n\r]+      -> channel(HIDDEN) ;
COMMENT                    : '//' (~[\n\r])* -> channel(HIDDEN) ;
BLOCK_COMMET               : '/*' .*? '*/'   -> channel(HIDDEN) ;

