/*
 * system functions
 */

#include "../global.h"

const L NUM_LIST_TABLE = 100;
E H_EPSILON = 1E-13;

ListT listTable;
L listTableCur;

L H_CORE = 1;

void initSys(){
    listTable = (ListT)malloc(sizeof(ListT0) * NUM_LIST_TABLE);
    listTableCur = 0;
    getNumberOfCore();
}

void getNumberOfCore(){
    #pragma omp parallel
    {
        L tid = omp_get_thread_num();
        if(0==tid)
            H_CORE = omp_get_num_threads();
    }
    P("# of cores: %lld\n", H_CORE);
}

void deleteSys(){
    listTableCur = 0;
    free(listTable);
}

L registerTable(S tableName, V tablePtr){
    listTable[listTableCur].sid   = getSymbol(tableName);
    listTable[listTableCur].table = tablePtr;
    R listTableCur++;
}

V findTableByName(L sid){
    DOI(listTableCur, if(listTable[i].sid == sid){R listTable[i].table;})
    R NULL;
}

/* copy x to z */
L copyV(V z, V x){
    *z = *x;
    R 0;
}

L initListCopy(V z, V x, L len){
    if(isList(x)){
        initV(z, H_G, vn(x));
        DOI(vn(x), initV(vV(z,i), vp(vV(x,i)), len))
    }
    else if(isTypeGroupBasic(vp(x))){
        initV(z, vp(x), len);
    }
    else R E_DOMAIN;
    R 0;
}

L copyByIndex(V z, L p, V x, L q){
    if(isList(x)){
        DOI(vn(x), copyByIndexSimple(vV(z,i), p, vV(x,i), q));
    }
    else if(isTypeGroupBasic(vp(x))){
        CHECKE(copyByIndexSimple(z,p,x,q));
    }
    else R E_DOMAIN;
    R 0;
}

L copyByIndexSimple(V z, L p, V x, L q){
    if(isList(x)){
        CHECKE(copyV(vV(z,p),vV(x,q)))
    }
    else if(isTypeGroupBasic(vp(x))) {
        switch(vp(x)){
            caseB vB(z,p) = vB(x,q); break;
            caseH vH(z,p) = vH(x,q); break;
            caseI vI(z,p) = vI(x,q); break;
            caseL vL(z,p) = vL(x,q); break;
            caseQ vQ(z,p) = vQ(x,q); break;
            caseC vC(z,p) = vC(x,q); break;
            caseS vS(z,p) = vS(x,q); break;
        }
    }
    else R E_DOMAIN;
    R 0;
}
// L copyV(V z, V x){
//     initV(z,xp,xn);
//     if(isTypeGroupNumber(xp)){
//         memcpy(vg(z),vg(x),getTypeSize(xp)*xn);
//     }
//     else {
//         switch(xp){
//             caseG DOI(xn, CHECKE(copyV(vV(z,i),vV(x,i)))) break;
//         }
//     }
//     R 0;
// }

/*
 * Include: table and keyed table
 * Status: 0 or >0
 * Return: *z
 */
L findColFromTable2(V *z, V x, L cId){
    if(isTable(x)) {
        L t = findColFromTable(x,cId);
        if(t>=0) *z = getTableCol(x,t);
        else R E_DOMAIN;
    }
    else if(isKTable(x)) {
        V key = getKTableKey(x);
        V val = getKTableVal(x);
        L t = findColFromTable(key,cId);
        if(t>=0) *z = getTableCol(key,t);
        else{
            t = findColFromTable(val,cId);
            if(t>=0) *z = getTableCol(val,t);
            else R E_DOMAIN;
        }
    }
    else R E_DOMAIN;
    R 0;
}

L findColFromTable(V x, L cId){
    V key = getTableKeys(x);
    DOI(tableCol(x), if(cId == vQ(key,i))R i)
    R -1;
}

