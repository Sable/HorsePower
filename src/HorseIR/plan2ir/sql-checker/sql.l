%{
#include "y.tab.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void addStr();
%}

%option yylineno
%option caseless
%option noyywrap

DIGIT           [0-9]
NONZERODIGIT    [1-9]
LETTER          [a-zA-Z_]

%%

[ \t\r]+   ; /* ignore white spaces         */
--.*\n     ; /* ignore single line comments */

\n ; //{ printf("---- LINE %2d ----\n", yylineno); }

 /* reserved words */
"("  return '(';
")"  return ')';
"{"  return '{';
"}"  return '}';
","  return ',';
"["  return '[';
"]"  return ']';
"."  return '.';
"\"" return '"';
"+"  return '+';
"-"  return '-';
"\*" return '*';
"/"  return '/';
";"  return ';';

 /* keywords */
"select"      {addStr(); return kSELECT;}
"from"        {addStr(); return kFROM;}
"where"       {addStr(); return kWHERE;}
"having"      {addStr(); return kHAVING;}
"limit"       {addStr(); return kLIMIT;}
"distinct"    {addStr(); return kDISTINCT;}
"as"          {addStr(); return kAS;}
"in"          {addStr(); return kIN;}
"on"          {addStr(); return kON;}
"not"         {addStr(); return kNOT;}
"like"        {addStr(); return kLIKE;}
"between"     {addStr(); return kBETWEEN;}
"is"          {addStr(); return kIS;}
"null"        {addStr(); return kNULL;}
"date"        {addStr(); return kDATE;}
"interval"    {addStr(); return kINTERVAL;}
"case"        {addStr(); return kCASE;}
"when"        {addStr(); return kWHEN;}
"then"        {addStr(); return kTHEN;}
"else"        {addStr(); return kELSE;}
"end"         {addStr(); return kEND;}
"and"         {addStr(); return kAND;}
"or"          {addStr(); return kOR;}
"exists"      {addStr(); return kEXISTS;}
"asc"         {addStr(); return kASC;}
"desc"        {addStr(); return kDESC;}
"extract"     {addStr(); return kEXTRACT;}
"substring"   {addStr(); return kSUBSTRING;}
"for"         {addStr(); return kFOR;}
"create"      {addStr(); return kCREATE;}
"drop"        {addStr(); return kDROP;}
"view"        {addStr(); return kVIEW;}

"avg"         {addStr(); return fAVG;}
"sum"         {addStr(); return fSUM;}
"count"       {addStr(); return fCOUNT;}
"min"         {addStr(); return fMIN;}
"max"         {addStr(); return fMAX;}

"<>"          {addStr(); return aNOT2;}
"<="          {addStr(); return aLE;}
">="          {addStr(); return aGE;}
"<"           {addStr(); return aLT;}
">"           {addStr(); return aGT;}
"="           {addStr(); return aEQ;}
"!="          {addStr(); return aNOT;}

left[ ]+outer[ ]+join  { addStr(); return kLEFTJOIN;  }
right[ ]+outer[ ]+join { addStr(); return kRIGHTJOIN; }
group[ ]+by            { addStr(); return kGROUPBY;}
order[ ]+by            { addStr(); return kORDERBY;}

0|({NONZERODIGIT}{DIGIT}*) {
    //printf("-- Saw an integer %s\n", yytext);
    yylval.intconst = atoi(yytext);
    return tINT;
}
{LETTER}({LETTER}|{DIGIT})* {
    //printf("-- Saw a token %s\n", yytext);
    yylval.stringconst = (char*)malloc(strlen(yytext)+1);
    sprintf(yylval.stringconst, "%s", yytext);
    return tID;
}
(0\.{DIGIT}*)|(\.{DIGIT}*)|({NONZERODIGIT}{DIGIT}*\.{DIGIT}*) {
    //printf("-- Saw a double %s\n", yytext);
    yylval.floatconst = atof(yytext);
    return tFLOAT;
}
(\"[^"]*\")|(\'[^']*\') {
    printf("-- Saw a string %s\n", yytext);
    int len = strlen(yytext);
    yylval.stringconst = (char*)malloc(len-1);
    sprintf(yylval.stringconst, "%s", yytext+1);
    yylval.stringconst[len-2] = 0;
    return (yytext[0]=='"'?tSTRING:tSINGLE);
}

.     return yytext[0];
%%

void addStr() {
    yylval.stringconst = (char*)malloc(strlen(yytext)+1);
    sprintf(yylval.stringconst, "%s", yytext);
}

