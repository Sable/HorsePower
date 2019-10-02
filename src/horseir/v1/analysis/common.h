#ifndef __A_COMMON__
#define __A_COMMON__

/* nodes */
//typedef struct ValueNode{
//    pType typ; int len;
//    union {
//        bool   b;
//        int    i64,d;
//        char   c;
//        float  f32;
//        double f64;
//    };
//    char *g;
//}ValueNode;

/* consts */
typedef enum FunctionType {
    /* unary 60 */
    absF, negF, ceilF, floorF, roundF, conjF, recipF, signumF,   piF, notF,
    logF, log2F, log10F, expF, cosF, sinF, tanF, acosF, asinF,
    atanF, coshF, sinhF, tanhF, acoshF, asinhF, atanhF,
    dateF, yearF, monthF, dayF,
    timeF, hourF, minuteF, secondF, millF,
    uniqueF, strF, lenF, rangeF, factF, randF, seedF, flipF, reverseF,
    whereF, groupF, countF, sumF, avgF, minF, maxF, razeF, enlistF, tolistF,
    formatF, keysF, valuesF, metaF, loadTableF, fetchF,
    /* binary 33 */
    ltF,  gtF,  leqF,   geqF,    eqF,  neqF,  plusF,  minusF,  mulF, divF,
    powerF, logbF, modF, andF, orF, nandF, norF, xorF,
    dtdiffF, 
    appendF, likeF, compressF, randkF, indexofF, takeF, dropF, orderF,
    memberF, vectorF, matchF, indexF, columnValueF, subStringF,
    /* special 14 */
    eachF, eachItemF, eachLeftF, eachRightF, enumF, dictF, tableF,
    ktableF, indexAF, listF, outerF, joinIndexF, dtaddF, dtsubF,
    totalFunc
}pFunc;

typedef enum FuncKind {
    elemKind
}FuncKind;

typedef struct FuseNode {
    S targ, invc;
}OptNode, FuseNode, PeepholeNode;

/* signature */
V getLiteralFromNode(Node *n);
int getFuncIndexByName(char *name);

void buildUDChain(Prog *root);
void analyzeSR(Prog *root);
void analyzeFusionElem();
//void analyzeCPF(); // not implemented
void analyzeFusePattern();
void analyzeFusion2();
bool isElementwise(char *funcName);

/* macros: elementwise.c and fusion2.c */
#define chainNode(c)    (c->cur)
#define isSimpleStmt(n) instanceOf(n,simpleStmtK)
#define isCastStmt(n)   instanceOf(n,castStmtK)
#define isAssignment(n) (isSimpleStmt(n)||isCastStmt(n))
#define isReturnStmt(n) instanceOf(n,returnK)
#define setVisited(c,v) c->isVisited=v
#define isVisited(c)    (c->isVisited)
#define indentStr       "    "

#include "udchain.h"
#include "typeshape.h"

/* elementwise */
int  findParamName(char *pName, Chain *chain);
void fuseName(char *buff, int nameID);
int  findNameID(char *name);
void fuseNameClean();
char *fuseNameTarg(char *buff, Chain *chain);
void getNameTypeAlias(char *buff, char *name);

#endif