L promoteValue(V z, V x, L typMax){
    if(vp(x) == typMax) R copyV(z,x);
    else{
        initV(z, typMax, xn); //opt?
        switch(vp(z)){
            caseH {
                switch(vp(x)){
                    caseB DOP(xn, vH(z,i)=vB(x,i)) break;
                }
            } break;
            caseI {
                switch(vp(x)){
                    caseB DOP(xn, vI(z,i)=vB(x,i)) break;
                    caseH DOP(xn, vI(z,i)=vH(x,i)) break;
                }
            } break;
            caseL {
                switch(vp(x)){
                    caseB DOP(xn, vL(z,i)=vB(x,i)) break;
                    caseH DOP(xn, vL(z,i)=vH(x,i)) break;
                    caseI DOP(xn, vL(z,i)=vI(x,i)) break;
                }
            } break;
            caseF {
                switch(vp(x)){
                    caseB DOP(xn, vF(z,i)=(F)vB(x,i)) break;
                    caseH DOP(xn, vF(z,i)=(F)vH(x,i)) break;
                    caseI DOP(xn, vF(z,i)=(F)vI(x,i)) break;
                    caseL DOP(xn, vF(z,i)=(F)vL(x,i)) break;
                }
            } break;
            caseE {
                switch(vp(x)){
                    caseB DOP(xn, vE(z,i)=(E)vB(x,i)) break;
                    caseH DOP(xn, vE(z,i)=(E)vH(x,i)) break;
                    caseI DOP(xn, vE(z,i)=(E)vI(x,i)) break;
                    caseL DOP(xn, vE(z,i)=(E)vL(x,i)) break;
                    caseF DOP(xn, vE(z,i)=(E)vF(x,i)) break;
                }
            } break;
            caseX {
                switch(vp(x)){
                    caseB DOP(xn, xCopy(vX(z,i),vB(x,i),0)) break;
                    caseH DOP(xn, xCopy(vX(z,i),vH(x,i),0)) break;
                    caseI DOP(xn, xCopy(vX(z,i),vI(x,i),0)) break;
                    caseL DOP(xn, xCopy(vX(z,i),vL(x,i),0)) break;
                    caseF DOP(xn, xCopy(vX(z,i),vF(x,i),0)) break;
                    caseE DOP(xn, xCopy(vX(z,i),vE(x,i),0)) break;
                }
            } break;
            default: P("Error: PromoteValue error\n"); R E_DOMAIN;
        }
        R 0;
    }
}

B checkZero(V x){
    #define FOUND 1
    #define NOTFOUND 0
    switch(xp){
        caseB DOI(xn, if(0==xB(i)) R FOUND); break;
        caseH DOI(xn, if(0==xH(i)) R FOUND); break;
        caseI DOI(xn, if(0==xI(i)) R FOUND); break;
        caseL DOI(xn, if(0==xL(i)) R FOUND); break;
        caseF DOI(xn, if(0==xF(i)) R FOUND); break;
        caseE DOI(xn, if(0==xE(i)) R FOUND); break;
    }
    R NOTFOUND;
}

B checkMatch(V x){
    switch(xp){
        caseG if(xn>0){L n=vn(vV(x,0)); DOI(xn,if(n!=vn(vV(x,i)))R 0)} break; /* List */
    }
    R 1;
}

L getSingleIntValue(V x){
    R (1==xn)?(H_B==xp?xb:H_H==xp?xh:H_I==xp?xi:H_L==xp?xl:-1):-1;
}

L calcFact(L n){
    if(n>=0){
        L z = 1; DOI(n, z*=(i+1)) R z;
    }
    else R -1;
}

L getNumOfNonZero(V x, L *z){
    if(H_B==xp){
        DOT(xn, if(vB(x,i))z[tid]++)
        R 0;
    }
    else R E_DOMAIN;
}

