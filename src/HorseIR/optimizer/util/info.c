#include "../global.h"

#define CS(x) case x: return #x
#define SZ(x) case x: return sizeof(x)
#define DF(x) default: EP("Type not defined: %d\n",x)

const char *getTypeName(L x){
    switch(x){
        CS(H_B); CS(H_J); CS(H_H); CS(H_I); CS(H_L); CS(H_F); CS(H_E); CS(H_X);
        CS(H_C); CS(H_Q); CS(H_S); CS(H_M); CS(H_D); CS(H_Z); CS(H_U); CS(H_W);
        CS(H_T); CS(H_G); CS(H_N); CS(H_Y); CS(H_A); CS(H_K); CS(H_V); 
        DF(x);
    }
}

const char *getpTypeName(pType x){
    if(x >= totalT){
        EP("pType not defined: %d (total %d)\n", x, totalT);
    }
    switch(x){
        CS(unknownT); CS(boolT); CS(i16T); CS(i32T); CS(i64T);
        CS(charT); CS(clexT); CS(symT); CS(strT);
        CS(monthT); CS(dateT); CS(dtT); CS(hourT); CS(minuteT); CS(timeT);
        CS(tableT); CS(ktableT); CS(listT); CS(enumT); CS(dictT); CS(funcT);
        DF(x);
    } R 0;
}

// TODO 1: change the function to a constant array
// TODO 2: check `getTypeSize` in backend/h_memory.c
const int getpTypeSize(pType x){
    if(x >= totalT){
        EP("pType not defined: %d (total %d)\n", x, totalT);
    }
    switch(x){
        SZ(unknownT); SZ(boolT); SZ(i16T); SZ(i32T); SZ(i64T);
        SZ(charT); SZ(clexT); SZ(symT); SZ(strT);
        SZ(monthT); SZ(dateT); SZ(dtT); SZ(hourT); SZ(minuteT); SZ(timeT);
        SZ(tableT); SZ(ktableT); SZ(listT); SZ(enumT); SZ(dictT); SZ(funcT);
        DF(x);
    } R 0;
}

const char *getKindName(Kind x){
    if(x >= totalK){
        EP("Kind not defined: %d (total %d)\n", x,totalK);
    }
    switch(x){
        CS(idK);  CS(floatK); CS(intK); CS(typeK); CS(compoundK); CS(dateK); CS(symK); CS(strK);
        CS(literalFloatK); CS(literalSymK); CS(literalDateK); CS(literalCharK); CS(literalStrK);
        CS(literalBoolK); CS(literalParamK); CS(literalIntK); CS(literalFuncK);
        CS(funcK); CS(exprK); CS(paramExprK); CS(nameTypeK);
        CS(simpleStmtK); CS(castStmtK); CS(returnK); CS(importK); CS(methodK); CS(moduleK);
        DF(x);
    } R 0;
}

const char *getpFuncName(pFunc x){
    if(x >= totalFunc){
        EP("pFunc not defined: %d (total %d)\n", x,totalFunc);
    }
    switch(x){
        /* unary */
        CS(absF); CS(negF); CS(ceilF); CS(floorF); CS(roundF); CS(conjF); CS(recipF);
        CS(signumF); CS(piF); CS(notF); CS(logF); CS(log2F); CS(log10F); CS(expF);
        CS(cosF); CS(sinF); CS(tanF); CS(acosF); CS(asinF); CS(atanF); CS(coshF);
        CS(sinhF); CS(tanhF); CS(acoshF); CS(asinhF); CS(atanhF);
        CS(dateF); CS(yearF); CS(monthF); CS(dayF);
        CS(timeF); CS(hourF); CS(minuteF); CS(secondF); CS(millF);
        CS(uniqueF); CS(strF); CS(lenF); CS(rangeF); CS(factF); CS(randF); CS(seedF);
        CS(flipF); CS(reverseF); CS(whereF); CS(groupF); CS(countF); CS(sumF); CS(avgF);
        CS(minF); CS(maxF); CS(razeF); CS(enlistF); CS(tolistF); CS(formatF); CS(keysF); CS(valuesF);
        CS(metaF); CS(loadTableF); CS(fetchF);
        /* binary */
        CS(ltF); CS(gtF); CS(leqF); CS(geqF); CS(eqF); CS(neqF); CS(plusF); CS(minusF); CS(mulF); CS(divF);
        CS(powerF); CS(logbF); CS(modF); CS(andF); CS(orF); CS(nandF); CS(norF); CS(xorF);
        CS(dtdiffF); CS(dtaddF); CS(dtsubF);
        CS(appendF); CS(likeF); CS(compressF); CS(randkF); CS(indexofF); CS(takeF); CS(dropF); CS(orderF);
        CS(memberF); CS(vectorF); CS(matchF); CS(indexF); CS(columnValueF); CS(subStringF);
        /* special */
        CS(eachF); CS(eachItemF); CS(eachLeftF); CS(eachRightF); CS(enumF); CS(dictF); CS(tableF);
        CS(ktableF); CS(indexAF); CS(listF); CS(outerF); CS(joinIndexF);
    } R 0;
}


