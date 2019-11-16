#include "../global.h"

int depth;
static bool withLine = false;
static bool toC      = false;

#define BUFF_SIZE 10240
static char buff[BUFF_SIZE];

#define nospace 0

#define echo(b,n)        strcat(b,n)
#define printLine(b)     if(withLine) echo(b,"\n")
#define printChar(c)     SP(b+strlen(b),"%c",c)
#define printID(b,n)     echo(b,n->val.idS)
#define printName1(b,n)  SP(b,"%s",n->val.name.id2)
#define printName2(b,n)  SP(b,"%s.%s", n->val.name.id1, n->val.name.id2)
#define printName(b,n)   {if(n->val.name.one)printName1(b,n);else printName2(b,n);}
#define printVar(b,n)    {SP(b,"%s:", n->val.param.id); prettyNodeBuff(b,n->val.param.typ);}

#define printArgExpr(b,n)   prettyListBuff(b,n->val.listS,comma)
#define printParamExpr(b,n)   prettyListBuff(b,n->val.listS,comma)
#define printReturnStmt(b,n)  {SP(b,"return "); prettyListBuff(b,n->val.listS,comma); strcat(b, ";"); printLine(b); }

#define resetBuff(b) if(b[0]!=0) b+=strlen(b)

/* declaration */
static void printBlockBuff(char *b, Node *n);
static void prettyNodeBuff(char *b, Node *n);

static int countList(List *list){
    int tot = 0; while(list){ tot++; list = list->next; } return tot;
}

static void printDepth(char* b){
    if(withLine) {
        resetBuff(b);
        for(int i=0;i<depth;i++){
            SP(b+i*4, "    ");
        }
    }
}

// output: A<sep> B<sep> C
static void prettyListBuff(char *b, List *list, char sep){
    if(list){
        prettyListBuff(b, list->next, sep);
        if(sep && list->next) { printChar(sep); echo(b," "); }
        resetBuff(b);
        if(list->val){
            prettyNodeBuff(b, list->val);
        }
    }
}

// output: <sep>A<sep>B
static void prettyListBuffv2(char *b, List *list, char sep){
    if(list){
        prettyListBuff(b, list->next, sep);
        if(sep) printChar(sep);
        resetBuff(b);
        if(list->val){
            prettyNodeBuff(b, list->val);
        }
    }
}

static void printNodeFunc(char *b, Node *n){
    resetBuff(b);
    prettyListBuffv2(b,n->val.listS,'@');
}

static void printNodeTypeBuff(char *b, Node *n){
    resetBuff(b);
    if(n->val.type.isWild) echo(b, "?");
    else if(n->val.type.cell){
        echo(b, n->val.type.typ);
        echo(b, "<");
        prettyListBuff(b, n->val.type.cell, comma);
        echo(b, ">");
    }
    else echo(b, n->val.type.typ);
}

static void printStmtBuff(char *b, Node *n){
    resetBuff(b);
    prettyListBuff(b, n->val.assignStmt.vars, comma);
    strcat(b," = ");
    prettyNodeBuff(b, n->val.assignStmt.expr);
    strcat(b,";");
    printLine(b);
}
static void printCastBuff(char *b, Node *n){
    resetBuff(b);
    echo(b, "check_cast");
    echo(b, "(");
    prettyNodeBuff(b, n->val.cast.exp);
    echo(b, ",");
    prettyNodeBuff(b, n->val.cast.typ);
    echo(b, ")");
}

static void printImportStmt(char *b, Node *n){
    resetBuff(b);
    SP(b, "import %s.", n->val.import.module);
    List *names = n->val.import.content;
    int total = countList(names);
    if(total>1) echo(b, "{");
    if(names) prettyListBuff(b, names, comma);
    else echo(b, "*");
    if(total>1) echo(b, "}");
    echo(b, ";");
    printLine(b);
}