L appendList(V z, V x, V y){R E_NOT_IMPL;}
/* x: enum, y: number */
L appendEnum(V z, V x, V y){R E_NOT_IMPL;}
/*
 * % -> .* // zero or more
 * _ -> .  // single
 */
S genLikeString(S inStr, L inLen){
    S outStr=NULL;
    L outLen= inLen;
    DOI(inLen, if(inStr[i]=='%' || inStr[i]=='.' || inStr[i]=='\\')outLen++)
    if(inLen>0){
        L c= 0; outLen+= 2;
        outStr= (S)malloc(sizeof(C)*outLen);
        outStr[c++]= '^';  /* begin */
        DOI(inLen, \
            {if(inStr[i]=='%'){outStr[c++]='.';outStr[c++]='*';}\
            else if(inStr[i]=='.' || inStr[i]=='\\'){outStr[c++]='\\';outStr[c++]=inStr[i];}\
            else outStr[c++]=inStr[i]=='_'?'.':inStr[i];})
        outStr[c++]='$'; outStr[c]=0; /* end */
    }
    R outStr;
}

V getValueFromSymbol(Q id){
    R NULL;
}

L checkLength(V x){
    L n0 = vn(x)>0?vn(vV(x,0)):0;
    DOI(vn(x), if(vn(vV(x,i))!=n0)R E_LENGTH)
    R 0;
}


/* error code -2 */
L decideType(L x, L y){
    if(isTypeGroupNumber(x) && isTypeGroupNumber(y)){
        R MAX(x,y);
    }
    else if(x == y){
        R x;
    }
    else R -2;
}

/* typ init. -1 */
L getCommonType(V x, L *typ, L *len){
    if(isList(x)){
        DOI(xn, CHECKE(getCommonType(vV(x,i),typ,len)))
    }
    else {
        *typ = (-1==*typ)?xp:decideType(*typ,xp); CHECKE(-2==(*typ)?E_RAZE_LIST:0);
        *len+= xn;
    }
    R 0;
}


L fillRaze(V z, L *n0, V x){
    if(isList(x)){
        DOI(xn, CHECKE(fillRaze(z,n0,vV(x,i))))
    }
    else {
        L n = *n0;
        switch(vp(z)){
             caseB DOI(xn, vB(z,n++)=vB(x,i)); break;
             caseH { switch(xp){
                     caseB DOI(xn, vH(z,n++)=vB(x,i)); break;
                     caseH DOI(xn, vH(z,n++)=vH(x,i)); break; } } break;
             caseI { switch(xp){
                     caseB DOI(xn, vI(z,n++)=vB(x,i)); break;
                     caseH DOI(xn, vI(z,n++)=vH(x,i)); break;
                     caseI DOI(xn, vI(z,n++)=vI(x,i)); break; } } break;
             caseL { switch(xp){
                     caseB DOI(xn, vL(z,n++)=vB(x,i)); break;
                     caseH DOI(xn, vL(z,n++)=vH(x,i)); break;
                     caseI DOI(xn, vL(z,n++)=vI(x,i)); break;
                     caseL DOI(xn, vL(z,n++)=vL(x,i)); break; } } break;
             caseF { switch(xp){
                     caseB DOI(xn, vF(z,n++)=vB(x,i)); break;
                     caseH DOI(xn, vF(z,n++)=vH(x,i)); break;
                     caseI DOI(xn, vF(z,n++)=vI(x,i)); break;
                     caseL DOI(xn, vF(z,n++)=vL(x,i)); break;
                     caseF DOI(xn, vF(z,n++)=vF(x,i)); break; } } break;
             caseE { switch(xp){
                     caseB DOI(xn, vE(z,n++)=vB(x,i)); break;
                     caseH DOI(xn, vE(z,n++)=vH(x,i)); break;
                     caseI DOI(xn, vE(z,n++)=vI(x,i)); break;
                     caseL DOI(xn, vE(z,n++)=vL(x,i)); break;
                     caseF DOI(xn, vE(z,n++)=vF(x,i)); break;
                     caseE DOI(xn, vE(z,n++)=vE(x,i)); break; } } break;
             caseX DOI(xn, vX(z,n++)=vX(x,i)); break;
             caseQ DOI(xn, vQ(z,n++)=vQ(x,i)); break;
             caseS DOI(xn, vS(z,n++)=vS(x,i)); break;
             caseC DOI(xn, vC(z,n++)=vC(x,i)); break;
             default: R E_NOT_IMPL;
        }
        *n0 = n;
    }
    R 0;
}


