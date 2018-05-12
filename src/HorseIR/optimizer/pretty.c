#include "global.h"

int depth;

#define comma ','
#define nospace 0

#define echo(s)       P("%s", s)
#define printChar(c)  P("%c", c)
#define printID(n)    echo(n->val.idS)
#define printFloat(n) P("%g:f64", n->val.floatS)
#define printInt(n)   P("%d:i64", n->val.intS)
#define printComp(n)  P("%s.%s" , n->val.compoundID.name1, n->val.compoundID.name2)
#define printDate(n)  {int x=n->val.dateS; P("%d.%02d.%02d:d",x/10000,x%10000/100,x%100);}
#define printSym(n)   P("`%s",n->val.charS)
#define printFunc(n)  echo(n->val.idS)
#define printLiteralFloat(n) prettyList(n->val.listS, comma)
#define printLiteralSym(n)   {prettyList(n->val.listS, nospace);P(":sym");}
#define printLiteralDate(n)  prettyList(n->val.listS, comma)
#define printLiteralChar(n)  P("'%s'", n->val.charS)
#define printLiteralBool(n)  prettyList(n->val.listS, comma)
#define printLiteralInt(n)   prettyList(n->val.listS, comma)
#define printLiteralParam(n) prettyNode(n->val.nodeS)
#define printParamExpr(n)    prettyList(n->val.listS, comma)
#define printReturnStmt(n)   {P("return "); prettyNode(n->val.nodeS); PN(";");}
#define printImportStmt(n)   {P("import "); prettyNode(n->val.nodeS); PN(";");}

void printDepth(){
    for(int i=0;i<depth;i++){
        P("    ");
    }
}

void printExpr(Node *n){
    if(n->val.expr.func){
        P("@"); prettyNode(n->val.expr.func);
        P("("); prettyNode(n->val.expr.param); P(")");
    }
    else {
        prettyNode(n->val.expr.param);
    }
}

void printSimpleStmt(Node *n){
    prettyNode(n->val.simpleStmt.name);
    P(":"); prettyNode(n->val.simpleStmt.type);
    P("="); prettyNode(n->val.simpleStmt.expr);
    newLine();
}

void printCastStmt(Node *n){
    prettyNode(n->val.castStmt.name);
    P(":"); prettyNode(n->val.castStmt.type);
    P("="); P("check_cast"); P("(");
    prettyNode(n->val.castStmt.expr);
    P(","); prettyNode(n->val.castStmt.cast); P(")");
    newLine();
}

void printMethod(Node *n){
    depth++;
    P("def "); prettyNode(n->val.method.name); P("() : ");
    prettyNode(n->val.method.type);
    PN("{"); prettyList(n->val.method.list, nospace);
    depth--;
    printDepth();
    PN("}");
}

void printModule(Node *n){
    depth++;
    P("module "); prettyNode(n->val.module.name); PN(" {");
    prettyList(n->val.module.body, nospace); PN("}");
    depth--;
}

void prettyNode(Node *n){
    switch(n->kind){
        case simpleStmtK:
        case   castStmtK:
        case     importK:
        case     returnK:
        case     methodK: printDepth(); break;
    }
    switch(n->kind){
        case           idK: printID          (n); break;
        case        floatK: printFloat       (n); break;
        case          intK: printInt         (n); break;
        case         dateK: printDate        (n); break;
        case          symK: printSym         (n); break;
        case         typeK: printType        (n); break;
        case     compoundK: printComp        (n); break;
        case         funcK: printFunc        (n); break;
        case         exprK: printExpr        (n); break;
        case    paramExprK: printParamExpr   (n); break;
        case literalFloatK: printLiteralFloat(n); break;
        case   literalSymK: printLiteralSym  (n); break;
        case  literalDateK: printLiteralDate (n); break;
        case  literalCharK: printLiteralChar (n); break;
        case  literalBoolK: printLiteralBool (n); break;
        case   literalIntK: printLiteralInt  (n); break;
        case literalParamK: printLiteralParam(n); break;
        case   simpleStmtK: printSimpleStmt  (n); break;
        case     castStmtK: printCastStmt    (n); break;
        case       returnK: printReturnStmt  (n); break;
        case       importK: printImportStmt  (n); break;
        case       methodK: printMethod      (n); break;
        case       moduleK: printModule      (n); break;
        default: P("%d\n",n->kind);
                 error("unexpected node type");
    }
}

void prettyList(List *list, char sep){
    if(list){
        List *ptr = list;
        while(ptr){
            if(ptr->val){
                prettyNode(ptr->val);
                if(sep && ptr->next) printChar(sep);
            }
            ptr = ptr->next;
        }
    }
    else { PN("<NULL list found>"); }
}

void printType(Node *n){
    pType k = n->val.typeS;
    switch(k){
        case unknownT: echo("?");    break;
        case    boolT: echo("bool"); break;
        case      i8T: echo("i8");   break;
        case     i16T: echo("i16");  break;
        case     i32T: echo("i32");  break;
        case     i64T: echo("i64");  break;
        case     f32T: echo("f32");  break;
        case     f64T: echo("f64");  break;
        case    charT: echo("char"); break;
        case    clexT: echo("complex"); break;
        case     symT: echo("sym");     break;
        case     strT: echo("str");     break;
        case   monthT: echo("m");       break;
        case    dateT: echo("d");       break;
        case   tableT: echo("table");   break;
        case  ktableT: echo("ktable");  break;
        default: P("Not supported yet.: %d\n", k);
                 error("something wrong");
    }
}


void prettyProg(Prog *root){
    depth = 0;
    prettyList(root->module_list, nospace);
    PN("Done.");
}

