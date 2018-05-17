#ifndef __H_COMMON__
#define __H_COMMON__

/* nodes */
typedef struct ValueNode{
    Kind kind; int len;
    union {
        bool   b;
        int    i64,d;
        char   c;
        float  f32;
        double f64;
    };
    char *g;
}ValueNode;

/* consts */
typedef enum FunctionType {
    /* unary */
    absF, negF, ceilF, floorF, roundF, conjF, recipF, signumF,   piF, notF,
    logF, expF,  cosF,   sinF,   tanF, acosF,  asinF,   atanF, coshF, sinhF,
    tanhF, acoshF, asinhF, atanhF,
    dateF, yearF, monthF, dayF,
    timeF, hourF, minuteF, secondF, millF,
    uniqueF, strF, lenF, rangeF, factF, randF, seedF, flipF, reverseF,
    whereF, groupF, countF, sumF, avgF, minF, maxF, razeF, enlistF, tolistF,
    formatF,
    /* binary */
    ltF,  gtF,  leqF,   geqF,    eqF,  neqF,  plusF,  minusF,  mulF, divF,
    powerF, log2F, modF, andF, orF, nandF, norF, xorF,
    dtdiffF, dtaddF, dtsubF,
    appendF, likeF, compressF, randkF, indexofF, takeF, dropF, orderF,
    memberF, vectorF, matchF,
    /* special */
    eachF, eachItemF, eachLeftF, eachRightF, enumF, dictF, tableF,
    ktableF, keysF, valuesF, metaF, columnValueF, loadTableF, fetchF,
    indexF, indexAF,
    totalFunc
}pFunc;

typedef enum FuncKind {
    elemKind
}FuncKind;

/* signature */

#endif