L matchPair(B *t, V x, V y){
    *t=0;
    if(isSameType(x,y) && isEqualLength(x,y)){
        if(isTypeGroupBasic(vp(x))){
            switch(vp(x)){
                caseB DOI(vn(x), if(vB(x,i)!=vB(y,i))R 0) break;
                caseH DOI(vn(x), if(vH(x,i)!=vH(y,i))R 0) break;
                caseI DOI(vn(x), if(vI(x,i)!=vI(y,i))R 0) break;
                caseL DOI(vn(x), if(vL(x,i)!=vL(y,i))R 0) break;
                caseF DOI(vn(x), if(vF(x,i)!=vF(y,i))R 0) break;
                caseE DOI(vn(x), if(vE(x,i)!=vE(y,i))R 0) break;
                caseX DOI(vn(x), if(!xEqual(vX(x,i),vX(y,i)))R 0)   break;
                caseQ DOI(vn(x), if(vQ(x,i)!=vQ(y,i))R 0)           break;
                caseS DOI(vn(x), if(0!=strcmp(vS(x,i),vS(y,i)))R 0) break;
                default: R E_NOT_IMPL;
            }
            *t=1; R 0;
        }
        else if(isList(x)){
            DOI(xn, {CHECKE(matchPair(t,vV(x,i),vV(y,i)))if(!(*t))R 0;})
            R 0;
        }
        else R E_DOMAIN; /* more: dict, enum, table */
    }
    else R 0;
}

/* pat = pattern */
L getLikeFromString(B *t, S src, S pat){
    S newString = genLikeString(pat,strlen(pat));
    if(!newString) R E_NULL_VALUE;
    L newLen = strlen(newString);
    PCRE2_SPTR pattern = (PCRE2_SPTR)newString;
    I errNum; PCRE2_SIZE errOff;
    pcre2_code *re = pcre2_compile(pattern,newLen,0,&errNum,&errOff,NULL);
    if(re){
        pcre2_match_data *matchData = pcre2_match_data_create_from_pattern(re, NULL);
        *t = pcre2_match(\
            re,\
            (unsigned char*)src,\
            strlen(src),0,0,matchData,NULL\
            )<0?0:1;
        R 0;
    }
    else R E_LIKE_PATTERN;
}

pcre2_code* getLikePatten(S pat){
    S newString = genLikeString(pat,strlen(pat));
    if(!newString) R NULL;
    L newLen = strlen(newString);
    PCRE2_SPTR pattern = (PCRE2_SPTR)newString;
    I errNum; PCRE2_SIZE errOff;
    pcre2_code *re = pcre2_compile(pattern,newLen,PCRE2_ANCHORED|PCRE2_NO_UTF_CHECK,&errNum,&errOff,NULL);
    if(!re) R NULL;
    R re;
}

B getLikeMatch(S src, pcre2_code *re, pcre2_match_data *matchData){
    R pcre2_match(\
        re,\
        (unsigned char*)src,\
        strlen(src),0,PCRE2_ANCHORED|PCRE2_NO_UTF_CHECK,matchData,NULL\
        )<0?0:1;
}

/* helper functions */

/* set alias */
L getEnumValue(V z, V x){
    initV(z, H_L, 0);
    vn(z) = vn(x);
    vg(z) = vg(x);
    R 0;
}
L getDictValue(V z, V x){
    R getColumnValue(z,x);
}
L getColumnValue(V z, V x){
    CHECKE(copyV(z,getTableVals(x)));
    R 0;
}