static void printMethodBuff(char *b, Node *n){
    resetBuff(b);
    //depth++;
    echo(b, n->val.method.kind);
    echo(b, " ");
    echo(b, n->val.method.fname);
    echo(b, "(");
    prettyNodeBuff(b, n->val.method.param);
    echo(b, ")");
    if(n->val.method.typ){
        echo(b, " : ");
        prettyListBuff(b, n->val.method.typ, comma);
    }
    echo(b, " ");
    prettyNodeBuff(b, n->val.method.block);
    //depth--;
    //printDepth(b);
}

static void printModuleBuff(char* b, Node *n){
    resetBuff(b);
    depth++;
    SP(b,"module %s", n->val.module.id);
    echo(b," {\n");
    prettyListBuff(b, n->val.module.body, nospace);
    echo(b,"}\n");
    depth--;
}

static void printIfBuff(char *b, Node *n){
    resetBuff(b);
    echo(b, "if (");
    prettyNodeBuff(b, n->val.ifStmt.condExpr);
    echo(b, ")");
    Node *thenBlock = n->val.ifStmt.thenBlock;
    if(thenBlock->kind == blockK){
        printBlockBuff(b, thenBlock);
    }
    else {
        printLine(b);
        depth++;
        prettyNodeBuff(b, n->val.ifStmt.thenBlock);
        depth--;
    }
    if(n->val.ifStmt.elseBlock){
        printDepth(b); echo(b, "else");
        prettyNodeBuff(b, n->val.ifStmt.elseBlock);
    }
}

static void printWhileBuff(char *b, Node *n){
    resetBuff(b);
    echo(b, "while (");
    prettyNodeBuff(b, n->val.whileStmt.condExpr);
    echo(b, ")");
    prettyNodeBuff(b, n->val.whileStmt.bodyBlock);
}

static void printRepeatBuff(char *b, Node *n){
    resetBuff(b);
    echo(b, "repeat (");
    prettyNodeBuff(b, n->val.repeatStmt.condExpr);
    echo(b, ")");
    prettyNodeBuff(b, n->val.repeatStmt.bodyBlock);
}

static void printExprStmtBuff(char *b, Node *n){
    resetBuff(b);
    prettyNodeBuff(b, n->val.exprStmt.expr);
    printLine(b);
}

/*
static void printGotoBuff(char *b, Node *n){
    resetBuff(b);
    echo(b, "goto ");
    if(n->val.gotoStmt.targ)
        echo(b, n->val.gotoStmt.targ);
    else {
        echo(b, "(");
        prettyListBuff(b, n->val.gotoStmt.val, comma);
        echo(b, "/");
        prettyListBuff(b, n->val.gotoStmt.label, comma);
        echo(b, ")");
    }
    echo(b, ";");
    printLine(b);
}
*/

#define printBreakBuff(b,n)     printBreakContinueBuff(b,n,"break")
#define printContinueBuff(b,n)  printBreakContinueBuff(b,n,"continue")

static void printBreakContinueBuff(char *b, Node *n, const char *op){
    SP(b, "%s", op);
    echo(b, ";");
    printLine(b);
}

static void printGlobalBuff(char *b, Node *n){
    echo(b, "global ");
    echo(b, n->val.global.id);
    echo(b, ":");
    prettyNodeBuff(b, n->val.global.typ);
    echo(b, "=");
    prettyNodeBuff(b, n->val.global.op);
    echo(b, ";");
    printLine(b);
}

static void printCallBuff(char *b, Node *n){
    echo(b,"@");
    prettyNodeBuff(b, n->val.call.func);
    echo(b, "(");
    prettyNodeBuff(b, n->val.call.param);
    echo(b, ")");
}

static void printVectorBuff(char *b, Node *n){
    if(!n->val.vec.one) echo(b, "(");
    prettyListBuff(b, n->val.vec.val, comma);
    if(!n->val.vec.one) echo(b, ")");
    echo(b, ":");
    prettyNodeBuff(b, n->val.vec.typ);
}

