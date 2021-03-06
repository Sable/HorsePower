#include "../global.h"

int depth;
static bool withAttr = true;
static bool withLine = false;
static bool toC      = false;

#define BUFF_SIZE 10240
static char buff[BUFF_SIZE];

#define comma ','
#define nospace 0

#define newLineBuff() if(withLine) strcat(b, "\n")
#define echo(b,n)     strcat(b,n)
#define printChar(c)  SP(b+strlen(b),"%c",c)
#define printID(b,n)  echo(b,n->val.idS)
#define printFloat(b,n)  SP(b,"%g", n->val.floatS)
#define printInt(b,n)    SP(b,"%d", n->val.intS)
#define printString(b,n) SP(b,"\"%s\"", n->val.strS)
#define printComp1(b,n)  SP(b,"%s",n->val.compoundID.name2)
#define printComp2(b,n)  SP(b,"%s.%s" , n->val.compoundID.name1, n->val.compoundID.name2)
#define printComp(b,n)   {if(n->val.compoundID.name1)printComp2(b,n);else printComp1(b,n);}
#define printDate(b,n)   {int x=n->val.dateS; if(toC)SP(b,"%d",x);else SP(b,"%d.%02d.%02d",x/10000,x%10000/100,x%100);}
#define printSym(b,n)    {if(toC)SP(b,"getSymbol(\"%s\")", n->val.charS);else printSymCommon(b,n->val.charS);}
#define printFunc(b,n)   {SP(b,"@"); prettyNodeBuff(b,n->val.nodeS);}

#define printPlainList(b,n)  {L x=countList(n->val.listS); \
                              if(x>1)printChar('('); prettyListBuff(b, n->val.listS, comma); if(x>1)printChar(')');}
#define printPlainList2(b,n) prettyListBuff(b, n->val.listS, nospace)
#define printPlainFloat(b,n) printPlainList(b,n)
#define printPlainSym(b,n)   printPlainList2(b,n)
#define printPlainDate(b,n)  printPlainList(b,n)
#define printPlainBool(b,n)  printPlainList(b,n)
#define printPlainInt(b,n)   printPlainList(b,n)
#define printPlainStr(b,n)   printPlainList(b,n)
#define printPlainFunc(b,n)  printPlainList2(b,n)

#define printLiteralFloat(b,n) {printPlainFloat(b,n); if(withAttr) strcat(b,":f64");}
#define printLiteralSym(b,n)   {printPlainSym(b,n);   if(withAttr) strcat(b,":sym");}
#define printLiteralDate(b,n)  {printPlainDate(b,n);  if(withAttr) strcat(b,":d"  );}
#define printLiteralChar(b,n)  SP(b,"'%s'", n->val.charS)
#define printLiteralStr(b,n)   {printPlainStr(b,n);   if(withAttr) strcat(b,":str" );}
#define printLiteralBool(b,n)  {printPlainBool(b,n);  if(withAttr) strcat(b,":bool");}
#define printLiteralInt(b,n)   {printPlainInt(b,n);   if(withAttr) strcat(b,":i64" );}
#define printLiteralParam(b,n) prettyNodeBuff(b,n->val.nodeS)
//#define printLiteralFunc(b,n)  {strcpy(b, "@"); prettyNodeBuff(b,n->val.nodeS);}
#define printLiteralFunc(b,n)  {printPlainFunc(b,n);}
#define printParamExpr(b,n)    prettyListBuff(b,n->val.listS, comma)
#define printReturnStmt(b,n)   {SP(b,"return "); prettyNodeBuff(b,n->val.nodeS); strcat(b,withLine?";\n":";");}
#define printImportStmt(b,n)   {SP(b,"import "); prettyNodeBuff(b,n->val.nodeS); strcat(b,withLine?";\n":";");}
#define resetBuff(b) if(b[0]!=0) b+=strlen(b)

static int countList(List *list){
    int tot = 0; while(list){ tot++; list = list->next; } return tot;
}

static void printDepth(char* b){
    resetBuff(b);
    for(int i=0;i<depth;i++){
        SP(b+i*4, "    ");
    }
}

static void printSymCommon(char *b, char *str){
    bool flag = false;
    DOI(strlen(str), if(str[i]==' '){flag=true;break;})
    if(flag) SP(b, "`\"%s\"", str);
    else SP(b, "`%s", str);
}

static void printExprBuff(char *b, Node *n){
    resetBuff(b);
    if(n->val.expr.func){
        prettyNodeBuff(b, n->val.expr.func);
        strcat(b,"("); prettyNodeBuff(b, n->val.expr.param); strcat(b, ")");
    }
    else {
        prettyNodeBuff(b, n->val.expr.param);
    }
}

static void printSimpleStmtBuff(char *b, Node *n){
    resetBuff(b);
    prettyNodeBuff(b, n->val.simpleStmt.name);
    strcat(b,":"); prettyNodeBuff(b, n->val.simpleStmt.type);
    strcat(b,"="); prettyNodeBuff(b, n->val.simpleStmt.expr);
    strcat(b,";");
    newLineBuff();
}

static void printCastStmtBuff(char *b, Node *n){
    resetBuff(b);
    prettyNodeBuff(b, n->val.castStmt.name);
    strcat(b,":"); prettyNodeBuff(b, n->val.castStmt.type);
    strcat(b,"="); strcat(b, "check_cast"); strcat(b,"(");
    prettyNodeBuff(b, n->val.castStmt.expr);
    strcat(b,","); prettyNodeBuff(b,n->val.castStmt.cast); strcat(b,")");
    strcat(b,";");
    newLineBuff();
}