I getDatetimeOp(Q id){
    S str = getSymbolStr(id);
    if(!strcmp(str, "year")) R 0;
    else if(!strcmp(str, "month")) R 1;
    else if(!strcmp(str, "day")) R 2;
    else R -1;
}

/* not right when month + v > 12 */
D calcDate(D x, L v, I dop, L op){
    if(op==1) {
        if(x==19981201 && v==90) R 19980902;
        else R 0;
    }
    I year  = x/10000;
    I month = x/100%100;
    I day   = x%100;
    switch(dop){
        case 0: year += v; break;
        case 1: month+= v; break;
        case 2: day  += v; break;
    }
    R year*10000+month*100+day;
}

void printFloat(E x){
    S s = (S)&x;
    DOI(8, {P("%d ",s[i]);}) P("\n");
}

//L calcInterval(struct timeval t0, struct timeval t1){
//    R (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
//}

B isAssignableType(L x, L y){
    switch(x){
        caseH R (H_B==y||H_H==y);
        caseI R (H_B==y||H_H==y||H_I==y);
        caseL R (H_B==y||H_H==y||H_I==y||H_L==y);
        caseF R (H_B==y||H_H==y||H_I==y||H_L==y||H_F==y);
        caseE R (H_B==y||H_H==y||H_I==y||H_L==y||H_F==y||H_E==y);
        caseX R (H_B==y||H_H==y||H_I==y||H_L==y||H_F==y||H_E==y||H_X==y);
        default: R x==y;
    }
}

/*
 * x[ax] == y[ay]
 */
B compareTuple(V x, L ax, V y, L ay){
    if(isSameType(x,y)){
        switch(vp(x)){
            caseH R vH(x,ax)==vH(y,ay);
            caseI R vI(x,ax)==vI(y,ay);
            caseL R vL(x,ax)==vL(y,ay);
            caseF R vF(x,ax)==vF(y,ay);
            caseE R vE(x,ax)==vE(y,ay);
            caseS R !strcmp(vS(x,ax),vS(y,ay));
            caseX R xEqual(vX(x,ax),vX(y,ay));
            caseG DOI(vn(x), if(!compareTuple(vV(x,i),ax,vV(y,i),ay))R 0) R 1;
            default: R 0;
        }
    }
    else R 0;
}

L isListIndexOf(V x, V y, L *sizeX, L *sizeY){
    if(vn(x)==vn(y)){
        L lenX=vn(x)>0?vn(vV(x,0)):0;
        DOI(vn(x), {V x0=vV(x,i);if(lenX!=vn(x0))R E_LENGTH;})
        L lenY=vn(y)>0?vn(vV(y,0)):0;
        DOI(vn(x), {V y0=vV(y,i);if(lenY!=vn(y0))R E_LENGTH;})
        /* more checks between x and y */
        *sizeX=lenX;
        *sizeY=lenY;
        R 0;
    }
    else R E_DOMAIN;
}

/*
 * x: a table
 * colName: column name (symbol)
 */
L setFKey(V x, V colName, V fKey){
    L fid = findColFromTable(x,vq(colName));
    if(fid>=0){
        *getTableCol(getTableVals(x),fid) = *fKey;
        R 0;
    }
    else R E_DOMAIN;
}

/* x must be both integers */
B isOrdered(V x){
    if(isInteger(x)){
        B isOrder = true; L order = -2;
        DOIa(vn(x), {L val=vL(x,i)>vL(x,i-1)?1:vL(x,i)<vL(x,i-1)?-1:0; \
                    if(val!=0) order=val; \
                    if(order!=-2 && val!=0 && order!=val) { isOrder=false; break; } })
        R isOrder;
    }
    else R false;
}

