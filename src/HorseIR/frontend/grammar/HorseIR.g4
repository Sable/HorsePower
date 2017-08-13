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

boolValueN       : value=LITERAL_INTEGER | value=NULL_TOKEN ;
literalBool      : boolValueN ':' 'bool'                               #literalBoolCase0
                 | '(' (boolValueN (',' boolValueN)*)? ')' ':' 'bool'  #literalBoolCase1
                 ;
charValue        : value=LITERAL_CHAR ;
literalChar      : charValue (':' 'char')?                             #literalCharCase0
                 | LITERAL_CHAR_VECTOR (':' 'char')?                   #literalCharCase1
                 | '(' ')' ':' 'char'                                  #literalCharCase2
                 | '(' (NULL_TOKEN (',' NULL_TOKEN)* ',')? charValue (',' (charValue | NULL_TOKEN))* ')' (':' 'char')?
                                                                       #literalCharCase3
                 | '(' NULL_TOKEN (',' NULL_TOKEN)* ')' ':' 'char'     #literalCharCase4
                 | NULL_TOKEN ':' 'char'                               #literalCharCase5
                 ;
stringValue      : LITERAL_STRING ;
literalString    : stringValue (':' 'str')?                            #literalStringCase0
                 | '(' ')' ':' 'str'                                   #literalStringCase1
                 | '(' (NULL_TOKEN (',' NULL_TOKEN)* ',')? stringValue (',' (stringValue | NULL_TOKEN))* ')' (':' 'str')?
                                                                       #literalStringCase2
                 | '(' NULL_TOKEN (',' NULL_TOKEN)* ')' ':' 'str'      #literalStringCase3
                 | NULL_TOKEN ':' 'str'                                #literalStringCase4
                 ;
intValueN        : op=('+' | '-')? LITERAL_INTEGER | NULL_TOKEN ;
literalInteger   : intValueN ':' typeToken=('i8' | 'i16' | 'i32' | 'i64')                             #literalIntegerCase0
                 | '(' (intValueN (',' intValueN)*)? ')' ':' typeToken=('i8' | 'i16' | 'i32' | 'i64') #literalIntegerCase1
                 ;
floatValueN      : op=('+' | '-')? value=(LITERAL_FLOAT | LITERAL_INTEGER)
                 | NULL_TOKEN ;
literalFloat     : floatValueN ':' typeToken=('f32' | 'f64')                               #literalFloatCase0
                 | '(' (floatValueN (',' floatValueN)*)? ')' ':' typeToken=('f32' | 'f64') #literalFloatCase1
                 ;
complexValueN    : realOp=('+' | '-')? real=(LITERAL_FLOAT | LITERAL_INTEGER)         #complexValueNCase0
                 | imOp=('+' | '-')? im=(LITERAL_FLOAT | LITERAL_INTEGER) ('i' | 'j') #complexValueNCase1
                 | realOp=('+' | '-')? real=(LITERAL_FLOAT | LITERAL_INTEGER) imOp=('+' | '-') im=(LITERAL_FLOAT | LITERAL_INTEGER)? ('i' | 'j')
                                                                                      #complexValueNCase2
                 | imOp=('+' | '-')? ('i' | 'j')                                      #complexValueNCase3
                 | NULL_TOKEN                                                         #complexValueNCase4
                 ;
literalComplex   : complexValueN ':' 'complex'                                  #literalComplexCase0
                 | '(' (complexValueN (',' complexValueN)*)? ')' ':' 'complex'  #literalComplexCase1
                 ;
symbolValue      : LITERAL_SYMBOL ;
literalSymbol    : symbolValue (':' 'sym')?
                 | '(' ')' ':' 'sym'
                 | '(' (NULL_TOKEN (',' NULL_TOKEN)* ',')? symbolValue (',' (symbolValue | NULL_TOKEN))* ')' (':' 'sym')?
                 | '(' NULL_TOKEN (',' NULL_TOKEN)* ')' ':' 'sym'
                 | NULL_TOKEN ':' 'sym'
                 ;
