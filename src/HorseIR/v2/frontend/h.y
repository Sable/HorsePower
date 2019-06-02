%{
#include <stdio.h>
#include <stdlib.h>
#define P printf
#define D(x) P("Debug:" x " %s" "\n",yytext)
%}

%code requires {
    #include "tree.h"
}

%union {
    int          intconst;
    long long    longconst;
    char        *stringconst;
    double       floatconst;
    struct Prog *program;
    struct List *list;
    struct Node *node;
    struct ConstValue *constvalue;
}

%code {
    extern char *yytext;
    extern int yylineno;
    extern Prog *root;
    int yylex(); /* defined in lex.yy.c */

    void yyerror() {
        fprintf(stderr, "[Syntax Error] line %d near %s\n", yylineno, yytext);
        exit(1);
    }
}

%token <stringconst> kMODULE kIMPORT kDEF kKERNEL kGLOBAL kCKCAST kIF kELSE kWHILE kREPEAT kRETURN kBREAK kCONTINUE kVAR
//%token <stringconst> kBOOL kCHAR kI16 kI32 kI64 kF32 kF64 kCLEX kSTR kSYM kDT kDATE kMONTH kMINUTE kSECOND kTIME kFUNC kLIST kDICT kENUM kTABLE kKTABLE
//%token <stringconst> kLIST kDICT kENUM kTABLE kKTABLE
%token <stringconst> tID tCHAR tSTRING

%token <intconst> tINT tDATE tMONTH tTIME tSECOND tMINUTE
%token <floatconst> tFLOAT
%token <longconst> tDT

%type <prog> program

%type <node> module moduleContent importDirective globalDecl functionDecl
%type <node> params param typ wildCard generalTypes
%type <node> stmt block assignStmt controlStmt expressionStmt varDecl
%type <node> expression var name ifStmt whileStmt repeatStmt returnStmt
%type <node> breakStmt continueStmt condition controlBlock
%type <node> operand functionCall paramExpr cast functionId literal literalFunc
%type <node> literalVector value

%type <list> module_list moduleContent_list import_list id_list param_list
%type <list> typ_list stmt_list var_list operand_list value_list 

%type <constvalue> realValue complexValue realNumber numericValue 
%type <constvalue> charValue stringValue symbolValue calendarValue

%type <stringconst> funcKind

%start program

%nonassoc kTHEN  /* declared as %prec kTHEN */
%nonassoc kELSE

%left '-' '+'
%left '*'

%%

program            : module_list
                     { root = makeProg($1); }
;


module_list        : 
                     { $$ = NULL; }
                   | module_list module
                     { $$ = makeList($1, $2); }
;

module             : kMODULE tID '{' moduleContent_list '}'
                     { $$ = makeNodeModule($2, $4); }
;

moduleContent_list :
                     { $$ = NULL; }
                   | moduleContent_list moduleContent
                     { $$ = makeList($1, $2); }
;

moduleContent      : importDirective
                     { $$ = $1; }
                   | globalDecl
                     { $$ = $1; }
                   | functionDecl
                     { $$ = $1; }
;

 /* import */
importDirective    : kIMPORT tID '.' import_list ';'
                     { $$ = makeNodeImport($2, $4); }
;

import_list        : '*'
                     { $$ = NULL; }
                   | tID
                     { $$ = makeList(NULL, makeNodeID($1)); }
                   | '{' id_list '}'
                     { $$ = $2; }
;

id_list            : tID
                     { $$ = makeList(NULL, makeNodeID($1)); }
                   | id_list ',' tID
                     { $$ = makeList($1, makeNodeID($3)); }
;

 /* function declaration */
functionDecl       : funcKind tID '(' params ')' block
                     { $$ = makeNodeModuleMethod($1, $2, $4, NULL, $6); }
                   | funcKind tID '(' params ')' ':' typ_list block
                     { $$ = makeNodeModuleMethod($1, $2, $4, $7, $8); }
;

funcKind           : kDEF
                     { $$ = $1; }
                   | kKERNEL
                     { $$ = $1; }
;

params             :
                     { $$ = NULL; }
                   | param_list
                     { $$ = makeNodeParamExpr($1); }
;

param_list         : param
                     { $$ = makeList(NULL, $1); }
                   | param_list ',' param
                     { $$ = makeList($1, $3); }
;

param              : tID ':' typ
                     { $$ = makeNodeVar($1, $3); }
;

 /* global declaration */

globalDecl         : kGLOBAL tID ':' typ '=' operand ';'
                     { $$ = makeNodeGlobal($2, $4, $6); }
;

 /* types and shapes */
typ                : wildCard
                     { $$ = $1; }
                   | generalTypes
                     { $$ = $1; }
;

wildCard           : '?'
                     { $$ = makeNodeType(true, NULL, NULL); }
;

generalTypes       : tID
                     { $$ = makeNodeType(false, $1, NULL); }
                   | tID '<' typ_list '>'  /* list(any),enum(1/2),dict(2) */
                     { $$ = makeNodeType(false, $1, $3); }

typ_list           : typ
                     { $$ = makeList(NULL, $1); }
                   | typ_list ',' typ
                     { $$ = makeList($1, $3); }
;

 /* block */
block              : '{' stmt_list '}'
                     { $$ = makeNodeBlock($2); }
;

stmt_list          :
                     { $$ = NULL; }
                   | stmt_list stmt
                     { $$ = makeList($1, $2); }
;

stmt               : assignStmt
                     { $$ = $1; }
                   | controlStmt
                     { $$ = $1; }
                   | expressionStmt
                     { $$ = $1; }
                   | varDecl
                     { $$ = $1; }
;

assignStmt         : var_list '=' expression ';'
                     { $$ = makeNodeAssignment($1, $3); }

