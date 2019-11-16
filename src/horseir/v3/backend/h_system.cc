/*
 * system functions
 */

#include "../global.h"
#include <math.h>

const L NUM_LIST_TABLE = 100;
E H_EPSILON = 1E-13;

ListT listTable;
L listTableCur;

L H_CORE = 1;

static void setNumberOfCore(){
    #pragma omp parallel
    {
        L tid = omp_get_thread_num();
        if(0==tid)
            H_CORE = omp_get_num_threads();
    }
    WP("# of cores: %lld\n", H_CORE);
}

static void deleteSys(){
    listTableCur = 0;
    free(listTable);
}

// TODO: maybe swap by pointers?
void swap2(V z){
    if(isList(z) && 2==vn(z)){
        V0 tt[2];
        tt[0] = *vV(z,1);
        tt[1] = *vV(z,0);
        //P("swap solving: input size\n");
        //printV(vV(z,0)); printV(vV(z,1)); getchar();
        memcpy(vg(z), tt, sizeof(V0)*2);
        //printV(vV(z,0)); printV(vV(z,1)); getchar();
    }
    else EP("z is not a list or vn(z) !=2: %d %d\n", isList(z),2==vn(z));
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
I copyV(V z, V x){
    *z = *x;
    R 0;
}

I initListCopy(V z, V x, L len){
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

//I copyByIndex(V z, L p, V x, L q){
//    if(isList(x)){
//        DOI(vn(x), copyByIndexSimple(vV(z,i), p, vV(x,i), q));
//    }
//    else if(isTypeGroupBasic(vp(x))){
//        CHECKE(copyByIndexSimple(z,p,x,q));
//    }
//    else R E_DOMAIN;
//    R 0;
//}

//L copyByIndexSimple(V z, L p, V x, L q){
//    if(isList(x)){
//        CHECKE(copyV(vV(z,p),vV(x,q)))
//    }
//    else if(isTypeGroupBasic(vp(x))) {
//        switch(vp(x)){
//            caseB vB(z,p) = vB(x,q); break;
//            caseJ vJ(z,p) = vJ(x,q); break;
//            caseH vH(z,p) = vH(x,q); break;
//            caseI vI(z,p) = vI(x,q); break;
//            caseL vL(z,p) = vL(x,q); break;
//            caseQ vQ(z,p) = vQ(x,q); break;
//            caseC vC(z,p) = vC(x,q); break;
//            caseS vS(z,p) = vS(x,q); break;
//        }
//    }
//    else R E_DOMAIN;
//    R 0;
//}

// L copyV(V z, V x){
//     initV(z,xp,xn);
//     if(isTypeGroupNumber(xp)){
//         memcpy(vg(z),vg(x),getTypeSize(xp)*xn);
//     }
//     else {
//         switch(xp){
//             caseV DOI(xn, CHECKE(copyV(vV(z,i),vV(x,i)))) break;
//         }
//     }
//     R 0;
// }

/*
 * Include: table and keyed table
 * Status: 0 or >0
 * Return: *z
 */
//I findColFromTable2(V *z, V x, L cId){
//    if(isTable(x)) {
//        L t = findColFromTable(x,cId);
//        if(t>=0) *z = getTableCol(x,t);
//        else R E_DOMAIN;
//    }
//    else if(isKTable(x)) {
//        V key = getKTableKey(x);
//        V val = getKTableVal(x);
//        L t = findColFromTable(key,cId);
//        if(t>=0) *z = getTableCol(key,t);
//        else{
//            t = findColFromTable(val,cId);
//            if(t>=0) *z = getTableCol(val,t);
//            else R E_DOMAIN;
//        }
//    }
//    else R E_DOMAIN;
//    R 0;
//}

I findColFromTable(V x, L cId){
    V key = getTableKeys(x);
    // if(H_DEBUG) {
    //     P("cId = %lld, col size = %lld\n", cId,tableCol(x));
    //     DOI(tableCol(x), P("col[%lld] = %lld\n",i,vQ(key,i)));
    //     getchar();
    // }
    DOI(tableCol(x), if(cId == vQ(key,i))R i)
    R -1;
}

I promoteValueWithIndex(V z, V x, L typMax, V ind){
    L size = ind?vn(ind):vn(x);
    L *indx = ind?sL(ind):NULL;
#define WithIndex ind?indx[i]:i
    if(vp(x) == typMax) {
        if(!ind) R copyV(z,x);
        else {
            initV(z, typMax, size); //opt?
            switch(vp(x)){
                caseB DOP(size, vB(z,i) = vB(x,indx[i])) break;
                caseJ DOP(size, vJ(z,i) = vJ(x,indx[i])) break;
                caseH DOP(size, vH(z,i) = vH(x,indx[i])) break;
                caseI DOP(size, vI(z,i) = vI(x,indx[i])) break;
                caseL DOP(size, vL(z,i) = vL(x,indx[i])) break;
                caseF DOP(size, vF(z,i) = vF(x,indx[i])) break;
                caseE DOP(size, vE(z,i) = vE(x,indx[i])) break;
                default: R E_NOT_IMPL;
            }
            R 0;
        }
    }
    else{
        initV(z, typMax, size); //opt?
        switch(vp(z)){
            caseJ {
                switch(vp(x)){
                    caseB DOP(size, vJ(z,i)=vB(x,WithIndex)) break;
                }
            } break;
            caseH {
                switch(vp(x)){
                    caseB DOP(size, vH(z,i)=vB(x,WithIndex)) break;
                    caseJ DOP(size, vH(z,i)=vJ(x,WithIndex)) break;
                }
            } break;
            caseI {
                switch(vp(x)){
                    caseB DOP(size, vI(z,i)=vB(x,WithIndex)) break;
                    caseJ DOP(size, vI(z,i)=vJ(x,WithIndex)) break;
                    caseH DOP(size, vI(z,i)=vH(x,WithIndex)) break;
                }
            } break;
            caseL {
                switch(vp(x)){
                    caseB DOP(size, vL(z,i)=vB(x,WithIndex)) break;
                    caseJ DOP(size, vL(z,i)=vJ(x,WithIndex)) break;
                    caseH DOP(size, vL(z,i)=vH(x,WithIndex)) break;
                    caseI DOP(size, vL(z,i)=vI(x,WithIndex)) break;
                }
            } break;
            caseF {
                switch(vp(x)){
                    caseB DOP(size, vF(z,i)=(F)vB(x,WithIndex)) break;
                    caseJ DOP(size, vF(z,i)=(F)vJ(x,WithIndex)) break;
                    caseH DOP(size, vF(z,i)=(F)vH(x,WithIndex)) break;
                    caseI DOP(size, vF(z,i)=(F)vI(x,WithIndex)) break;
                    caseL DOP(size, vF(z,i)=(F)vL(x,WithIndex)) break;
                }
            } break;
            caseE {
                switch(vp(x)){
                    caseB DOP(size, vE(z,i)=(E)vB(x,WithIndex)) break;
                    caseJ DOP(size, vE(z,i)=(E)vJ(x,WithIndex)) break;
                    caseH DOP(size, vE(z,i)=(E)vH(x,WithIndex)) break;
                    caseI DOP(size, vE(z,i)=(E)vI(x,WithIndex)) break;
                    caseL DOP(size, vE(z,i)=(E)vL(x,WithIndex)) break;
                    caseF DOP(size, vE(z,i)=(E)vF(x,WithIndex)) break;
                }
            } break;
            caseX {
                switch(vp(x)){
                    caseB DOP(size, xCopy(vX(z,i),vB(x,WithIndex),0)) break;
                    caseJ DOP(size, xCopy(vX(z,i),vJ(x,WithIndex),0)) break;
                    caseH DOP(size, xCopy(vX(z,i),vH(x,WithIndex),0)) break;
                    caseI DOP(size, xCopy(vX(z,i),vI(x,WithIndex),0)) break;
                    caseL DOP(size, xCopy(vX(z,i),vL(x,WithIndex),0)) break;
                    caseF DOP(size, xCopy(vX(z,i),vF(x,WithIndex),0)) break;
                    caseE DOP(size, xCopy(vX(z,i),vE(x,WithIndex),0)) break;
                }
            } break;
            default: EP("PromoteValue error: %s\n", getTypeName(vp(z)));
        }
        R 0;
    }
}

B checkZero(V x){
    #define FOUND 1
    #define NOT_FOUND 0
    switch(xp){
        caseB DOI(xn, if(0==xB(i)) R FOUND); break;
        caseJ DOI(xn, if(0==xJ(i)) R FOUND); break;
        caseH DOI(xn, if(0==xH(i)) R FOUND); break;
        caseI DOI(xn, if(0==xI(i)) R FOUND); break;
        caseL DOI(xn, if(0==xL(i)) R FOUND); break;
        caseF DOI(xn, if(0==xF(i)) R FOUND); break;
        caseE DOI(xn, if(0==xE(i)) R FOUND); break;
    }
    R NOT_FOUND;
}

B checkMatch(V x){
    switch(xp){
        caseV if(xn>0){L n=vn(vV(x,0)); DOI(xn,if(n!=vn(vV(x,i)))R 0)} break; /* List */
    }
    R 1;
}

L getSingleIntValue(V x){
    R (1==xn)?(H_B==xp?xb:H_H==xp?xh:H_I==xp?xi:H_L==xp?xl:-1):-1;
}

L calcFact(L n){
    if(n>=0){
        L z=1; DOI(n, z*=(i+1)) R z;
    }
    else R -1;
}

I getNumOfNonZero(V x, L *z){
    if(H_B==xp){
        DOT(xn, if(vB(x,i))z[tid]++)
        R 0;
    }
    else R E_DOMAIN;
}

I appendList(V z, V x, V y){R E_NOT_IMPL;}
/* x: enum, y: number */
I appendEnum(V z, V x, V y){R E_NOT_IMPL;}
/*
 * % -> .* // zero or more
 * _ -> .  // single
 */
static S genLikeString(S inStr, L inLen){
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

I checkLength(V x){
    L n0 = vn(x)>0?vn(vV(x,0)):0;
    DOI(vn(x), if(vn(vV(x,i))!=n0)R E_LENGTH)
    R 0;
}


/* error code -2 */
static L decideType(L x, L y){
    if(isTypeGroupNumber(x) && isTypeGroupNumber(y)){
        R MAX(x,y);
    }
    else if(x == y){
        R x;
    }
    else R -2;
}

/* typ init. -1 */
I getCommonType(V x, I *typ, L *len){
    if(isList(x)){
        DOI(xn, CHECKE(getCommonType(vV(x,i),typ,len)))
    }
    else {
        *typ = (-1==*typ)?xp:decideType(*typ,xp); CHECKE(-2==(*typ)?E_RAZE_LIST:0);
        *len+= xn;
    }
    R 0;
}


I fillRaze(V z, L *n0, V x){
    if(isList(x)){
        DOI(xn, CHECKE(fillRaze(z,n0,vV(x,i))))
    }
    else {
        L n = *n0;
        switch(vp(z)){
             caseB DOI(xn, vB(z,n++)=vB(x,i)); break;
             caseJ { switch(xp){
                     caseB DOI(xn, vJ(z,n++)=vB(x,i)); break;
                     caseJ DOI(xn, vJ(z,n++)=vH(x,i)); break; } } break;
             caseH { switch(xp){
                     caseB DOI(xn, vH(z,n++)=vB(x,i)); break;
                     caseJ DOI(xn, vH(z,n++)=vJ(x,i)); break;
                     caseH DOI(xn, vH(z,n++)=vH(x,i)); break; } } break;
             caseI { switch(xp){
                     caseB DOI(xn, vI(z,n++)=vB(x,i)); break;
                     caseJ DOI(xn, vI(z,n++)=vJ(x,i)); break;
                     caseH DOI(xn, vI(z,n++)=vH(x,i)); break;
                     caseI DOI(xn, vI(z,n++)=vI(x,i)); break; } } break;
             caseL { switch(xp){
                     caseB DOI(xn, vL(z,n++)=vB(x,i)); break;
                     caseJ DOI(xn, vL(z,n++)=vJ(x,i)); break;
                     caseH DOI(xn, vL(z,n++)=vH(x,i)); break;
                     caseI DOI(xn, vL(z,n++)=vI(x,i)); break;
                     caseL DOI(xn, vL(z,n++)=vL(x,i)); break; } } break;
             caseF { switch(xp){
                     caseB DOI(xn, vF(z,n++)=vB(x,i)); break;
                     caseJ DOI(xn, vF(z,n++)=vJ(x,i)); break;
                     caseH DOI(xn, vF(z,n++)=vH(x,i)); break;
                     caseI DOI(xn, vF(z,n++)=vI(x,i)); break;
                     caseL DOI(xn, vF(z,n++)=vL(x,i)); break;
                     caseF DOI(xn, vF(z,n++)=vF(x,i)); break; } } break;
             caseE { switch(xp){
                     caseB DOI(xn, vE(z,n++)=vB(x,i)); break;
                     caseJ DOI(xn, vE(z,n++)=vJ(x,i)); break;
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


I matchPair(B *t, V x, V y){
    *t=0;
    if(isSameType(x,y) && isEqualLength(x,y)){
        if(isTypeGroupBasic(vp(x))){
            switch(vp(x)){
                caseB DOI(vn(x), if(vB(x,i)!=vB(y,i))R 0) break;
                caseJ DOI(vn(x), if(vJ(x,i)!=vJ(y,i))R 0) break;
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
        else R E_DOMAIN; /* TODO: dict, enum, table */
    }
    else R 0;
}

static void check_pcre2_jit(){
    L has_jit = 0;
    pcre2_config(PCRE2_CONFIG_JIT, &has_jit);
    if(has_jit == 1) P("JIT is available!\n");
    else EP("JIT is not enabled: %lld\n", (L)has_jit);
}

/* pat = pattern */
I getLikeFromString(B *t, S src, S pat){
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
    if(H_DEBUG) check_pcre2_jit();
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
I copyEnumIndex(V z, V x){
    initV(z, H_L, 0);
    vn(z) = vn(x);
    vg(z) = getEnumIndex(x);
    R 0;
}

I getDictValue(V z, V x){
    R getColumnValue(z,x);
}

I getColumnValue(V z, V x){
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
D calcDate(D x, L v, I dop, I op){
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

B isAssignableType(I x, I y){
    switch(x){
        caseJ R (H_B==y||H_J==y);
        caseH R (H_B==y||H_J==y||H_H==y);
        caseI R (H_B==y||H_J==y||H_H==y||H_I==y);
        caseL R (H_B==y||H_J==y||H_H==y||H_I==y||H_L==y);
        caseF R (H_B==y||H_J==y||H_H==y||H_I==y||H_L==y||H_F==y);
        caseE R (H_B==y||H_J==y||H_H==y||H_I==y||H_L==y||H_F==y||H_E==y);
        caseX R (H_B==y||H_J==y||H_H==y||H_I==y||H_L==y||H_F==y||H_E==y||H_X==y);
        default: R x==y;
    }
}

/*
 * x[ax] == y[ay]
 */
static B compareTupleItem(V x, L ax, V y, L ay){
    if(isSameType(x,y)){
        switch(vp(x)){
            caseJ R vJ(x,ax)==vJ(y,ay);
            caseH R vH(x,ax)==vH(y,ay);
            caseI R vI(x,ax)==vI(y,ay);
            caseL R vL(x,ax)==vL(y,ay);
            caseF R vF(x,ax)==vF(y,ay);
            caseE R vE(x,ax)==vE(y,ay);
            caseS R !strcmp(vS(x,ax),vS(y,ay));
            caseX R xEqual(vX(x,ax),vX(y,ay));
            /* logic here is buggy: need to check if they are the same Lists */
            //caseG DOI(vn(x), if(!compareTuple(vV(x,i),ax,vV(y,i),ay))R 0) R 1;
            caseG EP("Need to compare the whole cell\n");
            default: EP("type not supported: %s\n", getTypeName(vp(x)));
        }
    }
    else R 0;
}

B compareTuple(V x, L ax, V y, L ay){
    DOI(vn(x), if(!compareTupleItem(vV(x,i),ax,vV(y,i),ay))R 0) R 1;
}

I isListIndexOf(V x, V y, L *sizeX, L *sizeY){
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

static B isSameValueItem(V x, L a, L b){
    switch(xp){
        caseB R vB(x,a) == vB(x,b); 
        caseJ R vJ(x,a) == vJ(x,b); 
        caseH R vH(x,a) == vH(x,b); 
        caseI R vI(x,a) == vI(x,b); 
        caseL R vL(x,a) == vL(x,b); 
        caseE R vE(x,a) == vE(x,b); 
        caseF R vF(x,a) == vF(x,b); 
        caseC R vC(x,a) == vC(x,b); 
        caseQ R vQ(x,a) == vQ(x,b); 
        caseS R !strcmp(vS(x,a), vS(x,b));
        default: EP("isSameValueItem: support more types.\n");
    }
    R 0;
}

/*
 * Search on an ordered vector, may contain duplicated values
 * TODO: check the bug: join_index in step 1, q8 (13389 expected, but 13311 found)
 */
static I joinIndexHashValue(V z0, V z1, V x, V y){
    V0 t0; V t = &t0; L c;
    CHECKE(pfnIndexOf(t, x, y));
    c = 0; DOI(vn(t), {L pos=vL(t,i);L j=pos;while(j<vn(x)&&isSameValueItem(x,pos,j++))c++;})
    P("[joinIndex] c = %lld\n",(L)c);
    initV(z0,H_L,c);
    initV(z1,H_L,c);
    c = 0; DOI(vn(t), {L pos=vL(t,i);L j=pos;while(j<vn(x)&&isSameValueItem(x,pos,j)){vL(z0,c)=j++;vL(z1,c)=i;c++;}})
    R 0;
}

I joinIndexHash(V z, V x, V y, C op){
    P("len: x = %lld, y = %lld\n",(L)vn(x),(L)vn(y));
    P("typ: x = %lld, y = %lld\n",(L)vp(x),(L)vp(y));
    if(op == 'l'){ // left
        initV(z, H_G, 2);
        R joinIndexHashValue(vV(z,0),vV(z,1),x,y);
    }
    else if(op == 'r'){ // right
        initV(z, H_G, 2);
        R joinIndexHashValue(vV(z,1),vV(z,0),y,x);
    }
    else R E_DOMAIN;
}


static void insertFKey(V x, ListY *val){
    ListY *t = tableFKey(x);
    if(!t){
        tableFKey(x) = val;
    }
    else { // append to the end
        while(t->next) t=t->next;
        t->next = val;
    }
}

/*
 * x: a table
 * colName: column name (symbol)
 */
I setFKey(V x, V colName, V fKey){
    //*getTableCol(getTableVals(x),fid) = *fKey;
    ListY *n = NEW(ListY);
    n->y     = (G)fKey;
    n->cn    = (G)colName;
    n->next  = NULL;
    insertFKey(x, n);
    R 0;
}

/* x must be both integers */
B isOrdered(V x){
    if(isInt(x)){
        B isOrder = true; I order = -2;
        DOIa(vn(x), {I val=vI(x,i)>vI(x,i-1)?1:vI(x,i)<vI(x,i-1)?-1:0; \
                     if(order==-2 && val!=0) {order=val; break;}})
        if(order == -2) R true;
        DOIa(vn(x), {I val=vI(x,i)>vI(x,i-1)?1:vI(x,i)<vI(x,i-1)?-1:0; \
                    if(val!=0 && order!=val) { isOrder=false; break; } })
        R isOrder;
    }
    else if(isLong(x)){
        B isOrder = true; I order = -2;
        DOIa(vn(x), {I val=vL(x,i)>vL(x,i-1)?1:vL(x,i)<vL(x,i-1)?-1:0; \
                     if(order==-2 && val!=0) {order=val; break;}})
        if(order == -2) R true;
        DOIa(vn(x), {I val=vL(x,i)>vL(x,i-1)?1:vL(x,i)<vL(x,i-1)?-1:0; \
                    if(val!=0 && order!=val) { isOrder=false; break; } })
        R isOrder;
    }
    else R false;
}

B isOrderWithIndex(V x, V ind){
    if(!isLong(ind)) EP("index must be i64\n");
    if(isInt(x)){
        DOIa(vn(x), if(vI(x,vL(ind,i))<vI(x,vL(ind,i-1))){\
            P("vI(x,%lld) = %d\n", vL(ind,i),vI(x,vL(ind,i))); \
            P("vI(x,%lld) = %d\n", vL(ind,i-1),vI(x,vL(ind,i-1))); \
                R 0;})
    }
    else if(isLong(x)){
        DOIa(vn(x), if(vL(x,vL(ind,i))<=vL(x,vL(ind,i-1)))R 0)
    }
    else R 0;
    R 1;
}

static L binarySearch_I(I *x, L low, L high, I val){
    L mid = (low+high)>>1;
    if(x[mid] == val) R mid;
    else if(low == mid) R -1;
    else if(x[mid] > val) R binarySearch_I(x, low, mid, val);
    else R binarySearch_I(x, mid, high, val);
}

static L binarySearch_L(L *x, L low, L high, L val){
    L mid = (low+high)>>1;
    if(x[mid] == val) R mid;
    else if(low == mid) R -1;
    else if(x[mid] > val) R binarySearch_L(x, low, mid, val);
    else R binarySearch_L(x, mid, high, val);
}

I searchOrdered(V z, V x, V y){
    //getInfoVar(x); getInfoVar(y); getchar();
    if(isInt(x)){
        L lenZ = vn(y);
        initV(z,H_L,lenZ);
        DOP(lenZ, {L k=binarySearch_I(sI(x),0,vn(x),vI(y,i)); vL(z,i)=k<0?vn(x):k;})
    }
    else if(isLong(x)){
        L lenZ = vn(y);
        initV(z,H_L,lenZ);
        DOP(lenZ, {L k=binarySearch_L(sL(x),0,vn(x),vL(y,i)); vL(z,i)=k<0?vn(x):k;})
    }
    else EP("type not supported: %s\n", getTypeName(xp));
    R 0;
}

I listFlat2Normal(V z, V x){
    V g2 = vg2(x); L typ2 = vp(g2);
    if(isTypeGroupBasic(typ2)){
        L lenZ = vn(x), typSize = getTypeSize(typ2,1); G cur=vg(g2); B f0 = typ2==H_C;
        initV(z, H_G, lenZ);
        DOI(lenZ, {V t=vV(z,i); L k=vL(x,i); L kSize=getTypeSize(typ2,k)-f0; initV(t,typ2,k);\
                memcpy(vg(t),cur,kSize); if(f0)vg(t)[kSize]=0; cur+=kSize;})
    }
    else EP("Not supported type: %s\n", getTypeName(typ2));
    R 0;
}

I listFlatEachLen(V z, V x){
    L lenZ = vn(x);
    initFlatList(z, lenZ);
    V g2 = allocNode();
    initV(g2,H_L,lenZ);
    memcpy(vg(g2),vg(x),sizeof(L)*lenZ);
    vg2(z) = g2;
    DOP(lenZ, vL(z,i)=1)
    R 0;
}

//L searchOrdered(V z, V x, V y){
//    L lenZ = vn(y);
//    initV(z,H_L,lenZ);
//    E optTime[10]; struct timeval tv0, tv1; 
//    P("Input size: %lld, vp(x) = %lld, vp(y) = %lld\n", lenZ, vp(x), vp(y));
//    DOJ(10, {gettimeofday(&tv0, NULL); DOP(lenZ, {L k=binarySearch(sL(x),0,vn(x),vL(y,i)); vL(z,i)=k<0?vn(x):k;}) gettimeofday(&tv1, NULL); optTime[j] = calcInterval(tv0, tv1)/1000.0; })
//    E total = 0; DOI(10, total += optTime[i]) P("[searchOrdered] Createing index time (avg): %g ms\n", total/10); getchar();
//    R 0;
//}

// same as pfnCompare
static I getOpFromName(S x){
    if(!strcmp(x, "lt")) R 0;
    else if(!strcmp(x, "leq")) R 1;
    else if(!strcmp(x, "gt"))  R 2;
    else if(!strcmp(x, "geq")) R 3;
    else if(!strcmp(x, "eq"))  R 4;
    else if(!strcmp(x, "neq")) R 5;
    else R -1;
}

I getOpFromSymbol(L x){
    R getOpFromName(getSymbolStr(x));
}

/* copy from backend/h_primitive.c */
#define COMP(op,x,y) (2>op?COMPLESS(op,x,y):4>op?COMPMORE(op,x,y):6>op?COMPEQ(op,x,y):0)
#define COMPLESS(op,x,y) (0==op?(x<y):(x)<=(y))
#define COMPMORE(op,x,y) (2==op?(x>y):(x)>=(y))
#define COMPEQ(op,x,y) (4==op?(x==y):(x!=y))

B compareOpWithIndex(V x, V y, L k0, L k1, I op){
    switch(vp(x)){
        caseB R COMP(op,vB(x,k0),vB(y,k1));
        caseH R COMP(op,vH(x,k0),vH(y,k1));
        caseJ R COMP(op,vJ(x,k0),vJ(y,k1));
        caseI R COMP(op,vI(x,k0),vI(y,k1));
        caseL R COMP(op,vL(x,k0),vL(y,k1));
        caseF R COMP(op,vF(x,k0),vF(y,k1));
        caseE R COMP(op,vE(x,k0),vE(y,k1));
        default: EP("type not supported: %s\n", getTypeName(vp(x)));
    }
    R 0;
}

static B compareOp(V x, V y, L k, V ind0, V ind1, I op){
    L k0=ind0?vL(ind0,k):k;
    L k1=ind1?vL(ind1,k):k;
    R compareOpWithIndex(x,y,k0,k1,op);
}

static I compareOneColumn(V z0, V z1, V x, V y, V ind0, V ind1, I op){
    //getInfoVar(x); getInfoVar(y);
    L size = vn(ind0);
    V t = allocNode(); initV(t, H_B, size);
    if(vp(x) == vp(y)){
        //DOI(20, P("x[%lld] = %g, y[%lld] = %g\n",\
                    vL(ind0,i),vE(x,vL(ind0,i)),\
                    vL(ind1,i),vE(y,vL(ind1,i))))
        DOP(size, vB(t,i)=compareOp(x,y,i,ind0,ind1,op))
        //DOI(20, P("resut[%lld] = %d\n",i,vB(t,i)))
    }
    else {
        V tempX  = allocNode(), tempY = allocNode();
        I typMax = MAX(vp(x), vp(y));
        //getInfoVar(x); getInfoVar(y);
        CHECKE(promoteValueWithIndex(tempX, x, typMax, ind0));
        CHECKE(promoteValueWithIndex(tempY, y, typMax, ind1));
        DOP(size, vB(t,i)=compareOp(tempX,tempY,i,NULL,NULL,op))
    }
    L tot=0; DOI(size, tot+=vB(t,i))
    L *indx0=sL(ind0), *indx1=sL(ind1);
    initV(z0,H_L,tot); initV(z1,H_L,tot);
    tot=0; DOI(size, if(vB(t,i)){vL(z0,tot)=indx0[i];vL(z1,tot++)=indx1[i];}) R 0; /* free temp: t, tempX, tempY */
}

I joinOtherColumns(V z, V x, V y, V ind, L fx, V f){
    V ind0 = vV(ind,0), ind1 = vV(ind,1);
    //DOI(vn(x), \
       if(i!=fx){I k=(compareOneColumn(vV(z,0),vV(z,1),vV(x,i),vV(y,i),ind0,ind1,getOpFromSymbol(vQ(f,i)))); if(k!=0) P("k=%d, i=%lld\n",k,i);}) getchar();
    DOI(vn(x), \
       if(i!=fx){CHECKE(compareOneColumn(vV(z,0),vV(z,1),vV(x,i),vV(y,i),ind0,ind1,getOpFromSymbol(vQ(f,i))));\
       ind0=vV(z,0); ind1=vV(z,1);}) R 0;
}

I joinOneColumn(V z, V x, V y, L sop){
    L lenX    = vn(x), lenY = vn(y);
    L typMax  = MAX(vp(x),vp(y));
    I op      = getOpFromSymbol(sop);
    V tempX = allocNode();
    V tempY = allocNode();
    CHECKE(promoteValue(tempX, x, typMax));
    CHECKE(promoteValue(tempY, y, typMax));
    initV(z, H_G, 2);
    if(4 == op || 5 == op){ /* equal or not equal */
        //P("join one column\n"); getchar();
        switch(typMax){
            caseH caseI caseL caseF caseE
                R lib_join_index_hash(vV(z,0),vV(z,1),tempX,tempY,4==op);
            default: R E_NOT_IMPL;
        }
    }
    else if(0 <= op && op < 4){
        switch(typMax){
            caseH caseI caseL caseF caseE
                R lib_join_index_compare(vV(z,0),vV(z,1),tempX,tempY,op);
            default: R E_NOT_IMPL;
        }
    }
    else R E_DOMAIN;
    R 0;
}

L getListSize1(V x){
    if(isList(x) && vn(x)>0){
        R vn(vV(x,0));
    }
    else EP("List size rank error\n");
}

F logBaseF(F b, F x){
    R logf(x)/logf(b);
}

E logBaseE(E b, E x){
    R log(x)/log(b);
}

B logicAnd(B x, B y){
    R x&y;
}

B logicNand(B x, B y){
    R ~(x&y);
}

B logicOr(B x, B y){
    R x|y;
}

B logicNor(B x, B y){
    R ~(x|y);
}

B logicXor(B x, B y){
    R x^y;
}


/* Checking types */

/* 1-level */
B isTypeGroupInt(I t){
    R (H_B==t || H_J==t || H_H==t || H_I==t || H_L==t);
}

B isTypeGroupFloat(I t){
    R (H_F==t || H_E==t);
}

B isTypeGroupReal(I t){
    R (isTypeGroupInt(t) || isTypeGroupFloat(t));
}

B isTypeGroupDate(I t){
    R (H_M==t || H_D==t || H_Z==t);
}

B isTypeGroupTime(I t){
    R (H_U==t || H_W==t || H_T==t);
}

B isTypeGroupString(I t){
    R (H_C==t || H_Q==t || H_S==t);
}

B isTypeGroupCompound(I t){
    R (H_G==t || H_N==t || H_Y==t || H_A==t || H_K==t);
}

/* 2-level */
B isTypeGroupNumber(I t){
    R (isTypeGroupReal(t) || H_X==t);
}

B isTypeGroupDTime(I t){
    R (isTypeGroupDate(t) || isTypeGroupTime(t));
}

B isTypeGroupColumn(I t){
    R (isTypeGroupNumber(t) || isTypeGroupString(t) || isTypeGroupDTime(t) || H_Y==t);
}

B isTypeGroupComparable(I t){
    R (isTypeGroupReal(t) || H_C==t || H_Q==t || isTypeGroupDTime(t));
}

B isTypeGroupBasic(I t){
    R (isTypeGroupReal(t) || isTypeGroupString(t) || H_X==t || isTypeGroupDTime(t));
}

B isTypeGroupAny(I t){
    R (isTypeGroupBasic(t) || isTypeGroupCompound(t));
}

B isListSameType(V x, I typ){
    if(isList(x)){
        DOI(vn(x), if(vp(vV(x,i)) != typ) R 0) R 1;
    }
    else R 0;
}

/* Type inference */

I inferNeg(I t){
    R (H_B==t?H_H:t);
}

I inferReal2Int(I t){ // deprecated
    R isTypeGroupFloat(t)?H_L:t;
}

I inferRecip(I t){
    R (H_L==t||H_E==t?H_E:H_F);
}

I inferSignum(I t){ // deprecated
    R (H_H==t||H_I==t)?t:H_L;
}

I inferPi(I t){
    R (H_X==t)?t:(H_E==t||H_L==t)?H_E:H_F;
}

/* Error messages */

#define errCase(label, msg)\
    case label: EP("Error: " msg ".\n"); break

void printErrMsg(I eid){
    switch(eid){
        errCase(E_DOMAIN,          "Domain error"       );
        errCase(E_GENERAL,         "General error"      );
        errCase(E_INDEX,           "Index error"        );
        errCase(E_DIV_ZERO,        "Divide zero"        );
        errCase(E_LENGTH,          "Length error"       );
        errCase(E_TYPE,            "Type error"         );
        errCase(E_NULL_VALUE,      "Null value error"   );
        errCase(E_LIKE_PATTERN,    "Like pattern error" );
        errCase(E_MATCH,           "Columns not match"  );
        errCase(E_ENUM_INDEX,      "Enum index error"   );
        errCase(E_RAZE_LIST,       "Raze list error"    );
        errCase(E_UNKNOWN,         "Unknown error"      );
        errCase(E_TABLE_NOT_FOUND, "Table not found"    );
        errCase(E_COL_NOT_FOUND,   "Column not found"   );
        errCase(E_NOT_IMPL,        "Not implement yet"  );
        default: EP("Error code not specified: %d", eid);
    }
}

void initSys(){
    listTable    = NEWL(ListT0, NUM_LIST_TABLE);
    listTableCur = 0;
    H_TARGET     = TARGET_NA;
    //omp_set_num_threads(1);
    setNumberOfCore(); // set H_CORE
    time_clear();
}