tMonthValueN     : LITERAL_FLOAT | NULL_TOKEN ;
literalTMonth    : tMonthValueN ':' 'm'
                 | '(' (tMonthValueN (',' tMonthValueN)*)? ')' ':' 'm'
                 ;
tDateValue       : LITERAL_GROUP_3_DATE ;
tDateValueN      : LITERAL_GROUP_3_DATE | NULL_TOKEN ;
literalTDate     : tDateValue (':' 'd')?
                 | '(' tDateValue (',' tDateValue)* ')' (':' 'd')?
                 | '(' ')' ':' 'd'
                 | '(' NULL_TOKEN (',' tDateValueN)* (',' tDateValue) (',' tDateValueN)* ')' (':' 'd')?
                 | '(' tDateValue (',' tDateValueN)* (',' NULL_TOKEN) (',' tDateValueN)* ')' (':' 'd')?
                 | '(' NULL_TOKEN (',' NULL_TOKEN)* ')' ':' 'd'
                 | NULL_TOKEN ':' 'd'
                 ;
tDateTimeValue   : LITERAL_GROUP_7_DATETIME ;
tDateTimeValueN  : LITERAL_GROUP_7_DATETIME | NULL_TOKEN ;
literalTDateTime : tDateTimeValue (':' 'z')?
                 | '(' tDateTimeValue (',' tDateTimeValue)* ')' (':' 'z')?
                 | '(' ')' ':' 'z'
                 | '(' NULL_TOKEN (',' tDateTimeValueN)* (',' tDateTimeValue) (',' tDateTimeValueN)* ')' (':' 'z')?
                 | '(' tDateTimeValue (',' tDateTimeValueN)* (',' NULL_TOKEN) (',' tDateTimeValueN)* ')' (':' 'z')?
                 | '(' NULL_TOKEN (',' NULL_TOKEN)* ')' ':' 'z'
                 | NULL_TOKEN ':' 'z'
                 ;
tMinuteValue     : LITERAL_GROUP_2_MINUTE ;
tMinuteValueN    : LITERAL_GROUP_2_MINUTE | NULL_TOKEN ;
literalTMinute   : tMinuteValue (':' 'u')?
                 | '(' tMinuteValue (',' tMinuteValue)* ')' (':' 'u')?
                 | '(' ')' ':' 'u'
                 | '(' NULL_TOKEN (',' tMinuteValueN)* (',' tMinuteValue) (',' tMinuteValueN)* ')' (':' 'u')?
                 | '(' tMinuteValue (',' tMinuteValueN)* (',' NULL_TOKEN) (',' tMinuteValueN)* ')' (':' 'u')?
                 | '(' NULL_TOKEN (',' NULL_TOKEN)* ')' ':' 'u'
                 | NULL_TOKEN ':' 'u'
                 ;
tSecondValue     : LITERAL_GROUP_3_SECOND ;
tSecondValueN    : LITERAL_GROUP_3_SECOND | NULL_TOKEN ;
literalTSecond   : tSecondValue (':' 'v')?
                 | '(' tSecondValue (',' tSecondValue)* ')' (':' 'v')?
                 | '(' ')' ':' 'v'
                 | '(' NULL_TOKEN (',' tSecondValueN)* (',' tSecondValue) (',' tSecondValueN)* ')' (':' 'v')?
                 | '(' tSecondValue (',' tSecondValueN)* (',' NULL_TOKEN) (',' tSecondValueN)* ')' (':' 'v')?
                 | '(' NULL_TOKEN (',' NULL_TOKEN)* ')' ':' 'v'
                 | NULL_TOKEN ':' 'v'
                 ;
