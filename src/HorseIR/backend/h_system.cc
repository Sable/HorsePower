/*
 * system functions
 */

#include "h_global.h"

const L NUM_LIST_TABLE = 100;

ListT listTable;
L listTableCur;

void initSys(){
    listTable = (ListT)malloc(sizeof(ListT0) * NUM_LIST_TABLE);
    listTableCur = 0;
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
    initV(z,xp,xn);
    if(isTypeGroupScalar(xp)){
        memcpy(vg(z),vg(x),getTypeSize(xp)*xn);
    }
    else {
        switch(xp){
            caseG DOI(xn, CHECKE(copyV(vV(z,i),vV(x,i)))) break;
        }
    }
    R 0;
}

L findColFromTable(V x, L cId){
    DOI(vn(x), if(cId == vq(getDictKey(getTableDict(x,i))))R i)
    R -1;
}

L promoteValue(V z, V x, L typMax){
    if(vp(x) == typMax) R copyV(z,x);
    else{
        initV(z, typMax, xn); //opt?
        switch(vp(z)){
            caseH {
                switch(vp(x)){
                    caseB DOI(xn, vH(z,i)=vB(x,i)) break;
                }
            } break;
            caseI {
                switch(vp(x)){
                    caseB DOI(xn, vI(z,i)=vB(x,i)) break;
                    caseH DOI(xn, vI(z,i)=vH(x,i)) break;
                }
            } break;
            caseL {
                switch(vp(x)){
                    caseB DOI(xn, vL(z,i)=vB(x,i)) break;
                    caseH DOI(xn, vL(z,i)=vH(x,i)) break;
                    caseI DOI(xn, vL(z,i)=vI(x,i)) break;
                }
            } break;
            caseF {
                switch(vp(x)){
                    caseB DOI(xn, vF(z,i)=(F)vB(x,i)) break;
                    caseH DOI(xn, vF(z,i)=(F)vH(x,i)) break;
                    caseI DOI(xn, vF(z,i)=(F)vI(x,i)) break;
                    caseL DOI(xn, vF(z,i)=(F)vL(x,i)) break;
                }
            } break;
            caseE {
                switch(vp(x)){
                    caseB DOI(xn, vE(z,i)=(E)vB(x,i)) break;
                    caseH DOI(xn, vE(z,i)=(E)vH(x,i)) break;
                    caseI DOI(xn, vE(z,i)=(E)vI(x,i)) break;
                    caseL DOI(xn, vE(z,i)=(E)vL(x,i)) break;
                    caseF DOI(xn, vE(z,i)=(E)vF(x,i)) break;
                }
            } break;
            caseX {
                switch(vp(x)){
                    caseB DOI(xn, xCopy(vX(z,i),vB(x,i),0)) break;
                    caseH DOI(xn, xCopy(vX(z,i),vH(x,i),0)) break;
                    caseI DOI(xn, xCopy(vX(z,i),vI(x,i),0)) break;
                    caseL DOI(xn, xCopy(vX(z,i),vL(x,i),0)) break;
                    caseF DOI(xn, xCopy(vX(z,i),vF(x,i),0)) break;
                    caseE DOI(xn, xCopy(vX(z,i),vE(x,i),0)) break;
                }
            } break;
            default: R E_DOMAIN;
        }
        R 0;
    }
}

B checkZero(V x){
    switch(xp){
        caseB DOI(xn, if(0==xB(i)) R 0); break;
        caseH DOI(xn, if(0==xH(i)) R 0); break;
        caseI DOI(xn, if(0==xI(i)) R 0); break;
        caseL DOI(xn, if(0==xL(i)) R 0); break;
        caseF DOI(xn, if(0==xF(i)) R 0); break;
        caseE DOI(xn, if(0==xE(i)) R 0); break;
    }
    R 1;
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

L getNumOfNonZero(V x){
    if(H_B==xp){
        L z = 0; DOI(xn, z+=vB(x,i)) R z;
    }
    R -1;
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

/* error code -2 */
L decideType(L x, L y){
    if(isTypeGroupScalar(x) && isTypeGroupScalar(y)){
        R max(x,y);
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
             caseX DOI(xn, vX(z,n++)=vX(x,i)); break;
             caseQ DOI(xn, vQ(z,n++)=vQ(x,i)); break;
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
                caseX DOI(vn(x), if(!xEqual(vX(x,i),vX(y,i)))R 0) break;
                caseQ DOI(vn(x), if(vQ(x,i)!=vQ(y,i))R 0) break;
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

/* helper functions */

L getEnumValue(V z, V x){
    R 0;
}
L getDictValue(V z, V x){
    R 0;
}
L getColumnValue(V z, V x, V y){
    R 0;
}

V getDictKey(V x) { R xV(0); }

V getDictVal(V x) { R xV(1); }

V getTableDict(V x, L k) { R xV(k); }

L getTableRowNumber(V x){
    R va(x).row;
    // R (vn(x)>0?vn(getDictVal(getTableDict(x,0))):0);
}

L getTableColNumber(V x){
    R va(x).col;
}


/* Type checking */

B isTypeGroupInt(L t){
    R (H_B==t || H_H==t || H_I==t || H_L==t);
}

B isTypeGroupFloat(L t){
    R (H_F==t || H_E==t);
}

B isTypeGroupReal(L t){
    R (isTypeGroupInt(t) || isTypeGroupFloat(t));
}

B isTypeGroupRealX(L t){
    R (isTypeGroupReal(t) || H_X==t);
}

B isTypeGroupDate(L t){
    R (H_M==t || H_D==t || H_Z==t);
}

B isTypeGroupTime(L t){
    R (H_U==t || H_W==t || H_T==t);
}

B isTypeGroupDTime(L t){
    R isTypeGroupDate(t) || isTypeGroupTime(t);
}

B isTypeGroupNumeric(L t){
    R isTypeGroupReal(t) || isTypeGroupDTime(t);
}

B isTypeGroupBasic(L t){
    R (isTypeGroupReal(t) || H_C==t || H_Q==t || isTypeGroupDTime(t));
}

B isTypeGroupColumn(L t){
    R (isTypeGroupReal(t) || H_Q==t || isTypeGroupDTime(t));
}

B isTypeGroupAdvanced(L t){
    R 1;
}

B isTypeGroupAll(L t){
    R (isTypeGroupBasic(t) || isTypeGroupAdvanced(t));
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