static void printMethodBuff(char *b, Node *n){
    resetBuff(b);
    depth++;
    strcat(b, "def "); prettyNodeBuff(b,n->val.method.name);
    strcat(b, "("); prettyListBuff(b, n->val.method.param, comma); strcat(b, ") : ");
    prettyNodeBuff(b,n->val.method.type);
    strcat(b, " {\n");
    prettyListBuff(b,n->val.method.list, nospace);
    depth--;
    printDepth(b);
    strcat(b,"}\n");
}

static void printModuleBuff(char* b, Node *n){
    resetBuff(b);
    depth++;
    strcat(b,"module "); prettyNodeBuff(b, n->val.module.name); strcat(b," {\n");
    prettyListBuff(b, n->val.module.body, nospace); strcat(b,"}\n");
    depth--;
}
static void printNameTypeBuff(char* b, Node *n){
    prettyNodeBuff(b,n->val.nameType.name);
    strcat(b,":");
    prettyNodeBuff(b,n->val.nameType.type);
}

void prettyNodeBuff(char *b, Node *n){
    switch(n->kind){
        case simpleStmtK:
        case   castStmtK:
        case     importK:
        case     returnK:
        case     methodK: printDepth(b); break;
    }
    resetBuff(b);
    switch(n->kind){
        case           idK: printID              (b,n); break;
        case        floatK: printFloat           (b,n); break;
        case          intK: printInt             (b,n); break;
        case         dateK: printDate            (b,n); break;
        case          symK: printSym             (b,n); break;
        case         typeK: printNodeTypeBuff    (b,n); break;
        case     compoundK: printComp            (b,n); break;
        case         funcK: printFunc            (b,n); break;
        case          strK: printString          (b,n); break;
        case         exprK: printExprBuff        (b,n); break;
        case    paramExprK: printParamExpr       (b,n); break;
        case literalFloatK: printLiteralFloat    (b,n); break;
        case   literalSymK: printLiteralSym      (b,n); break;
        case  literalDateK: printLiteralDate     (b,n); break;
        case  literalCharK: printLiteralChar     (b,n); break;
        case   literalStrK: printLiteralStr      (b,n); break;
        case  literalBoolK: printLiteralBool     (b,n); break;
        case   literalIntK: printLiteralInt      (b,n); break;
        case literalParamK: printLiteralParam    (b,n); break;
        case  literalFuncK: printLiteralFunc     (b,n); break;
        case   simpleStmtK: printSimpleStmtBuff  (b,n); break;
        case     castStmtK: printCastStmtBuff    (b,n); break;
        case       returnK: printReturnStmt      (b,n); break;
        case       importK: printImportStmt      (b,n); break;
        case       methodK: printMethodBuff      (b,n); break;
        case       moduleK: printModuleBuff      (b,n); break;
        case     nameTypeK: printNameTypeBuff    (b,n); break;
        default: EP("[prettyNodeBuff] unexpected node type: %d\n", n->kind);
    }
}

void prettyListBuff(char *b, List *list, char sep){
    if(list){
        List *ptr = list;
        while(ptr){
            resetBuff(b);
            if(ptr->val){
                prettyNodeBuff(b, ptr->val);
                if(sep && ptr->next) printChar(sep);
            }
            ptr = ptr->next;
        }
    }
    //else { strcat(b, "<NULL list found>"); }
}

/* TODO: check TypeNames in tree.c */
void printTypeBuff(char *b, pType p){
    resetBuff(b);
    switch(p){
        case unknownT: echo(b, "?");    break;
        case    boolT: echo(b, "bool"); break;
        case     i16T: echo(b, "i16");  break;
        case     i32T: echo(b, "i32");  break;
        case     i64T: echo(b, "i64");  break;
        case     f32T: echo(b, "f32");  break;
        case     f64T: echo(b, "f64");  break;
        case    charT: echo(b, "char"); break;
        case    clexT: echo(b, "complex"); break;
        case     symT: echo(b, "sym");     break;
        case     strT: echo(b, "str");     break;
        case   monthT: echo(b, "m");       break;
        case    dateT: echo(b, "d");       break;
        case   tableT: echo(b, "table");   break;
        case  ktableT: echo(b, "ktable");  break;
        case    listT: echo(b, "list");    break;
        case    enumT: echo(b, "enum");    break;
        case    dictT: echo(b, "dict");    break;
        case    funcT: echo(b, "func");    break;
        default: EP("Not supported yet.: %d\n", p);
    }
}

void printNodeTypeBuff(char *b, Node *n){
    resetBuff(b);
    pType k = n->val.typeS;
    printTypeBuff(b, k);
}

void printNodeType(Node *n){
    pType k = n->val.typeS;
    printType(k);
}

void prettyNodeBuffNoAttr(char *b, Node *n){
    withAttr = false;
    prettyNodeBuff(b,n);
    withAttr = true;
}

void prettyNodeBuff2C(char *b, Node *n){
    toC = true;
    prettyNodeBuffNoAttr(b,n);
    toC = false;
}

void prettyNodeBuffNoLine(char *b, Node *n){
    withLine = false;
    prettyNodeBuff(b,n);
    withLine = true;
}

void prettyNode(Node *n){
    buff[0]=0;
    prettyNodeBuff(buff, n);
    P("%s",buff);
}

void printKind(Kind k){
    P("%s\n",getKindName(k));
}

void prettyList(List *list, char sep){
    buff[0]=0;
    prettyListBuff(buff, list, sep);
    P("List: %s\n",buff);
}

void printType(pType p){
    buff[0]=0;
    printTypeBuff(buff, p);
    P("type(%s)",buff);
}

void printNodeKind(Node *n){
    printKind(n->kind);
} 

void prettyProg(Prog *root){
    buff[0]=0;
    printBanner("Pretty Printer");
    depth = 0;
    prettyListBuff(buff, root->module_list, nospace);
    P("%s\n", buff);
}