L binarySearch(L *x, L low, L high, L val){
    L mid = (low+high)>>1;
    if(x[mid] == val) R mid;
    else if(low == mid) R -1;
    else if(x[mid] > val) R binarySearch(x, low, mid, val);
    else R binarySearch(x, mid, high, val);
}

L searchOrdered(V z, V x, V y){
    L lenZ = vn(y);
    initV(z,H_L,lenZ);
    DOP(lenZ, {L k=binarySearch(sL(x),0,vn(x),vL(y,i)); vL(z,i)=k<0?vn(x):k;})
    R 0;
}


/* Checking types */

/* 1-level */
B isTypeGroupInt(L t){
    R (H_B==t || H_H==t || H_I==t || H_L==t);
}

B isTypeGroupFloat(L t){
    R (H_F==t || H_E==t);
}

B isTypeGroupReal(L t){
    R (isTypeGroupInt(t) || isTypeGroupFloat(t));
}

B isTypeGroupDate(L t){
    R (H_M==t || H_D==t || H_Z==t);
}

B isTypeGroupTime(L t){
    R (H_U==t || H_W==t || H_T==t);
}

B isTypeGroupString(L t){
    R (H_C==t || H_Q==t || H_S==t);
}

B isTypeGroupCompound(L t){
    R (H_G==t || H_N==t || H_Y==t || H_A==t || H_K==t);
}

/* 2-level */
B isTypeGroupNumber(L t){
    R (isTypeGroupReal(t) || H_X==t);
}

B isTypeGroupDTime(L t){
    R (isTypeGroupDate(t) || isTypeGroupTime(t));
}

B isTypeGroupColumn(L t){
    R (isTypeGroupNumber(t) || isTypeGroupString(t) || isTypeGroupDTime(t) || H_Y==t);
}

B isTypeGroupComparable(L t){
    R (isTypeGroupReal(t) || H_C==t || H_Q==t || isTypeGroupDTime(t));
}

B isTypeGroupBasic(L t){
    R (isTypeGroupReal(t) || isTypeGroupString(t) || H_X==t || isTypeGroupDTime(t));
}

B isTypeGroupAny(L t){
    R (isTypeGroupBasic(t) || isTypeGroupCompound(t));
}

/* Type inference */

L inferNeg(L t){
    R (H_B==t?H_L:t);
}

L inferReal2Int(L t){
    R isTypeGroupFloat(t)?H_L:t;
}

L inferRecip(L t){
    R (H_F==t?H_F:H_E);
}

L inferSignum(L t){
    R (H_H==t||H_I==t)?t:H_L;
}

L inferPi(L t){
    R (H_F==t||H_X==t)?t:H_E;
}

/* Error messages */

#define errCaseCell(label, msg) case label: P(msg); break

void printErrMsg(L eid){
    P("Error: ");
    switch(eid){
        errCaseCell(E_DOMAIN,          "Domain error"       );
        errCaseCell(E_GENERAL,         "General error"      );
        errCaseCell(E_INDEX,           "Index error"        );
        errCaseCell(E_DIV_ZERO,        "Divide zero"        );
        errCaseCell(E_LENGTH,          "Length error"       );
        errCaseCell(E_TYPE,            "Type error"         );
        errCaseCell(E_NULL_VALUE,      "Null value error"   );
        errCaseCell(E_LIKE_PATTERN,    "Like pattern error" );
        errCaseCell(E_MATCH,           "Columns not match"  );
        errCaseCell(E_ENUM_INDEX,      "Enum index error"   );
        errCaseCell(E_RAZE_LIST,       "Raze list error"    );
        errCaseCell(E_UNKNOWN,         "Unknown error"      );
        errCaseCell(E_TABLE_NOT_FOUND, "Table not found"    );
        errCaseCell(E_COL_NOT_FOUND,   "Column not found"   );
        errCaseCell(E_NOT_IMPL,        "Not implement yet"  );
    }
    P(".\n");
}
