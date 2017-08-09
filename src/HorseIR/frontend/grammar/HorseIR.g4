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
                 | name ':' type '=' name ';'
                 | name ':' type '=' '(' type ')' name ';'
                 | name ':' type '=' 'check_type' '(' name ',' type ')' ';'
                 | name ':' type '=' 'check_cast' '(' name ',' type ')' ';'
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
                 | literalTable
                 | literalKTable
                 | literalNull
                 ;
boolValue        : ('+' | '-')? LITERAL_INTEGER ;
boolValueN       : ('+' | '-')? LITERAL_INTEGER | 'null' ;
literalBool      : boolValue ':' 'bool'
                 | '(' ')' ':' 'bool'
                 | '(' boolValue (',' boolValue)* ')' ':' 'bool'
                 | '(' 'null' (',' boolValueN)* (',' boolValue) (',' boolValueN)* ')' ':' 'bool'
                 | '(' boolValue (',' boolValueN)* (',' 'null') (',' boolValueN)* ')' ':' 'bool'
                 ;

charValue        : LITERAL_CHAR ;
charValueN       : LITERAL_CHAR | 'null' ;
literalChar      : charValue (':' 'char')?
                 | '(' ')' ':' 'char'
                 | '(' charValue (',' charValue)* ')' (':' 'char')?
                 | '(' 'null' (',' charValueN)* (',' charValue) (',' charValueN)* ')' (':' 'char')?
                 | '(' charValue (',' charValueN)* (',' 'null') (',' charValueN)* ')' (':' 'char')?
                 | LITERAL_CHAR_VECTOR
                 ;

intValue         : ('+' | '-')? LITERAL_INTEGER ;
intValueN        : ('+' | '-')? LITERAL_INTEGER | 'null' ;
literalInteger   : intValue ':' ('i8' | 'i16' | 'i32' | 'i64')
                 | '(' intValue (',' intValue)* ')' ':' ('i8' | 'i16' | 'i32' | 'i64')
                 | '(' ')' ':' ('i8' | 'i16' | 'i32' | 'i64')
                 | '(' 'null' (',' intValueN)* (',' intValue) (',' intValueN)* ')' ':' ('i8' | 'i16' | 'i32' | 'i64')
                 | '(' intValue (',' intValueN)* (',' 'null') (',' intValueN)* ')' ':' ('i8' | 'i16' | 'i32' | 'i64')
                 ;

floatValue       : ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) ;
floatValueN      : ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) | 'null' ;
literalFloat     : floatValue ':' ('f32' | 'f64')
                 | '(' ')' ':' ('f32' | 'f64')
                 | '(' floatValue (',' floatValue)* ')' ':' ('f32' | 'f64')
                 | '(' 'null' (',' floatValueN)* (',' floatValue) (',' floatValueN)* ')' ('f32' | 'f64')
                 | '(' floatValue (',' floatValueN)* (',' 'null') (',' floatValueN)* ')' ('f32' | 'f64')
                 ;

complexValue     : ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER)
                 | ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) ('i' | 'j')
                 | ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) ('+' | '-') (LITERAL_FLOAT | LITERAL_INTEGER)? ('i' | 'j')
                 | ('+' | '-')? ('i' | 'j')
                 ;
complexValueN    : ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER)
                 | ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) ('i' | 'j')
                 | ('+' | '-')? (LITERAL_FLOAT | LITERAL_INTEGER) ('+' | '-') (LITERAL_FLOAT | LITERAL_INTEGER)? ('i' | 'j')
                 | ('+' | '-')? ('i' | 'j')
                 | 'null'
                 ;
literalComplex   : complexValue ':' 'complex'
                 | '(' ')' ':' 'complex'
                 | '(' complexValue (',' complexValue)* ')' ':' 'complex'
                 | '(' 'null' (',' complexValueN)* (',' complexValue) (',' complexValueN)* ')' ':' 'complex'
                 | '(' complexValue (',' complexValueN)* (',' 'null') (',' complexValueN)* ')' ':' 'complex'
                 ;

symbolValue      : LITERAL_SYMBOL ;
symbolValueN     : LITERAL_SYMBOL | 'null' ;
literalSymbol    : symbolValue (':' 'sym')?
                 | '(' ')' ':' 'sym'
                 | '(' symbolValue (',' symbolValue)* ')' (':' 'sym')?
                 | '(' 'null' (',' symbolValueN)* (',' symbolValue) (',' symbolValueN)* ')' (':' 'sym')?
                 | '(' symbolValue (',' symbolValueN)* (',' 'null') (',' symbolValueN)* ')' (':' 'sym')?
                 ;

tMonthValue      : LITERAL_FLOAT ;
tMonthValueN     : LITERAL_FLOAT | 'null' ;
literalTMonth    : tMonthValue ':' 'm'
                 | '(' ')' ':' 'm'
                 | '(' tMonthValue (',' tMonthValue)* ')' ':' 'm'
                 | '(' 'null' (',' tMonthValueN)* (',' tMonthValue) (',' tMonthValueN)* ')' ':' 'm'
                 | '(' tMonthValue (',' tMonthValueN)* (',' 'null') (',' tMonthValueN)* ')' ':' 'm'
                 ;

