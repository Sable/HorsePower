%{
#include <stdio.h>
#include <stdlib.h>
#define P printf
#define D(x) printf("Debug:" x " %s" "\n",yytext)
%}

%code requires {
    #include "tree.h"
}

%union {
    int    intconst;
    char  *stringconst;
    double floatconst;
    struct Prog *program;
    struct List *list;
    struct Node *node;
}

%code {
    extern char *yytext;
    extern int yylineno;
    extern Prog *root;
    int yylex(); /* defined in lex.yy.c */

    void yyerror() {
        fprintf(stderr, "syntax error on line %d near %s\n", yylineno, yytext);
        exit(1);
    }
}

%token <stringconst> kMODULE kIMPORT kDEF kCKCAST kRETURN
%token <stringconst> kBOOL kCHAR kI16 kI32 kI64 kF32 kF64 kCLEX kSYM kSTR kMONTH kDATE kTZ kTU kTV kTT
%token <stringconst> tID tSTRING tCHAR
%token <intconst>    tINT tDATE
%token <floatconst>  tFLOAT

%type <prog> program 
%type <list> module_list module_body_list stmt_list param_list symbol_list int_list float_list dateValue_list string_list paramType_list paramType_list1
%type <node> module moduleContent moduleBody stmt simple_stmt return_stmt expression literalFunction param funcName name literal literalBool literalChar literalString literalInteger intType literalFloat floatType literalSymbol literalDate compoundID type intValue floatValue paramExpr symbol_single nameType method
%type <stringconst> symbol_name

%start program

%% 

program         : module_list
                  { root = makeProg($1); }
;

module_list     :
                  { $$ = NULL; }
                | module module_list
                  { $$ = makeList($1, $2); }
;

module          : kMODULE name '{' moduleContent '}'
                  { $$ = makeNodeModule($2, $4); }
                | method
                  { $$ = makeNodeModule(NULL, makeNodeModuleContent(makeList($1,NULL))); }
;

moduleContent   : module_body_list
                  { $$ = makeNodeModuleContent($1); }
;

module_body_list:
                  { $$ = NULL; }
                | moduleBody module_body_list
                  { $$ = makeList($1, $2); }
;

moduleBody      : method
                  { $$ = $1; }
                | kIMPORT compoundID ';'
                  { $$ = makeNodeImport($2); }
;

method         : kDEF name '(' paramType_list ')' ':' type '{' stmt_list '}'
                  { $$ = makeNodeModuleMethod($2, $4, $7, $9); }
;

paramType_list  :
                 { $$ = NULL; }
                | paramType_list1
                 { $$ = $1; }
;

paramType_list1 : nameType
                 { $$ = makeList($1, NULL); }
                | nameType ',' paramType_list1
                 { $$ = makeList($1, $3); }
;

nameType        : name ':' type
                 { $$ = makeNodeNameType($1, $3); }
;

stmt_list       :
                 { $$ = NULL; }
                | stmt stmt_list
                 { $$ = makeList($1, $2); }
;

stmt            : simple_stmt
                 { $$ = $1; }
                | return_stmt
                 { $$ = $1; }
;
simple_stmt     : name ':' type '=' expression ';'
                 { $$ = makeNodeSimpleStmt1($1, $3, $5); }
                | name ':' type '=' kCKCAST '(' expression ',' type ')' ';'
                 { $$ = makeNodeSimpleStmt2($1, $3, $7, $9); }
;
return_stmt     : kRETURN param ';'
                 { $$ = makeNodeReturnStmt($2); }
;
expression      : literalFunction '(' paramExpr ')'
                 { $$ = makeNodeExpr($1, $3); }
                | param
                 { $$ = makeNodeExpr(NULL, $1); }
;
literalFunction : '@' tID
                 { $$ = makeNodeFunction(makeNodeCompoundID1(NULL,$2)); }
                | '@' compoundID
                 { $$ = makeNodeFunction($2); }
;
paramExpr       :
                 { $$ = NULL; }
                | param_list
                 { $$ = makeNodeParamExpr($1); }
;
param_list      : param
                 { $$ = makeList($1, NULL); }
                | param ',' param_list
                 { $$ = makeList($1, $3); }
;
param           : name
                 { $$ = makeNodeParamLiteral($1); }
                | funcName
                 { $$ = makeNodeParamLiteral($1); }
                | literal
                 { $$ = makeNodeParamLiteral($1); }
;
funcName        : '@' name
                 { $$ = makeNodeKind($2, literalFuncK); }
;
name            : tID
                 { $$ = makeNodeID($1); }
;
literal         : literalBool
                 { $$ = $1; }
                | literalChar
                 { $$ = $1; }
                | literalString
                 { $$ = $1; }
                | literalInteger
                 { $$ = $1; }
                | literalFloat
                 { $$ = $1; }
                | literalSymbol
                 { $$ = $1; }
                | literalDate
                 { $$ = $1; }
;