#define printMonth(x)  SP(b,"%04d-%02d",x/100,x%100)
#define printDate(x)   SP(b,"%04d-%02d-%02d",x/10000,x%10000/100,x%100)
#define printMinute(x) SP(b,"%02d:%02d",x/60,x%60)
#define printSecond(x) SP(b,"%02d:%02d:%02d",x/3600,x/60%60,x%60)
#define printTime(x)   {int x1=x/1000;int x2=x%1000; \
                       SP(b,"%02d:%02d:%02d.%03d",x1/3600,x1/60%60,x1%60,x2);}
#define printDT(x)     {int d1=(int)(x/86400000);int d2=(int)(x%86400000); \
                       printDate(d1);echo(b,"T");resetBuff(b);printTime(d2);}

static void prettyConst(char *b, ConstValue *c){
    //P("val = %d\n", c->valI); getchar();
    switch(c->type){
        case    intC: SP(b,"%d"     ,c->valI); break;
        case  floatC: SP(b,"%g"     ,c->valF); break;
        case   longC: SP(b,"%lld"   ,c->valL); break;
        case   clexC: SP(b,"%g%c%gi",c->valX[0], \
                      c->valX[1]>=0?'+':'-', c->valX[1]); break;
        case   charC: SP(b,"'%s'"   ,c->valS); break;
        case    symC: SP(b,"`%s"    ,c->valS); break;
        case    strC: SP(b,"\"%s\"" ,c->valS); break;
        case   dateC: printDate(c->valI);   break;
        case  monthC: printMonth(c->valI);  break;
        case minuteC: printMinute(c->valI); break;
        case secondC: printSecond(c->valI); break;
        case   timeC: printTime(c->valI);   break;
        case     dtC: printDT(c->valL);     break;
        default: EP("Invalid type: %d\n", c->type); break;
    }
}

static void printConstBuff(char *b, Node *n){
    prettyConst(b, n->val.nodeC);
}

static void printBlockBuff(char *b, Node *n){
    echo(b, "{"); printLine(b);
    depth++;
    prettyListBuff(b, n->val.block.stmts, nospace);
    depth--;
    printDepth(b); echo(b, "}");
    printLine(b);
}

static void prettyVarBuff(char *b, List *list, char sep){
    if(list){
        prettyVarBuff(b, list->next, sep);
        resetBuff(b);
        Node *n = list->val;
        SP(b, "%s", n->val.param.id);
        if(sep) { printChar(sep); echo(b," "); }
    }
}

static void printVarDecl(char *b, Node *n){
    SP(b,"var ");
    resetBuff(b);
    prettyVarBuff(b, n->val.listS->next, comma); // print the names rather than the 1st one without type info
    resetBuff(b);
    Node *first = n->val.listS->val;
    printVar(b, first);  // print the first one with type info
    echo(b, ";");
    printLine(b);
}

static void prettyNodeBuff(char *b, Node *n){
    if(!n) R ; // NULL, do nothing
    switch(n->kind){
        case       stmtK:
        case     returnK:
        case     importK:
        case         ifK:
        case      whileK:
        case     repeatK:
        case   exprstmtK:
        case      breakK:
        case   continueK:
        case     globalK:
        case    varDeclK:
        case     methodK: printDepth(b); break;
        default: break; // do thing, but skip clang's warning
    }
    resetBuff(b);
    switch(n->kind){
        case           idK: printID              (b,n); break;
        case         nameK: printName            (b,n); break;
        case          varK: printVar             (b,n); break;
        case      varDeclK: printVarDecl         (b,n); break;
        case         typeK: printNodeTypeBuff    (b,n); break;
        case         funcK: printNodeFunc        (b,n); break;
        case      argExprK: printArgExpr         (b,n); break;
        case    paramExprK: printParamExpr       (b,n); break;
        case         stmtK: printStmtBuff        (b,n); break;
        case         castK: printCastBuff        (b,n); break;
        case       returnK: printReturnStmt      (b,n); break;
        case       importK: printImportStmt      (b,n); break;
        case       methodK: printMethodBuff      (b,n); break;
        case       moduleK: printModuleBuff      (b,n); break;
        case           ifK: printIfBuff          (b,n); break;
        case        whileK: printWhileBuff       (b,n); break;
        case       repeatK: printRepeatBuff      (b,n); break;
        case     exprstmtK: printExprStmtBuff    (b,n); break;
        case        breakK: printBreakBuff       (b,n); break;
        case     continueK: printContinueBuff    (b,n); break;
        case       globalK: printGlobalBuff      (b,n); break;
        case         callK: printCallBuff        (b,n); break;
        case       vectorK: printVectorBuff      (b,n); break;
        case        constK: printConstBuff       (b,n); break;
        case        blockK: printBlockBuff       (b,n); break;
        default: EP("Unexpected node type: %d\n", n->kind);
    }
}