tDateValue       : LITERAL_GROUP_3;
tDateValueN      : LITERAL_GROUP_3 | 'null' ;
literalTDate     : tDateValue ':' 'd'
                 | '(' ')' ':' 'd'
                 | '(' tDateValue (',' tDateValue)* ')' ':' 'd'
                 | '(' 'null' (',' tDateValueN)* (',' tDateValue) (',' tDateValueN)* ')' ':' 'd'
                 | '(' tDateValue (',' tDateValueN)* (',' 'null') (',' tDateValueN)* ')' ':' 'd'
                 ;

tDateTimeValue   : LITERAL_GROUP_7 ;
tDateTimeValueN  : LITERAL_GROUP_7 | 'null' ;
literalTDateTime : tDateTimeValue ':' (':' 'z')?
                 | '(' ')' ':' 'z'
                 | '(' tDateTimeValue (',' tDateTimeValue)* ')' ':' (':' 'z')?
                 | '(' 'null' (',' tDateTimeValueN)* (',' tDateTimeValue) (',' tDateTimeValueN)* ')' ':' (':' 'z')?
                 | '(' tDateTimeValue (',' tDateTimeValueN)* (',' 'null') (',' tDateTimeValueN)* ')' ':' (':' 'z')?
                 ;

tMinuteValue     : LITERAL_FLOAT ;
tMinuteValueN    : LITERAL_FLOAT | 'null' ;
literalTMinute   :  tMinuteValue ':' 'u'
                 | '(' ')' ':' 'u'
                 | '(' tMinuteValue (',' tMinuteValue)* ')' ':' 'u'
                 | '(' 'null' (',' tMinuteValueN)* (',' tMinuteValue) (',' tMinuteValueN)* ')' ':' 'u'
                 | '(' tMinuteValue (',' tMinuteValueN)* (',' 'null') (',' tMinuteValueN)* ')' ':' 'u'
                 ;


tSecondValue     : LITERAL_GROUP_3 ;
tSecondValueN    : LITERAL_GROUP_3 | 'null' ;
literalTSecond   : tSecondValue ':' 'v'
                 | '(' ')' ':' 'v'
                 | '(' tSecondValue (',' tSecondValue)* ')' ':' 'v'
                 | '(' 'null' (',' tSecondValueN)* (',' tSecondValue) (',' tSecondValueN)* ')' ':' 'v'
                 | '(' tSecondValue (',' tSecondValueN)* (',' 'null') (',' tSecondValueN)* ')' ':' 'v'
                 ;

tTimeValue       : LITERAL_GROUP_4 ;
tTimeValueN      : LITERAL_GROUP_4 | 'null' ;
literalTTime     : tTimeValue ':' (':' 't')?
                 | '(' ')' ':' 't'
                 | '(' tTimeValue (',' tTimeValue)* ')' ':' (':' 't')?
                 | '(' 'null' (',' tTimeValueN)* (',' tTimeValue) (',' tTimeValueN)* ')' ':' (':' 't')?
                 | '(' tTimeValue (',' tTimeValueN)* (',' 'null') (',' tTimeValueN)* ')' ':' (':' 't')?
                 ;

functionValue    : LITERAL_FUNCTION ;
functionValueN   : LITERAL_FUNCTION | 'null' ;
literalFunction  : functionValue ':' (':' typeFunc)?
                 | '(' ')' ':' typeFunc
                 | '(' functionValue (',' functionValue)* ')' ':' (':' typeFunc)?
                 | '(' 'null' (',' functionValueN)* (',' functionValue) (',' functionValueN)* ')' ':' (':' typeFunc)?
                 | '(' functionValue (',' functionValueN)* (',' 'null') (',' functionValueN)* ')' ':' (':' typeFunc)?
                 ;

literalList      : '[' ']' ':' typeList
                 | '[' literal (',' literal)* ']' (typeList)?
                 ;

literalDict      : '{' '}' ':' typeDict
                 | '{' (literal '->' literal) (',' literal '->' literal)* '}' (':' typeDict)? ;

tableHeader      : name | LITERAL_SYMBOL ;
tableContent     : literalBool      | literalChar      | literalInteger
                 | literalFloat     | literalComplex   | literalSymbol
                 | literalTTime     | literalTSecond   | literalTMonth
                 | literalTMinute   | literalTDateTime | literalTDate
                 | '[' ']' (':' 'list' '<' 'char' '>')?
                 | '[' literalChar (',' literalChar)* ']' (':' 'list' '<' 'char' '>')?
                 ;
tableColumn      : (tableHeader) '->' tableContent ;
tableKeyedColumn : '[' (tableHeader) '->' tableContent ']' ;
literalTable     : '{' '}' ':' 'table'
                 | '{' tableColumn (',' tableColumn)* '}' ':' 'table'
                 ;
literalKTable    : '{' tableKeyedColumn (',' tableKeyedColumn)* (',' tableColumn)* '}' ':' 'ktable' ;

literalNull      : 'null' ':' type
                 | '(' 'null' (',' 'null')* ')' ':' type
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

