grammar MIR ;

program : programContent* ;
programContent : (module | content) ;
module  : 'module' name '{' content* '}' ;
content : method
        | globalVar
        | importModule
        ;
method  : 'def' name  '(' parameterList ')' ':' type '{' statement* '}' ;
parameterList :
              | name ':' type (',' name ':' type)* ;

globalVar : 'def' name ':' type ';' ;
importModule : 'import' IMPORT_COMPOUND_ID ';' 
             | 'import' COMPOUND_ID ';'
             ;

label : '[' name ']'
      | '[' COMPOUND_ID ']'
      ;
statement : (label)? statementCore ';' ;
statementCore : name ':' type '=' expression
              | compoundName ':' type '=' expression
              | 'nop'
              | 'return' name
              | 'goto' label (name)?
              ;

name : (ID | 'i' | 'j' | 'm' | 'd' | 'z' | 'u' | 'v' | 't');
compoundName : COMPOUND_ID ;
operand : compoundName
        | name
        | literal
        ;

methodCall : compoundName '(' argumentList ')'
            | name '(' argumentList ')'
            | literalFunction '(' argumentList ')'
            ;
argumentList :
             | operand (',' operand)* ;

expression : (methodCall | operand)
           | '(' type ')' (methodCall | operand)
           | 'check_type' '(' (methodCall | operand) ',' type ')'
           | 'check_cast' '(' (methodCall | operand) ',' type ')'
           | 'phi' '(' label name (',' label name)* ')'
           ;

literal : literalBool
        | literalChar
        | literalInteger
        | literalFloat
        | literalComplex
        | literalSymbol
        | literalTime
        | literalFunction
        | literalTable
        | literalKtable
        | literalString
        | literalList
        | literalDict
        | literalNil
        ;

literalList : '[' literalListInternal ']' (':' listType=typeList)? ;
literalListInternal :
                    | literal ((',' | ';')? literal)* ;

literalDict : '{' literalDictInternal '}' (':' dictType=typeDict)? ;
literalDictInternal :
                    | literalDictPair ((',' | ';')? literalDictPair)* ;
literalDictPair : key=literal '->' value=literal ;

literalNil     : 'nil' ':' type ;
literalComplex : opReal=('+' | '-')? real=(LITERAL_FLOAT | LITERAL_INTEGER)
                 ':' 'complex'
               | opIm  =('+' | '-')? im  =(LITERAL_FLOAT | LITERAL_INTEGER)
                 ('i' | 'j') ':' 'complex'
               | opReal=('+' | '-')? real=(LITERAL_FLOAT | LITERAL_INTEGER)
                 opIm  =('+' | '-')  im  =(LITERAL_FLOAT | LITERAL_INTEGER)?
                 ('i' | 'j') ':' 'complex'
               | opIM  =('+' | '-')? ('i' | 'j') ':' 'complex'
               ;
literalBool    : op=('+' | '-')? value=LITERAL_INTEGER ':' 'bool' ;
literalChar    : value=LITERAL_CHAR (':' 'char')? ;
literalInteger : op=('+' | '-')? value=LITERAL_INTEGER ':'
                 valueType=('i8'  | 'i16' | 'i32' | 'i64') ;
literalFloat   : op=('+' | '-')? value=(LITERAL_FLOAT | LITERAL_INTEGER) ':'
                 valueType=('f32' | 'f64') ;
literalSymbol  : value=LITERAL_SYMBOL (':' 'sym')? ;
literalTime    : value=LITERAL_FLOAT  ':' 'm'
               | value=LITERAL_T_GROUP_3  ':' 'd'
               | value=LITERAL_T_GROUP_7 (':' 'z')?
               | value=LITERAL_FLOAT  ':' 'u'
               | value=LITERAL_T_GROUP_3  ':' 'v'
               | value=LITERAL_T_GROUP_4 (':' 't')?
               ;
literalFunction: value=LITERAL_FUNCTION (':' typeFunc)? ;
literalTable   : value=ID ':' 'table' ;
literalKtable  : value=ID ':' 'ktable' ;
literalString  : value=LITERAL_STRING (':' 'str')? ;

type : tokenValue=( 'bool'    |
                    'char'    |
                    'i8'      | 'i16'     | 'i32'     | 'i64'     |
                    'f32'     | 'f64'     |
                    'complex' |
                    'sym'     |
                    'm' | 'd' | 'z' | 'u' | 'v' | 't' |
                    'str'     |
                    'table'   | 'ktbale'                          )
                                                               #typeCaseScalar
     | tokenValue='?'                                          #typeCaseWildcard
     | typeList                                                #typeCaseList
     | typeDict                                                #typeCaseDict
     | typeEnum                                                #typeCaseEnum
     | typeFunc                                                #typeCaseFunc
     ;

typeList : 'list' '<' element=type '>' ;
typeDict : 'dict' '<' key=type ',' value=type '>'
         ;
typeEnum : 'enum' '<' element=type '>' ;
typeFunc : 'func' '<' ':' type '>'                                #typeFuncCase0
         | 'func' '<' '...' ':' type '>'                          #typeFuncCase1
         | 'func' '<' type (',' type)* ':' type '>'               #typeFuncCase2
         | 'func' '<' type (',' type)* ',' '...' ':' type '>'     #typeFuncCase3
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
fragment FRAGMENT_FLOAT    : FRAGMENT_INTEGER '.' [0-9]+ ;

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

IMPORT_COMPOUND_ID : FRAGMENT_ID ('.' FRAGMENT_ID)* '.' '*' ;
COMPOUND_ID : FRAGMENT_ID ('.' FRAGMENT_ID)+ ;
ID          : FRAGMENT_ID ;

WHITE_SPACE : [ \t\n\r]+      -> channel(HIDDEN) ;
COMMENT     : '//' (~[\n\r])* -> channel(HIDDEN) ;
BLOCK_COMMET: '/*' .*? '*/'   -> channel(HIDDEN) ;