char *getNodeTypeStr(Node *n){
    switch(n->kind){
        CaseLine(idK);
        CaseLine(varK);
        CaseLine(varDeclK);
        CaseLine(globalK);
        CaseLine(typeK);
        CaseLine(nameK);
        CaseLine(funcK);
        CaseLine(argExprK);
        CaseLine(paramExprK);
        CaseLine(blockK);
        CaseLine(stmtK);
        CaseLine(castK);
        CaseLine(importK);
        CaseLine(methodK);
        CaseLine(moduleK);
        CaseLine(ifK);
        CaseLine(whileK);
        CaseLine(repeatK);
        CaseLine(returnK);
        CaseLine(breakK);
        CaseLine(continueK);
        CaseLine(callK);
        CaseLine(vectorK);
        CaseLine(constK);
        CaseLine(exprstmtK);
        default: EP("unknown node: %d\n", n->kind);
    }
    R NULL;
}

static void setNewLine(bool x){
    withLine = x;
}

static void printTypeBuff(char *b, Type x){
    resetBuff(b);
    switch(x){
        case   wildT: echo(b, "?");    break;
        case   boolT: echo(b, "bool"); break;
        case    i16T: echo(b, "i16");  break;
        case    i32T: echo(b, "i32");  break;
        case    i64T: echo(b, "i64");  break;
        case    f32T: echo(b, "f32");  break;
        case    f64T: echo(b, "f64");  break;
        case   charT: echo(b, "char"); break;
        case   clexT: echo(b, "complex"); break;
        case    symT: echo(b, "sym");     break;
        case    strT: echo(b, "str");     break;
        case  monthT: echo(b, "m");       break;
        case   dateT: echo(b, "d");       break;
        case  tableT: echo(b, "table");   break;
        case ktableT: echo(b, "ktable");  break;
        case   listT: echo(b, "list");    break;
        case   enumT: echo(b, "enum");    break;
        case   dictT: echo(b, "dict");    break;
        case   funcT: echo(b, "func");    break;
        case  wrongT: echo(b, "WRONG");   break;
        default: EP("Type not supported yet.: %d\n", x);
    }
}

void printType(Type x){
    buff[0]=0;
    printTypeBuff(buff, x);
    P("%s",buff); // type(%s)
}


void printNodeType(Node *n){
    P("[Line %3d]: %s\n",n->lineno,getNodeTypeStr(n));
}

void printNodeLine(Node *n){
    P("[Line %3d]: ",n->lineno); printNode(n);
}

void printNodeStr(Node *n){
    buff[0]=0; setNewLine(false);
    prettyNodeBuff(buff, n);
    P("%s", buff);
}

void printNode(Node *n){
    printNodeStr(n); P("\n");
}

void printProg(Prog *root){
    printBanner("Pretty Printer");
    buff[0]=0; setNewLine(true);
    depth = 0;
    prettyListBuff(buff, root->module_list, nospace);
    P("%s\n", buff);
}

