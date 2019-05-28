%{
#include "y.tab.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void addStr();
int convertMonth (int yy, int mm);
int convertDate  (int yy, int mm, int dd);
int convertSecond(int hh, int mm, int ss);

%}

%option yylineno
%option noyywrap

letter     [a-zA-Z_]
digit      [0-9]
nzdigit    [1-9]

/* escape_sequence  = "\a" | "\b" | "\f" | "\n" | "\r" | "\t" | "\v" ; */

%x BlockComment

%%

[ \t\r]+   ; /* ignore white spaces         */
\/\/.*\n   ; /* ignore single line comments */

\n ; //{ printf("---- LINE %2d ----\n", yylineno); }

"/*"               { BEGIN(BlockComment); }
<BlockComment>"*/" { BEGIN(INITIAL); }
<BlockComment>\n   { }
<BlockComment>.    { }

 /* reserved words */
"("  return '(';
")"  return ')';
"{"  return '{';
"}"  return '}';
";"  return ';';
","  return ',';
"["  return '[';
"]"  return ']';
"<"  return '<';
">"  return '>';
"."  return '.';
"&"  return '&';
"\"" return '"';
":"  return ':';
"="  return '=';
"\*" return '*';
"\?" return '?';
"@"  return '@';
"+"  return '+';
"-"  return '-';

 /* keywords */
"module"      {addStr(); return kMODULE;  }
"import"      {addStr(); return kIMPORT;  }
"def"         {addStr(); return kDEF;     }
"kernel"      {addStr(); return kKERNEL;  }
"global"      {addStr(); return kGLOBAL;  }
"check_cast"  {addStr(); return kCKCAST;  }
"if"          {addStr(); return kIF;      }
"else"        {addStr(); return kELSE;    }
"while"       {addStr(); return kWHILE;   }
"repeat"      {addStr(); return kREPEAT;  }
"goto"        {addStr(); return kGOTO;    }
"return"      {addStr(); return kRETURN;  }
"break"       {addStr(); return kBREAK;   }
"continue"    {addStr(); return kCONTINUE;}

 /* "bool"        {addStr(); return kBOOL;    }*/
 /* "char"        {addStr(); return kCHAR;    }*/
 /* "i16"         {addStr(); return kI16;     }*/
 /* "i32"         {addStr(); return kI32;     }*/
 /* "i64"         {addStr(); return kI64;     }*/
 /* "f32"         {addStr(); return kF32;     }*/
 /* "f64"         {addStr(); return kF64;     }*/
 /* "complex"     {addStr(); return kCLEX;    }*/
 /* "str"         {addStr(); return kSTR;     }*/
 /* "sym"         {addStr(); return kSYM;     }*/
 /* "dt"          {addStr(); return kDT;      }*/
 /* "date"        {addStr(); return kDATE;    }*/
 /* "month"       {addStr(); return kMONTH;   }*/
 /* "minute"      {addStr(); return kMINUTE;  }*/
 /* "second"      {addStr(); return kSECOND;  }*/
 /* "time"        {addStr(); return kTIME;    }*/
 /* "func"        {addStr(); return kFUNC;    }*/

 /* "list"        {addStr(); return kLIST;    }*/
 /* "dict"        {addStr(); return kDICT;    }*/
 /* "enum"        {addStr(); return kENUM;    }*/
 /* "table"       {addStr(); return kTABLE;   }*/
 /* "ktable"      {addStr(); return kKTABLE;  }*/

({letter}|_)({letter}|{digit}|_)* {
    //printf("-- Saw a token %s\n", yytext);
    yylval.stringconst = (char*)malloc(strlen(yytext)+1);
    sprintf(yylval.stringconst, "%s", yytext);
    return tID;
}

0|({nzdigit}{digit}*) {
    //printf("-- Saw an integer %s\n", yytext);
    yylval.longconst = atoi(yytext);  // default: long
    return tINT;
}

(0\.{digit}*)|({nzdigit}{digit}*\.{digit}*) {
    //printf("-- Saw a double %s\n", yytext);
    yylval.floatconst = atof(yytext);
    return tFLOAT;
}

 /* datetime */
{digit}{digit}{digit}{digit}-{digit}{digit}-{digit}{digit}T{digit}{digit}:{digit}{digit}:{digit}{digit}\.{digit}{digit}{digit} {
    int yy,mm,dd,hh,m2,ss,ll;
    sscanf(yytext, "%d-%d-%dT%d:%d:%d.%d",&yy,&mm,&dd,&hh,&m2,&ss,&ll);
    yylval.longconst = convertDate(yy,mm,dd)*86400000LL + convertSecond(hh,m2,ss)*1000 + ll;
    return tDT;
}

 /* date */
{digit}{digit}{digit}{digit}-{digit}{digit}-{digit}{digit} {
    //printf("-- Saw a date %s\n", yytext);
    int yy,mm,dd;
    sscanf(yytext, "%d-%d-%d",&yy,&mm,&dd);
    yylval.intconst = yy*10000+mm*100+dd;
    return tDATE;
}

 /* month */
{digit}{digit}{digit}{digit}-{digit}{digit} {
    //printf("-- Saw a month %s\n", yytext);
    int yy,mm;
    sscanf(yytext, "%d-%d",&yy,&mm);
    yylval.intconst = convertMonth(yy,mm);
    return tMONTH;
}

 /* time */
{digit}{digit}:{digit}{digit}:{digit}{digit}\.{digit}{digit}{digit} {
    int hh,mm,ss,ll;
    sscanf(yytext, "%d:%d:%d.%d",&hh,&mm,&ss,&ll);
    yylval.intconst = convertSecond(hh,mm,ss)*1000 + ll;
    return tTIME;
}

 /* second */
{digit}{digit}:{digit}{digit}:{digit}{digit} {
    int hh,mm,ss;
    sscanf(yytext, "%d:%d:%d",&hh,&mm,&ss);
    yylval.intconst = convertSecond(hh,mm,ss);
    return tSECOND;
}

 /* minute */
{digit}{digit}:{digit}{digit} {
    //printf("-- Saw a minute %s\n", yytext);
    int hh,mm;
    sscanf(yytext, "%d:%d",&hh,&mm);
    yylval.intconst = hh*60+mm;
    return tMINUTE;
}

'[^']*' {
    int len = strlen(yytext);
    yylval.stringconst = (char*)malloc(len-1);
    sprintf(yylval.stringconst, "%s", yytext+1);
    yylval.stringconst[len-2] = 0;
    return tCHAR;
}

\"[^"]*\" {
    //printf("-- Saw a string %s\n", yytext);
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

int convertMonth(int yy, int mm){
    return yy * 100 + mm;
}

int convertDate(int yy, int mm, int dd){
    return yy * 10000 + mm * 100 + dd;
}

int convertSecond(int hh, int mm, int ss){
    return hh * 3600 + mm * 60 + ss;
}

