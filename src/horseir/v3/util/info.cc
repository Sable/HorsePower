#include "../global.h"

#define CASE_TYPE(t,x) case H_##t: return x
//#define SZ(x) CASE(x, sizeof(x))

const char *getTypeName(I x){
    switch(x){
        CASE_STR(H_B); CASE_STR(H_J); CASE_STR(H_H); CASE_STR(H_I);
        CASE_STR(H_L); CASE_STR(H_F); CASE_STR(H_E); CASE_STR(H_X);
        CASE_STR(H_C); CASE_STR(H_Q); CASE_STR(H_S); CASE_STR(H_M);
        CASE_STR(H_D); CASE_STR(H_Z); CASE_STR(H_U); CASE_STR(H_W);
        CASE_STR(H_T); CASE_STR(H_G); CASE_STR(H_N); CASE_STR(H_Y);
        CASE_STR(H_A); CASE_STR(H_K); CASE_STR(H_V); 
        default: EP("Type not defined: %d",(I)x);
    }
}

HorseType getTypeFromV(V x){
    switch(xp){
        CASE_TYPE(B,   boolT); CASE_TYPE(J,    i8T); CASE_TYPE(H,    i16T);
        CASE_TYPE(I,    i32T); CASE_TYPE(L,   i64T); CASE_TYPE(F,    f32T);
        CASE_TYPE(E,    f64T); CASE_TYPE(X,  clexT); CASE_TYPE(C,   charT);
        CASE_TYPE(Q,    symT); CASE_TYPE(S,   strT); CASE_TYPE(D,   dateT);
        CASE_TYPE(T,   timeT); CASE_TYPE(Z,    dtT); CASE_TYPE(U, minuteT);
        CASE_TYPE(W, secondT); CASE_TYPE(G,  listT); CASE_TYPE(N,   dictT);
        CASE_TYPE(Y,   enumT); CASE_TYPE(A, tableT); CASE_TYPE(K, ktableT);
        default: EP("Type not defined: %d",(I)xp);
    }
}

const char *getExtraKind(GenKind x){
    switch(x){
        CASE(NativeG, "Native");
        CASE(  SkipG, "Skip");
        CASE(   OptG, "Opt");
        default: EP("Unknown kind: %d", (I)x);
    }
}

HorseType getType(Node *x){
    if(!x)
        EP("Empty type node found");
    if(x->val.type.isWild)
        R wildT;
    //else if(x->val.type.cell) {
    //    TODO("Cell types not allowed.\n");
    //}
    else {
        char *typ = x->val.type.typ;
        if(sEQ(typ, "bool")) R boolT;
        else if(sEQ(typ, "i8"     )) R i8T;
        else if(sEQ(typ, "i16"    )) R i16T;
        else if(sEQ(typ, "i32"    )) R i32T;
        else if(sEQ(typ, "i64"    )) R i64T;
        else if(sEQ(typ, "f32"    )) R f32T;
        else if(sEQ(typ, "f64"    )) R f64T;
        else if(sEQ(typ, "complex")) R clexT;
        else if(sEQ(typ, "char"   )) R charT;
        else if(sEQ(typ, "sym"    )) R symT;
        else if(sEQ(typ, "str"    )) R strT;
        else if(sEQ(typ, "date"   )) R dateT;
        else if(sEQ(typ, "month"  )) R monthT;
        else if(sEQ(typ, "dt"     )) R dtT;
        else if(sEQ(typ, "minute" )) R minuteT;
        else if(sEQ(typ, "second" )) R secondT;
        else if(sEQ(typ, "time"   )) R timeT;
        else if(sEQ(typ, "list"   )) R listT;
        else if(sEQ(typ, "dict"   )) R dictT;
        else if(sEQ(typ, "enum"   )) R enumT;
        else if(sEQ(typ, "table"  )) R tableT;
        else if(sEQ(typ, "ktable" )) R ktableT;
        else if(sEQ(typ, "func"   )) R funcT;
        else EP("Invalid type: %s", typ);
    }
    R wildT; // unreachable code
}

//const char *getpTypeName(pType x){
//    if(x >= totalT){
//        EP("pType not defined: %d (total %d)\n", x, totalT);
//    }
//    switch(x){
//        CS(unknownT); CS(boolT); CS(i16T); CS(i32T); CS(i64T);
//        CS(charT); CS(clexT); CS(symT); CS(strT);
//        CS(monthT); CS(dateT); CS(dtT); CS(hourT); CS(minuteT); CS(timeT);
//        CS(tableT); CS(ktableT); CS(listT); CS(enumT); CS(dictT); CS(funcT);
//        DF(x);
//    } R 0;
//}

// TODO 1: change the function to a constant array
// TODO 2: check `getTypeSize` in backend/h_memory.c
// #define SZ(x) case x: return sizeof(x)
//int getpTypeSize(pType x){
//    if(x >= totalT){
//        EP("pType not defined: %d (total %d)\n", x, totalT);
//    }
//    switch(x){
//        SZ(unknownT); SZ(boolT); SZ(i16T); SZ(i32T); SZ(i64T);
//        SZ(charT); SZ(clexT); SZ(symT); SZ(strT);
//        SZ(monthT); SZ(dateT); SZ(dtT); SZ(hourT); SZ(minuteT); SZ(timeT);
//        SZ(tableT); SZ(ktableT); SZ(listT); SZ(enumT); SZ(dictT); SZ(funcT);
//        DF(x);
//    } R 0;
//}

