%{
#include <stdio.h>
#include <stdlib.h>
#define P printf
#define D(x) printf("Debug:" x " %s" "\n",yytext)
%}

%code requires {
    //#include "tree.h"
}

%union {
    int    intconst;
    char  *stringconst;
    double floatconst;
    //struct Prog *program;
    //struct List *list;
    //struct Node *node;
}

%code {
    extern char *yytext;
    extern int yylineno;
    //extern Prog *root;
    int yylex(); /* defined in lex.yy.c */

    void yyerror() {
        fprintf(stderr, "[Syntax Error] line %d near %s\n", yylineno, yytext);
        exit(1);
    }
}

%token <stringconst> kSELECT kFROM kWHERE kGROUPBY kORDERBY kHAVING kLIMIT kDISTINCT kAS kIN kNOT kLIKE kBETWEEN kIS kNULL kDATE kCASE kWHEN kTHEN kELSE kEND kAND kOR kINTERVAL kEXISTS kEXTRACT kSUBSTRING kFOR kLEFTJOIN kRIGHTJOIN kON kCREATE kDROP kVIEW
%token <stringconst> fAVG fSUM fCOUNT fMIN fMAX
%token <stringconst> aNOT2 aLE aGE aLT aGT aEQ aNOT
%token <stringconst> tID tSTRING tSINGLE kASC kDESC
%token <intconst>    tINT
%token <floatconst>  tFLOAT

/* TODO: need to clean this part */
//%type <prog> program 
//%type <list> select_expr_list table_expression expression and_condition expression_list
//%type <node> set_operation select_expression table_name_expr where_stmt groupby_stmt orderby_stmt having_stmt limit_stmt condition operand expr_stmt const_value term function_stmt literal_date case_stmt case_when_stmt name arith_op compare_op basic_func_op
/*%type <stringconst> compare_op basic_func_op */

%start program

%% 

program            : sql_view_list sql_query ';' sql_view_list
;

sql_query          : kSELECT select_expr_list
                     kFROM table_expression
                     optional_stmt
;

sql_view_list      :
                   | sql_view sql_view_list
;

sql_view           : sql_create_view
                   | sql_drop_view
;

sql_create_view    : kCREATE kVIEW tID parameter_list kAS sql_query ';' //q15
;

sql_drop_view      : kDROP kVIEW term ';'

optional_stmt      :   where_stmt
                     groupby_stmt
                      having_stmt
                     orderby_stmt
                       limit_stmt
;

/* SELECT part */
select_expr_list   : select_expression
                   | select_expression ',' select_expr_list
;

select_expression  : condition
                   | condition kAS tID
;

/* FROM */
table_expression   : table_name_expr
                   | table_name_expr ',' table_expression
;

table_name_expr    : term
                   | term tID
                   | term kAS tID
                   | term kAS tID parameter_list   // q13
                   | term kLEFTJOIN term kON expression
;

parameter_list     : '(' expression_list ')'

/* WHERE / GROUP BY / ORDER BY / HAVING */

where_stmt         :
                   | kWHERE expression
;

groupby_stmt       :
                   | kGROUPBY expression_list
;

orderby_stmt       :
                   | kORDERBY expression_list
;

having_stmt        :
                   | kHAVING expression
;

limit_stmt         :
                   | kLIMIT tINT
;

/* expressions */
expression_list    : expression
                   | expression_list ',' expression
;

expression         : and_list
                   | and_list kOR expression
;

and_list           : condition 
                   | condition kAND and_list
;

/*
and_condition      : 
                   | condition kAND and_condition
;
*/

condition          : operand condition_right
                   | kNOT condition           /* kNOT expression */
                   //| '(' expression ')'
;

condition_right    :
                   | compare_op operand
                   | condition_not kIN term
                   | condition_not kLIKE operand
                   | condition_not kBETWEEN operand
                   | kIS kNULL
;

condition_not      : 
                   | kNOT
;

operand            : term 
                   | term arith_op operand
;

compare_op         : aNOT2
                   | aLE
                   | aGE
                   | aLT
                   | aGT
                   | aEQ
                   | aNOT
;

const_value        : tINT
                   | tFLOAT
                   | literal_date
                   | interval_date
                   | tSINGLE
/*
                   | iteral_boolean
                   | iteral_null
*/
;

term               : '*'
                   | show_distinct name orderby_order
                   |     const_value
                   |      case_stmt
                   | case_when_stmt
                   |  function_stmt
                   |    exists_stmt
                   |   extract_stmt
                   | substring_stmt
                   | '(' expression_list ')'
                   | '(' sql_query ')'
;

function_stmt      : basic_func_op term
;

basic_func_op      : fAVG
                   | fSUM
                   | fCOUNT
                   | fMIN
                   | fMAX
;

exists_stmt        : kEXISTS term
;

orderby_order      :
                   | kASC
                   | kDESC
;

extract_stmt       : kEXTRACT '(' tID kFROM tID ')'
;

substring_stmt     : kSUBSTRING '(' tID kFROM tINT kFOR tINT ')'
;

show_distinct      :
                   | kDISTINCT
;

literal_date       : kDATE tSINGLE
;

interval_date      : kINTERVAL tSINGLE date_unit date_other
;

/*
literal_date_right :
                   | arith_op kINTERVAL '\'' tINT '\'' date_unit
;
*/

date_unit          : tID
;

/*
date_unit          : kDAY
                   | kYEAR
                   | kMONTH
;
*/

date_other         :
                   |'(' tINT ')'
;

arith_op           : '+'
                   | '-'
                   | '*'
                   | '/'
;

case_stmt          : kCASE term kWHEN expression kTHEN expression case_else_stmt kEND
;

case_when_stmt     : kCASE kWHEN expression kTHEN expression case_else_stmt kEND
;

case_else_stmt     :
                   | kELSE expression
;

name               : tID
                   | tID '.' tID
                   | tSTRING
;

%%
