#include "../global.h"

#define CS(x) case x: return #x
#define SZ(x) case H_##x: return sizeof(x)
#define DF(x) default: EP("Type not defined: %d\n",(I)x)
#define CS2(x, y) case H_##x: return y

const char *getTypeName(I x){
    switch(x){
        CS(H_B); CS(H_J); CS(H_H); CS(H_I); CS(H_L); CS(H_F); CS(H_E); CS(H_X);
        CS(H_C); CS(H_Q); CS(H_S); CS(H_M); CS(H_D); CS(H_Z); CS(H_U); CS(H_W);
        CS(H_T); CS(H_G); CS(H_N); CS(H_Y); CS(H_A); CS(H_K); CS(H_V); 
        DF(x);
    }
}

Type getTypeFromV(V x){
    switch(xp){
        CS2(B, boolT); CS2(J,  i8T); CS2(H,  i16T); CS2(I,  i32T); CS2(L, i64T);
        CS2(F,  f32T); CS2(E, f64T); CS2(X, clexT); CS2(C, charT); CS2(Q, symT);
        CS2(S,  strT); CS2(D,dateT); CS2(T, timeT); CS2(Z,   dtT);
        CS2(U, minuteT); CS2(W, secondT);
        CS2(G, listT); CS2(N,dictT); CS2(Y, enumT); CS2(A, tableT); CS2(K, ktableT);
        DF(xp);
    }
}

const char *getExtraKind(GenKind x){
    switch(x){
        case NativeG: return "Native";
        case   SkipG: return "Skip";
        case    OptG: return "Opt";
        default: EP("Unknown kind: %d\n", x);
    }
}

Type getType(Node *x){
    if(!x) EP("Empty type node found\n");
    if(x->val.type.isWild) R wildT;
    //else if(x->val.type.cell) {
    //    TODO("Cell types not allowed.\n");
    //}
    else {
        char *typ = x->val.type.typ;
        if(!strcmp(typ, "bool")) R boolT;
        else if(!strcmp(typ, "i8"     )) R i8T;
        else if(!strcmp(typ, "i16"    )) R i16T;
        else if(!strcmp(typ, "i32"    )) R i32T;
        else if(!strcmp(typ, "i64"    )) R i64T;
        else if(!strcmp(typ, "f32"    )) R f32T;
        else if(!strcmp(typ, "f64"    )) R f64T;
        else if(!strcmp(typ, "complex")) R clexT;
        else if(!strcmp(typ, "char"   )) R charT;
        else if(!strcmp(typ, "sym"    )) R symT;
        else if(!strcmp(typ, "str"    )) R strT;
        else if(!strcmp(typ, "date"   )) R dateT;
        else if(!strcmp(typ, "month"  )) R monthT;
        else if(!strcmp(typ, "dt"     )) R dtT;
        else if(!strcmp(typ, "minute" )) R minuteT;
        else if(!strcmp(typ, "second" )) R secondT;
        else if(!strcmp(typ, "time"   )) R timeT;
        else if(!strcmp(typ, "list"   )) R listT;
        else if(!strcmp(typ, "dict"   )) R dictT;
        else if(!strcmp(typ, "enum"   )) R enumT;
        else if(!strcmp(typ, "table"  )) R tableT;
        else if(!strcmp(typ, "ktable" )) R ktableT;
        else if(!strcmp(typ, "func"   )) R funcT;
        else EP("Invalid type: %s\n", typ);
    }
    R 0;
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
        EP("Kind not defined: %d (total %d)\n", x,totalK);
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

void getInfoVar2(V x, S name){
    P("Variable %s has type %s and len %lld\n", name, getTypeName(xp),xn);
    if(xp == H_G){
        L v_min = 9999999, v_max = -1;
        DOI(xn, {V t=vV(x,i); \
                if(vn(t)<v_min)v_min=vn(t); \
                if(vn(t)>v_max)v_max=vn(t);})
        if(xn < 10){
            P(" "); DOI(xn, P(" %s", getTypeName(vp(vV(x,i))))) P("\n");
        }
        P("  total = %lld, max = %lld, min = %lld\n", xn, v_max, v_min); //getchar();
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
                default: EP("type not supported: %s\n",getTypeName(vp(y)));
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