const char *getKindName(Kind x){
    if(x >= totalK){
        EP("Kind not defined: %d (total %d)", x,totalK);
    }
    // TODO: Fix
//    switch(x){
//        CS(idK);  CS(floatK); CS(intK); CS(typeK); CS(compoundK); CS(dateK); CS(symK); CS(strK);
//        CS(literalFloatK); CS(literalSymK); CS(literalDateK); CS(literalCharK); CS(literalStrK);
//        CS(literalBoolK); CS(literalParamK); CS(literalIntK); CS(literalFuncK);
//        CS(funcK); CS(exprK); CS(paramExprK); CS(nameTypeK);
//        CS(simpleStmtK); CS(castStmtK); CS(returnK); CS(importK); CS(methodK); CS(moduleK);
//        DF(x);
//    }
      R 0;
}

//const char *getpFuncName(pFunc x){
//    if(x >= totalFunc){
//        EP("pFunc not defined: %d (total %d)\n", x,totalFunc);
//    }
//    switch(x){
//        /* unary */
//        CS(absF); CS(negF); CS(ceilF); CS(floorF); CS(roundF); CS(conjF); CS(recipF);
//        CS(signumF); CS(piF); CS(notF); CS(logF); CS(log2F); CS(log10F); CS(expF);
//        CS(cosF); CS(sinF); CS(tanF); CS(acosF); CS(asinF); CS(atanF); CS(coshF);
//        CS(sinhF); CS(tanhF); CS(acoshF); CS(asinhF); CS(atanhF);
//        CS(dateF); CS(yearF); CS(monthF); CS(dayF);
//        CS(timeF); CS(hourF); CS(minuteF); CS(secondF); CS(millF);
//        CS(uniqueF); CS(strF); CS(lenF); CS(rangeF); CS(factF); CS(randF); CS(seedF);
//        CS(flipF); CS(reverseF); CS(whereF); CS(groupF); CS(sumF); CS(avgF);
//        CS(minF); CS(maxF); CS(razeF); CS(enlistF); CS(tolistF); CS(formatF); CS(keysF); CS(valuesF);
//        CS(metaF); CS(loadTableF); CS(fetchF);
//        /* binary */
//        CS(ltF); CS(gtF); CS(leqF); CS(geqF); CS(eqF); CS(neqF); CS(plusF); CS(minusF); CS(mulF); CS(divF);
//        CS(powerF); CS(logbF); CS(modF); CS(andF); CS(orF); CS(nandF); CS(norF); CS(xorF);
//        CS(dtdiffF);
//        CS(appendF); CS(likeF); CS(compressF); CS(randkF); CS(indexofF); CS(takeF); CS(dropF); CS(orderF);
//        CS(memberF); CS(vectorF); CS(matchF); CS(indexF); CS(columnValueF); CS(subStringF);
//        /* special */
//        CS(eachF); CS(eachItemF); CS(eachLeftF); CS(eachRightF); CS(enumF); CS(dictF); CS(tableF);
//        CS(ktableF); CS(indexAF); CS(listF); CS(outerF); CS(joinIndexF); CS(dtaddF); CS(dtsubF);
//    } R 0;
//}

void getInfoVar2(V x, CS name){
    WP("Variable %s has type %s and len %lld\n", name, getTypeName(xp),xn);
    if(xp == H_G){
        L v_min = 9999999, v_max = -1;
        DOI(xn, {V t=vV(x,i); \
                if(vn(t)<v_min)v_min=vn(t); \
                if(vn(t)>v_max)v_max=vn(t);})
        if(xn < 10){
            WP(" "); DOI(xn, WP(" %s", getTypeName(vp(vV(x,i))))) P("\n");
        }
        WP("  total = %lld, max = %lld, min = %lld\n", xn, v_max, v_min); //getchar();
    }
}

HA newHorseArray(){
    HA x = NEW(HA0);
    x->size=0;
    x->next=NULL;
    R x;
}

HA appendToHorseArray(HA x, L v){
    if(x->size < HORSE_ARRAY_SIZE){
        x->data[(x->size)++] = v;
        R x;
    }
    else {
        x->next = newHorseArray();
        R appendToHorseArray(x->next,v);
    }
}

L countHorseArray(HA x){
    R x?(x->size + countHorseArray(x->next)):0;
}

void copyFromHorseArray(G g, HA x){
    if(x){
        L n = sizeof(L)*(x->size);
        memcpy(g, x->data, n);
        copyFromHorseArray(g+n, x->next);
    }
}

static void indexWithHorseArraySub(V z, V y, HA x, L k){
    if(x){
        DOI(x->size, {
            switch(vp(y)){
                caseI vI(z,k+i)=vI(y,x->data[i]); break;
                caseL vL(z,k+i)=vL(y,x->data[i]); break;
                caseF vF(z,k+i)=vF(y,x->data[i]); break;
                caseE vE(z,k+i)=vE(y,x->data[i]); break;
                caseQ vQ(z,k+i)=vQ(y,x->data[i]); break;
                default: EP("Type not supported: %s",getTypeName(vp(y)));
            }})
        indexWithHorseArraySub(z,y,x->next,k+(x->size));
    }
}

void indexWithHorseArray(V z, V y, HA x){
    indexWithHorseArraySub(z,y,x,0);
}

void freeHorseArray(HA x){
    if(x){
        freeHorseArray(x->next);
        free(x);
    }
}