literalBool     : int_list ':' kBOOL
                 { $$ = makeNodeLiteralBool($1); }
                | '(' int_list ')' ':' kBOOL
                 { $$ = makeNodeLiteralBool($2); }
;

literalChar     : tCHAR ':' kCHAR
                 { $$ = makeNodeLiteralChar($1); }
;

literalString   : string_list  ':' kSTR
                 { $$ = makeNodeLiteralString($1); }
                | '(' string_list ')' ':' kSTR
                 { $$ = makeNodeLiteralString($2); }
;

literalInteger  : int_list ':' intType
                 { $$ = makeNodeLiteralInt($1, $3); }
                | '(' int_list ')' ':' intType
                 { $$ = makeNodeLiteralInt($2, $5); }
;

intType         : kI16
                 { $$ = makeNodeIntType(i16T); }
                | kI32
                 { $$ = makeNodeIntType(i32T); }
                | kI64
                 { $$ = makeNodeIntType(i64T); }
;

/* float */
literalFloat    : float_list ':' floatType
                 { $$ = makeNodeLiteralFloat($1, $3); }
                | '(' float_list ')' ':' floatType
                 { $$ = makeNodeLiteralFloat($2, $5); }
                | int_list ':' floatType
                 { $$ = makeNodeLiteralFloat($1, $3); }
                | '(' int_list ')' ':' floatType
                 { $$ = makeNodeLiteralFloat($2, $5); }
;
floatType       : kF32
                 { $$ = makeNodeFloatType(f32T); }
                | kF64
                 { $$ = makeNodeFloatType(f64T); }
;

/* symbol */
/*
literalSymbol   : symbol_list ':' kSYM
                 { $$ = makeNodeLiteralSymbol($1); }
                | symbol_list
;*/
literalSymbol   : symbol_list ':' kSYM
                 { $$ = makeNodeLiteralSymbol($1); }
;
symbol_list     : symbol_single
                 { $$ = makeList($1, NULL); }
                | symbol_single symbol_list
                 { $$ = makeList($1, $2);   }
;
symbol_single   : '`' symbol_name
                 { $$ = makeNodeConstSymbol($2); }
;
symbol_name     : tID
                 { $$ = $1; }
                | tSTRING
                 { $$ = $1; }
;

/* date */
literalDate     : dateValue_list ':' kDATE
                 { $$ = makeNodeLiteralDate($1); }
                | '(' dateValue_list ')' ':' kDATE
                 { $$ = makeNodeLiteralDate($2); }
;
dateValue_list  : tDATE
                 { $$ = makeList(makeNodeConstDate($1), NULL); }
                | tDATE ',' dateValue_list
                 { $$ = makeList(makeNodeConstDate($1), $3); }
;

compoundID      : tID '.' tID
                 { $$ = makeNodeCompoundID1($1, $3); }
                | tID '.' '*'
                 { $$ = makeNodeCompoundID2($1); }
;

/* type */
type            : '?'
                 { $$ = makeNodeType(unknownT); }
                | kBOOL
                 { $$ = makeNodeType(boolT); }
                | kCHAR
                 { $$ = makeNodeType(charT); }
                | kI16
                 { $$ = makeNodeType(i16T); }
                | kI32
                 { $$ = makeNodeType(i32T); }
                | kI64
                 { $$ = makeNodeType(i64T); }
                | kF32
                 { $$ = makeNodeType(f32T); }
                | kF64
                 { $$ = makeNodeType(f64T); }
                | kCLEX
                 { $$ = makeNodeType(clexT); }
                | kSYM
                 { $$ = makeNodeType(symT); }
                | kSTR
                 { $$ = makeNodeType(strT); }
                | kMONTH
                 { $$ = makeNodeType(monthT); }
                | kDATE
                 { $$ = makeNodeType(dateT); }
                | tID
                 { $$ = makeNodeTypeWithName($1); }
;

int_list        : intValue
                 { $$ = makeList($1, NULL); }
                | intValue ',' int_list
                 { $$ = makeList($1, $3); }
;
intValue        : tINT
                 { $$ = makeNodeIntValue($1, '+'); }
                | '+' tINT
                 { $$ = makeNodeIntValue($2, '+'); }
                | '-' tINT
                 { $$ = makeNodeIntValue($2, '-'); }
;

float_list      : floatValue
                 { $$ = makeList($1, NULL); }
                | floatValue ',' float_list
                 { $$ = makeList($1, $3); }
;
floatValue      : tFLOAT
                 { $$ = makeNodeFloatValue($1, '+'); }
                | '+' tFLOAT
                 { $$ = makeNodeFloatValue($2, '+'); }
                | '-' tFLOAT
                 { $$ = makeNodeFloatValue($2, '-'); }
;

string_list     : tSTRING
                 { $$ = makeList(makeNodeStringValue($1), NULL); }
                | tSTRING ',' string_list
                 { $$ = makeList(makeNodeStringValue($1), $3); }
%%