tTimeValue       : LITERAL_GROUP_4_TIME ;
tTimeValueN      : LITERAL_GROUP_4_TIME | NULL_TOKEN ;
literalTTime     : tTimeValue (':' 't')?
                 | '(' tTimeValue (',' tTimeValue)* ')' (':' 't')?
                 | '(' ')' ':' 't'
                 | '(' NULL_TOKEN (',' tTimeValueN)* (',' tTimeValue) (',' tTimeValueN)* ')' (':' 't')?
                 | '(' tTimeValue (',' tTimeValueN)* (',' NULL_TOKEN) (',' tTimeValueN)* ')' (':' 't')?
                 | '(' NULL_TOKEN (',' NULL_TOKEN)* ')' ':' 'z'
                 | NULL_TOKEN ':' 'z'
                 ;
functionValueN   : LITERAL_FUNCTION | NULL_TOKEN ;
literalFunction  : functionValueN ':' typeFunc
                 | '(' (functionValueN (',' functionValueN)*)? ')' ':' typeFunc
                 ;
literalList      : '[' ']' ':' typeList
                 | '[' literal (',' literal)* ']' (typeList)?
                 | NULL_TOKEN ':' typeList
                 ;
literalDict      : '{' '}' ':' typeDict
                 | '{' (literal '->' literal) (',' literal '->' literal)* '}' (':' typeDict)?
                 | NULL_TOKEN ':' typeDict
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
                 | NULL_TOKEN ':' 'table'
                 ;
literalKTable    : '{' tableKeyedColumn (',' tableKeyedColumn)* (',' tableColumn)* '}' ':' 'ktable'
                 | NULL_TOKEN ':' 'ktable' ;
literalEnum      : '<' tableColumn '>' (':' typeEnum)?
                 | NULL_TOKEN ':' typeEnum
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
                          'sym'     | 'str' |
                          'm' | 'd' | 'z'   | 'u'   | 'v'   | 't' |
                          'table'   | 'ktable'              ) #typeCasePrimitive

                | '?'                                         #typeCaseWildcard
                | typeList                                    #typeCaseList
                | typeDict                                    #typeCaseDict
                | typeEnum                                    #typeCaseEnum
                | typeFunc                                    #typeCaseFunc
                ;

typeList        : 'list' '<' element=type '>' ;

typeDict        : 'dict' '<' key=type ',' value=type '>' ;

typeEnum        : 'enum' '<' element=type '>' ;

typeFunc        : 'func' '<' ':' type '>'                            #typeFuncCase0
                | 'func' '<' '...' ':' type '>'                      #typeFuncCase1
                | 'func' '<' type (',' type)* ':' type '>'           #typeFuncCase2
                | 'func' '<' type (',' type)* ',' '...' ':' type '>' #typeFuncCase3
                ;

NULL_TOKEN            : 'null' ;
LITERAL_GROUP_2_MINUTE : [0-9]+ ':' [0-9]+ ;
LITERAL_GROUP_3_DATE   : [0-9]+ '.' [0-9]+ '.' [0-9]+ ;
LITERAL_GROUP_3_SECOND : [0-9]+ ':' [0-9]+ ':' [0-9]+ ;

LITERAL_GROUP_4_TIME :   [0-9]+ ':' [0-9]+ ':' [0-9]+ '.' [0-9]+ ;

LITERAL_GROUP_7_DATETIME : [0-9]+ '.' [0-9]+ '.' [0-9]+ 'T'
                           [0-9]+ ':' [0-9]+ ':' [0-9]+ '.' [0-9]+ ;

LITERAL_INTEGER : FRAGMENT_INTEGER ;

LITERAL_FLOAT   : FRAGMENT_FLOAT (('e' | 'E') ('+' | '-')? ('0')* FRAGMENT_INTEGER)?
                | FRAGMENT_INTEGER (('e' | 'E') ('+' | '-')? ('0')* FRAGMENT_INTEGER)
                | ('inf' | 'INF')
                | ('nan' | 'NAN')
                ;

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

