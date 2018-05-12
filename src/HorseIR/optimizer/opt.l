%{
#include "y.tab.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void addStr();
%}

%option yylineno

DIGIT           [0-9]
NONZERODIGIT    [1-9]
LETTER          [a-zA-Z_]

%%

[ \t\r]+   ; /* ignore white spaces         */
\/\/.*\n     ; /* ignore single line comments */

\n { printf("---- LINE %2d ----\n", yylineno); }

 /* reserved words */
"(" return '(';
")" return ')';
"{" return '{';
"}" return '}';
";" return ';';
"," return ',';
"[" return '[';
"]" return ']';
"." return '.';
"&" return '&';
"\"" return '"';
":" return ':';
"=" return '=';
"+" return '+';
"-" return '-';
"\*"return '*';
"\?"return '?';
"@" return '@';

 /* keywords */
"bool"        {addStr(); return kBOOL;}
"char"        {addStr(); return kCHAR;}
"i8"          {addStr(); return kI8;}
"i16"         {addStr(); return kI16;}
"i32"         {addStr(); return kI32;}
"i64"         {addStr(); return kI64;}
"f32"         {addStr(); return kF32;}
"f64"         {addStr(); return kF64;}
"complex"     {addStr(); return kCLEX;}
"sym"         {addStr(); return kSYM;}
"str"         {addStr(); return kSTR;}
"m"           {addStr(); return kMONTH;}
"d"           {addStr(); return kDATE;}
"z"           {addStr(); return kTZ;}
"u"           {addStr(); return kTU;}
"v"           {addStr(); return kTV;}
"t"           {addStr(); return kTT;}
"module"      {addStr(); return kMODULE;}
"import"      {addStr(); return kIMPORT;}
"def"         {addStr(); return kDEF;}
"check_cast"  {addStr(); return kCKCAST;}
"return"      {addStr(); return kRETURN;}

0|({NONZERODIGIT}{DIGIT}*) {
    //printf("-- Saw an integer %s\n", yytext);
    yylval.intconst = atoi(yytext);
    return tINT;
}
{LETTER}({LETTER}|{DIGIT})* {
    printf("-- Saw a token %s\n", yytext);
    yylval.stringconst = (char*)malloc(strlen(yytext)+1);
    sprintf(yylval.stringconst, "%s", yytext);
    return tID;
}
(0\.{DIGIT}*)|({NONZERODIGIT}{DIGIT}*\.{DIGIT}*) {
    //printf("-- Saw a double %s\n", yytext);
    yylval.floatconst = atof(yytext);
    return tFLOAT;
}
`{LETTER}({LETTER}|{DIGIT})* {
    printf("-- Saw a symbol %s\n", yytext);
    yylval.stringconst = (char*)malloc(strlen(yytext)+1);
    sprintf(yylval.stringconst, "%s", yytext+1);
    return tSYMBOL;
}
{DIGIT}{DIGIT}{DIGIT}{DIGIT}\.{DIGIT}{DIGIT}\.{DIGIT}{DIGIT} {
    //printf("-- Saw a date %s\n", yytext);
    int yy,mm,dd;
    sscanf(yytext, "%d.%d.%d",&yy,&mm,&dd);
    yylval.intconst = yy*10000+mm*100+dd;
    return tDATE;
}
'[^']*' {
    yylval.stringconst = (char*)malloc(strlen(yytext)+1);
    sprintf(yylval.stringconst, "%s", yytext);
    return tCHAR;
}
\"[^"]*\" {
    printf("-- Saw a string %s\n", yytext);
    int len = strlen(yytext);
    yylval.stringconst = (char*)malloc(len-1);
    sprintf(yylval.stringconst, "%s", yytext+1);
    yylval.stringconst[len-2] = 0;
    return tSTRING;
}

.     return yytext[0];
%%

void addStr() {
    yylval.stringconst = (char*)malloc(strlen(yytext)+1);
    sprintf(yylval.stringconst, "%s", yytext);
}

