#include "../global.h"

static const char *monFnName[] = {
    "pfnAbs", "pfnNeg", "pfnCeil", "pfnFloor", "pfnRound", "pfnConj",
    "pfnRecip", "pfnSignum", "pfnPi"  , "pfnNot" , "pfnLog", "pfnLog2",
    "pfnLog10", "pfnExp", "pfnTrigCos", "pfnTrigSin", "pfnTrigTan",
    "pfnTrigAcos", "pfnTrigAsin", "pfnTrigAtan", "pfnHyperCosh",
    "pfnHyperSinh", "pfnHyperTanh", "pfnHyperAcosh", "pfnHyperAsinh",
    "pfnHyperAtanh", "pfnDate", "pfnDateYear", "pfnDateMonth", "pfnDateDay",
    "pfnTime", "pfnTimeHour", "pfnTimeMinute", "pfnTimeSecond", "pfnTimeMill",
    "pfnUnique", "pfnStr", "pfnLen", "pfnRange", "pfnFact", "pfnRand",
    "pfnSeed", "pfnFlip", "pfnReverse", "pfnWhere", "pfnGroup", "pfnSum",
    "pfnAvg", "pfnMin", "pfnMax", "pfnRaze", "pfnToList", "pfnKeys",
    "pfnValues", "pfnMeta", "pfnLoadTable", "pfnFetch", "pfnPrint",
    "pfnCumsum"
};

static const char *dyaFnName[] = {
    "pfnLt", "pfnGt", "pfnLeq", "pfnGeq", "pfnEq", "pfnNeq", "pfnPlus",
    "pfnMinus", "pfnMul", "pfnDiv", "pfnPower", "pfnLog2", "pfnMod", "pfnAnd",
    "pfnOr", "pfnNand", "pfnNor", "pfnXor", "pfnAppend", "pfnLike2",
    "pfnCompress", "pfnRandK", "pfnIndexOf", "pfnTake", "pfnDrop",
    "pfnOrderBy", "pfnMember", "pfnVector", "pfnMatch", "pfnIndex",
    "pfnColumnValue", "pfnSubString", "pfnRange2", "pfnIndex2"
};

static const char *otherFnName[] = {
    "pfnEach", "pfnEachItem", "pfnEachLeft", "pfnEachRight", "pfnEnum",
    "pfnDict", "pfnTable", "pfnKTable", "pfnIndexA", "pfnList", "pfnJoinIndex",
    "pfnDatetimeAdd", "pfnDatetimeSub"
};

I getHType(HorseType t){
    switch(t){
        case   boolT: R H_B;
        case     i8T: R H_J;
        case    i16T: R H_H;
        case    i32T: R H_I;
        case    i64T: R H_L;
        case    f32T: R H_F;
        case    f64T: R H_E;
        case   clexT: R H_X;
        case   charT: R H_C;
        case    symT: R H_Q;
        case    strT: R H_S;
        case   dateT: R H_D;
        case  monthT: R H_M;
        case     dtT: R H_Z;
        case   timeT: R H_T;
        case minuteT: R H_U;
        case secondT: R H_W;
        default: EP("Type not supported: %d", t);
    }
}

static O loadInt(I v, V x, L k, I t){
    switch(t){
        caseB xB(k) = v!=0; break;
        caseJ xJ(k) = (J)v; break;
        caseH xH(k) = (H)v; break;
        caseI xI(k) = (I)v; break;
        caseF xF(k) = (F)v; break;
        caseE xE(k) = (E)v; break;
        caseD xD(k) =    v; break;
        caseM xM(k) =    v; break;
        caseT xT(k) =    v; break;
        caseU xU(k) =    v; break;
        caseW xW(k) =    v; break;
        default: EP("Type not supported: %d", t);
    }
}

static O loadFloat(E v, V x, L k, I t){
    switch(t){
        caseF xF(k) = (F)v; break;
        caseE xE(k) = (E)v; break;
        default: EP("Type not supported: %d", t);
    }
}

static O loadLong(L v, V x, L k, I t){
    switch(t){
        caseB xB(k) = v!=0; break;
        caseJ xJ(k) = (J)v; break;
        caseH xH(k) = (H)v; break;
        caseI xI(k) = (I)v; break;
        caseL xL(k) =    v; break;
        caseF xF(k) = (F)v; break;
        caseE xE(k) = (E)v; break;
        caseZ xZ(k) =    v; break;
        default: EP("Type not supported: %d", t);
    }
}

static O loadClex(F *v, V x, L k, I t){
    switch(t){
        caseX xReal(xX(k)) = v[0];
              xImag(xX(k)) = v[1]; break;
        default: EP("Type not supported: %d", t);
    }
}

static O loadString(S v, V x, L k, I t){
    switch(t){
        caseQ xQ(k) = getSymbol(v); break;
        caseS {S s=allocStrMem(strlen(v)); strcpy(s,v); xS(k)=s;} break;
        caseC xC(k) = v[0]; break;
        default: EP("Type not supported: %d", t);
    }
}

// similar to b/h_io.c:loadItem
O loadConst(Node *n, V x, L k, I t){
    ConstValue *p = n->val.nodeC;
    switch(p->type){
        case    intC: loadInt   (p->valI,x,k,t); break;
        case  floatC: loadFloat (p->valF,x,k,t); break;
        case   longC: loadLong  (p->valL,x,k,t); break;
        case   clexC: loadClex  (p->valX,x,k,t); break;
        case   charC:
        case    symC: 
        case    strC: loadString(p->valS,x,k,t); break;
        case  monthC: 
        case minuteC: 
        case secondC: 
        case   timeC: 
        case   dateC: loadInt   (p->valI,x,k,t); break;
        case     dtC: loadLong  (p->valF,x,k,t); break;
        default: EP("Constant type not supported: %d\n", p->type);
    }
}

const char *obtainBuiltinName(S func){
    FuncUnit x;
    getFuncIndexByName(func, &x);
    switch(x.kind){
        case 1: R   monFnName[x.u];
        case 2: R   dyaFnName[x.b];
        case 3: R otherFnName[x.t];
        default: EP("Kind not found: %d\n", x.kind);
    }
}


static const char *selectFnName[] = {
    "pfnPower"
};

static const char *gpuFnName[] = {
    "gpuPower"
};

const char *obtainGPUBuiltinName(S func){
    // WP("func = %s\n", func);
    const char * funcStr = obtainBuiltinName(func);
    L size = sizeof(selectFnName) / sizeof(const char *);
    // WP("size = %lld\n", size); getchar();
    DOI(size, if(sEQ(funcStr, selectFnName[i]))R gpuFnName[i])
    R funcStr;
}



