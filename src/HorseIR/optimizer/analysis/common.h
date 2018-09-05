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
    /* unary */
    absF, negF, ceilF, floorF, roundF, conjF, recipF, signumF,   piF, notF,
    logF, log2F, log10F, expF, cosF, sinF, tanF, acosF, asinF,
    atanF, coshF, sinhF, tanhF, acoshF, asinhF, atanhF,
    dateF, yearF, monthF, dayF,
    timeF, hourF, minuteF, secondF, millF,
    uniqueF, strF, lenF, rangeF, factF, randF, seedF, flipF, reverseF,
    whereF, groupF, countF, sumF, avgF, minF, maxF, razeF, enlistF, tolistF,
    formatF, keysF, valuesF, metaF, loadTableF, fetchF,
    /* binary */
    ltF,  gtF,  leqF,   geqF,    eqF,  neqF,  plusF,  minusF,  mulF, divF,
    powerF, logbF, modF, andF, orF, nandF, norF, xorF,
    dtdiffF, dtaddF, dtsubF,
    appendF, likeF, compressF, randkF, indexofF, takeF, dropF, orderF,
    memberF, vectorF, matchF, indexF, columnValueF, subStringF,
    /* special */
    eachF, eachItemF, eachLeftF, eachRightF, enumF, dictF, tableF,
    ktableF, indexAF, listF, outerF, joinIndexF,
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

#endif