var_list           : var
                     { $$ = makeList(NULL, $1); }
                   | var_list ',' var
                     { $$ = makeList($1, $3); }
;

var                : name
                     { $$ = $1; }
                   | tID ':' typ
                     { $$ = makeNodeVar($1, $3); }
;

name               : tID
                     { $$ = makeNodeName(NULL, $1); }
                   | tID '.' tID
                     { $$ = makeNodeName($1, $3); }
;


controlStmt        : ifStmt
                     { $$ = $1; }
                   | whileStmt
                     { $$ = $1; }
                   | repeatStmt
                     { $$ = $1; }
                   | returnStmt
                     { $$ = $1; }
                   | breakStmt
                     { $$ = $1; }
                   | continueStmt
                     { $$ = $1; }
;

controlBlock       : stmt
                     { $$ = $1; }
                   | block
                     { $$ = $1; }
;

ifStmt             : kIF '(' condition ')' controlBlock  %prec kTHEN
                     { $$ = makeNodeStmtIf($3, $5, NULL); }
                   | kIF '(' condition ')' controlBlock kELSE controlBlock
                     { $$ = makeNodeStmtIf($3, $5, $7); }
;

whileStmt          : kWHILE '(' condition ')' controlBlock
                     { $$ = makeNodeStmtWhile($3, $5); }
;

repeatStmt         : kREPEAT '(' condition ')' controlBlock
                     { $$ = makeNodeStmtRepeat($3, $5); }
;

expressionStmt     : expression ';'
                     { $$ = makeNodeStmtExpr($1); }
;

varDecl            : kVAR id_list ':' typ ';'
                     { $$ = makeNodeVarDecl($2, $4); }
;

/*
gotoStmt           : kGOTO tID ';'
                     { $$ = makeNodeStmtGoto($2, NULL, NULL); }
                   | kGOTO '(' id_list ')' '/' id_list ';'
                     { $$ = makeNodeStmtGoto(NULL, $3, $6); }
;
*/

returnStmt         : kRETURN operand_list ';'
                     { $$ = makeNodeStmtReturn($2); }
;

breakStmt          : kBREAK ';'
                     { $$ = makeNodeStmtBreak(); }
;

continueStmt       : kCONTINUE ';'
                     { $$ = makeNodeStmtContinue(); }
;

condition          : operand
                     { $$ = $1; }
;

/* expression */

expression         : functionCall
                     { $$ = $1; }
                   | operand
                     { $$ = $1; }
                   | cast
                     { $$ = $1; }
;

functionCall       : functionId '(' paramExpr ')'
                     { $$ = makeNodeFuncCall($1, $3); }
;

functionId         : '@' name
                     { $$ = $2; }
;

paramExpr          :
                     { $$ = NULL; }
                   | operand_list
                     { $$ = makeNodeArgExpr($1); }
;

operand_list       : operand
                     { $$ = makeList(NULL, $1); }
                   | operand_list ',' operand
                     { $$ = makeList($1, $3); }
;

operand            : name
                     { $$ = $1; }
                   | literal
                     { $$ = $1; }
;

literal            : literalVector
                     { $$ = $1; }
                   | literalFunc
                     { $$ = $1; }
;

literalFunc        : functionId
                     { $$ = makeNodeLiteralFunc($1, NULL); }
                   | functionId ':' typ
                     { $$ = makeNodeLiteralFunc($1, $3); }
;

literalVector      : value ':' typ
                     { $$ = makeNodeLiteralVector(true, makeList(NULL, $1), $3); }
                   | '(' value_list ')' ':' typ
                     { $$ = makeNodeLiteralVector(false,$2, $5); }
;

value_list         : value
                     { $$ = makeList(NULL, $1); }
                   | value_list ',' value
                     { $$ = makeList($1, $3); }
;

value              : numericValue
                     { $$ = makeNodeConst($1); }
                   | charValue
                     { $$ = makeNodeConst($1); }
                   | stringValue
                     { $$ = makeNodeConst($1); }
                   | symbolValue
                     { $$ = makeNodeConst($1); }
                   | calendarValue
                     { $$ = makeNodeConst($1); }
;

numericValue       : realValue
                     { $$ = $1; }
                   | complexValue
                     { $$ = $1; }

realValue          : realNumber
                     { $$ = makeReal('+',$1); }
                   | '-' realNumber     %prec '*'
                     { $$ = makeReal('-',$2); }
                   | '+' realNumber     %prec '*'
                     { $$ = makeReal('+',$2); }
;

complexValue       : realValue '-' realNumber
                     { $$ = makeComplex($1, '-', $3); }
                   | realValue '+' realNumber
                     { $$ = makeComplex($1, '+', $3); }
;

realNumber         : tINT
                     { $$ = makeLong($1); }
                   | tFLOAT
                     { $$ = makeFloat($1); }
;

charValue          : tCHAR
                     { $$ = makeChar($1); }
;

stringValue        : tSTRING
                     { $$ = makeString($1); }
;

symbolValue        : '`' tID
                     { $$ = makeSymbol($2); }
                   | '`' tSTRING
                     { $$ = makeSymbol($2); }
;

calendarValue      : tDT
                     { $$ = makeDT($1);     }
                   | tDATE
                     { $$ = makeDate($1);   }
                   | tMONTH
                     { $$ = makeMonth($1);  }
                   | tTIME
                     { $$ = makeTime($1);   }
                   | tMINUTE
                     { $$ = makeMinute($1); }
                   | tSECOND
                     { $$ = makeSecond($1); }

;

 /* check cast */
cast               : kCKCAST '(' expression ',' typ ')'
                     { $$ = makeNodeCast($3, $5); }
;

%%


