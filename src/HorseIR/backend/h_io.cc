#include "h_global.h"

const L LINE_MAX_CHAR = 1024;
const C LINE_SEP      = '|';
const L BUFF_SIZE     = 128;

/*
 * 1: csv
 * 2: txt (optional)
 */
// L readFile(S fileName, L op){
//  FILE *fp = openFile(fileName);
//  L status = 1;
//  switch(op){
//      case 1: status = readCSV(fp); break;        
//  }
//  fclose(fp);
//  R status;
// }

V readCSV(S fileName, L numCols, L *types, L *symList){
    FILE *fp = openFile(fileName);
    L numRow = loadCSV(fp, false, NULL, numCols, NULL);
    V x = allocTable(numCols);
    DOI(numCols, {V cell=xV(i); \
        initList(cell,2); \
        initSymbol(vV(cell,0), symList[i]); \
        initValue (vV(cell,1), types[i], numRow);})

    if(H_DEBUG) P("** Done with initialization **\n");
    rewind(fp);
    loadCSV(fp, true, x, numCols, types);
    if(H_DEBUG) {printTable(x); P("\n");}
    fclose(fp);
    R x;
}

L loadCSV(FILE *fp, B isLoading, V table, L numCols, L *types){
    C line[LINE_MAX_CHAR];
    L rowSize = 0, rowID = 0;
    L errCode = 0;
    while(fgets(line, LINE_MAX_CHAR, fp)){
        if(STRING_NONEMPTY(line)){
            if(isLoading){
                getField(line, LINE_SEP, table, rowSize, types, &errCode);
            }
            rowSize++;
        }
        rowID++;
    }
    if(errCode != 0){
        fprintf(stderr, "readCSV error at line %lld\n", rowID);
        exit(ERROR_CODE);
    }
    if(!isLoading){
        P(">> CSV info: %lld rows and %lld cols\n", rowSize, numCols);
    }
    else {
        va(table).row = rowSize;
        va(table).col = numCols;
    }
    R rowSize;
}

L getField(S line, C sp, V x, L rowID, L *types, L *errCode){
    C tmp[LINE_MAX_CHAR];
    S lineT = trim(line);
    L cnt = 0, numCols = 0, strLen = strlen(lineT);
    // P("line[%lld] = %s", rowID,lineT); printType(xp); P("\n");
    DOI(strLen, {\
        if(sp==lineT[i]){\
            tmp[cnt]=0; trimSelf(tmp); \
            loadItem(getColVal(getTableCol(x,numCols)),rowID,types[numCols],tmp);\
            cnt=0;numCols++;}\
        else{tmp[cnt++]=lineT[i];}})
    R numCols;
}

FILE* openFile(S s){
    FILE* fp = fopen(s, "r");
    if(NULL == fp){
        fprintf(stderr, "%s\n", strerror(errno));
        exit(ERROR_CODE);
    }
    R fp;
}

/* x[k] = (typ) s */
void loadItem(V x, L k, L typ, S s){
    switch(typ){
        caseB xB(k) = atoi(s); break;
        caseI xI(k) = atoi(s); break;
        caseL xL(k) = atol(s); break;
        caseF xF(k) = atof(s); break;
        caseE xE(k) = atof(s); break;
        caseQ xQ(k) = getSymbol(s); break;
        caseD {I a,b,c; sscanf(s,"%d-%d-%d",&a,&b,&c); xD(k) = a*10000+b*100+c;} break;
    }
}

/* helper functions */

#define SKIP(x,a) ((x)==(a))
#define SKIP_SET(c) (SKIP(c,' ') || SKIP(c, '\r') || SKIP(c, '\n'))

S trim(S s){
    R trimRight(trimLeft(s));
}

S trimLeft(S s){
    L k=0; while(s[k]==' ')k++; R s+k;
}

S trimRight(S s){
    S t = s;
    if(STRING_NONEMPTY(s)){
        t = s + strlen(s) - 1;
        while(t!=s && SKIP_SET(*t)) t--;
        *(t+(!SKIP_SET(*t)))=0;
    }
    R s;
}

S trimSelf(S s){
    S t = trim(s);
    if(s != t) {
        strcpy(s, t);
    }
    R s;
}

void errorMsg(S msg){
    fprintf(stderr, "%s\n", msg);
    exit(ERROR_CODE);
}

/* output */

#define FT(s,x) FP(stdout,s,x)
#define FS(x)   FT("%s",x)

L getTypeStr(L x, S buff){
    L c = 0; B op = true;
    switch(x){
        caseB c=SP(buff, "bool");          break;
        caseH c=SP(buff, "i16");           break;
        caseI c=SP(buff, "i32");           break;
        caseL c=SP(buff, op?"":"i64");     break;
        caseF c=SP(buff, "f32");           break;
        caseE c=SP(buff, op?"":"f64");     break;
        caseQ c=SP(buff, op?"":"sym");     break;
        caseS c=SP(buff, "str");           break;
        caseX c=SP(buff, op?"":"complex"); break;
        caseM c=SP(buff, "m");             break;
        caseD c=SP(buff, "d");             break;
        caseZ c=SP(buff, "z");             break;
        caseU c=SP(buff, "u");             break;
        caseW c=SP(buff, "w");             break;
        caseT c=SP(buff, "t");             break;
        caseG c=SP(buff, "list");          break;
        caseN c=SP(buff, "dict");          break;
        caseY c=SP(buff, "enum");          break;
        caseA c=SP(buff, "table");         break;
        caseK c=SP(buff, "ktable");        break;
        caseV c=SP(buff, "item");          break;
        default: c=SP(buff, "<unknown::%lld>",x); break;
    }
    R c;
}

L printType(L x){
    C buff[BUFF_SIZE];
    getTypeStr(x, buff);
    R FS(buff);
}

L getComplexStr(X x, S buff){
    L c0=SP(buff, "%g",xReal(x)),c1;
    if(xImag(x)<0){
        c1=SP(buff+c0, "%gi",xImag(x));
    }
    else if(xImag(x)>0){
        c1=SP(buff+c0, "+%gi",xImag(x));
    }
    R c0+c1;
}

/*
 * hasTick: true (default)
 */
L getBasicItemStr(V x, L k, S buff, B hasTick){
    L c = 0;
    switch(xp){
        caseB c=SP(buff, "%d"   , xB(k));   break;
        caseH c=SP(buff, "%d"   , xH(k));   break;
        caseI c=SP(buff, "%d"   , xI(k));   break;
        caseL c=SP(buff, "%lld" , xL(k));   break;
        caseF c=SP(buff, "%g"   , xF(k));   break;
        caseE c=SP(buff, "%.2lf", xE(k));   break;
        caseX c=getComplexStr(xX(k),buff); break;
        caseQ c=hasTick? \
                printSymTick(xQ(k), buff): \
                printSymbol(xQ(k), buff);  break;
        /* date time */
        caseM {I m=xM(k);
               c=SP(buff, "%d.%02d", \
                    CHOPM(0,m),CHOPM(1,m)); } break;
        caseD {I d=xD(k);
               c=SP(buff, "%d.%02d.%02d", \
                   CHOPD(0,d),CHOPD(1,d),CHOPD(2,d)); } break;
        caseZ {L d=Z2D(xZ(k)),t=Z2T(xZ(k)), ll=t%1000, w=t/1000;
               c=SP(buff, "%lld.%02lld.%02lldT\
                   %02lld:%02lld:%02lld.%03lld",\
                   CHOPD(0,d),CHOPD(1,d),CHOPD(2,d),
                   CHOPW(0,w),CHOPW(1,w),CHOPW(2,w),ll); } break;
        caseU {I u=xU(k);
               c=SP(buff,"%02d:%02d", \
                   CHOPU(0,u),CHOPU(1,u)); } break;
        caseW {I w=xW(k);
               c=SP(buff,"%02d:%02d:%02d", \
                   CHOPW(0,w),CHOPW(1,w),CHOPW(2,w));} break;
        caseT {I t=xT(k),ll=t%1000, w=t/1000;
               c=SP(buff,"%02d:%02d:%02d.%03d", \
                   CHOPW(0,w),CHOPW(1,w),CHOPW(2,w),ll); } break;
    }
    R c;
}

L printBasicItem(V x, L k){
    if(xp!=H_S){
        C buff[BUFF_SIZE];
        getBasicItemStr(x,k,buff,1);
        R FS(buff);
    }
    else {
        R FT("\"%s\"",xS(k));
    }
}

L printInfo(V x){
    FS("{["); printType(xp); FT("*%lld*",xn); FS("]}"); R 0;
}

L printBasicValue(V x, B hasTag){
    if(xn==1) printBasicItem(x,0);
    else {
        FS("("); DOI(xn, {if(i>0)FS(","); printBasicItem(x,i);}); FS(")");
    }
    if(hasTag) {
        FS(":"); printType(xp);
    }
    R 0;
}

L getStringItemStr(V x, L k){
    R FT("'%s'",xS(k));
}

L printStr(V x){
    getStringItemStr(x,0);
    FS("(");
    DOI(xn, {if(i>0)FS(","); getStringItemStr(x,i);})
    FS("):str"); R 0;
}

L printList(V x){
    FS("(");
    DOI(xn, {if(i>0)FS(","); printValue(xV(i));})
    FS("):list"); R 0;
}

L printDict(V x){
    FS("{");
    DOI(xn, {printValue(vV(xV(i),0)); FS(" -> "); printValue(vV(xV(i),1));})
    FS("}"); R 0;
}

L printEnum(V x){
    FS("<");
    FS(getSymbolStr(getEnumName(x)));
    FS(",");
    /* print enum */
    if(xn==1) printBasicItem((V)getEnumTarget(x),vY(x,0));
    else {
        FS("(");
        DOI(xn, {if(i>0)FS(","); printBasicItem((V)getEnumTarget(x),vY(x,i));})
        FS(")");
    }
    FS(">"); R 0;
}

L printValue(V x){
    if(H_DEBUG) printInfo(x);
    if(isTypeGroupBasic(xp)){
        printBasicValue(x, true);
    }
    else {
        switch(xp){
            caseG printList(x);   break;
            caseN printDict(x);   break;
            caseY printEnum(x);   break;
            caseA printTable(x);  break;
            caseK printKTable(x); break;
            default: R E_DOMAIN;
        }
    }
    R 0;
}

L printV(V x){
    printValue(x); FS("\n"); R 0;
}

L printTable(V x){
    printTablePretty(x,-1);
    R 0;
}

L printKTable(V x){
    printTablePretty(x,-1);
    R 0;
}

/* pretty print */

L printTablePretty(V x, L rowLimit){
    C buff[BUFF_SIZE];
    if(isTable(x)){
        L totSize = 0;
        L *colWidth = (L*)malloc(sizeof(L) * vn(x));
        DOI(vn(x), colWidth[i]=getColWidth(getTableCol(x,i)))
        DOI(vn(x), totSize+=colWidth[i]); totSize += vn(x);
        // DOI(vn(x), P("[%lld] %lld\n",i,colWidth[i]))
        /* print head */
        DOI(vn(x), {if(i>0) FS(" "); V key = getColKey(getTableCol(x,i)); \
            printSymbol(vq(key),buff); getStrPretty(buff,colWidth[i]); FT("%s",buff); })
        FS("\n");
        DOI(totSize-1, FS("-"));
        FS("\n");
        /* print body */
        L rowPrint = rowLimit<0?getTableRowNumber(x):rowLimit;
        DOI(rowPrint,  { \
            DOJ(vn(x), {if(j>0) FS(" "); V val = getColVal(getTableCol(x,j)); \
                getBasicItemStr(val,i,buff,0); getStrPretty(buff,colWidth[j]); FT("%s",buff);}) \
            FS("\n"); \
            })
        FS("\n");
        free(colWidth);
    }
    else if(isKTable(x)){
        V key = getKTableKey(x);
        V val = getKTableVal(x);
        L *colWidthKey = (L*)malloc(sizeof(L) * vn(key));
        L *colWidthVal = (L*)malloc(sizeof(L) * vn(val));
        L totSize1 = 0, totSize2 = 0;
        DOI(vn(key), colWidthKey[i]=getColWidth(getTableCol(key,i)))
        DOI(vn(key), totSize1+=colWidthKey[i]); totSize1 += vn(key);
        DOI(vn(val), colWidthVal[i]=getColWidth(getTableCol(val,i)))
        DOI(vn(val), totSize2+=colWidthVal[i]); totSize2 += vn(val);
        /* print head */
        DOI(vn(key),{if(i>0) FS(" "); V t=getColKey(getTableCol(key,i)); \
            printSymbol(vq(t),buff); getStrPretty(buff,colWidthKey[i]); FT("%s",buff); })
        FS("|");
        DOI(vn(val),{V t=getColKey(getTableCol(val,i)); \
            printSymbol(vq(t),buff); getStrPretty(buff,colWidthVal[i]); FT(" %s",buff); })
        FS("\n");
        DOI(totSize1-1, FS("-"));
        FS("| ");
        DOI(totSize2-1, FS("-"));
        FS("\n");
        /* print body */
        L rowPrint = rowLimit<0?getTableRowNumber(x):rowLimit;
        DOI(rowPrint, {\
             DOJ(vn(key), {if(j>0) FS(" "); V t=getColVal(getTableCol(key,j)); \
                 getBasicItemStr(t,i,buff,0); getStrPretty(buff,colWidthKey[j]); FT("%s",buff);}) \
             FS("|"); \
             DOJ(vn(val), {V t=getColVal(getTableCol(val,j)); \
                 getBasicItemStr(t,i,buff,0); getStrPretty(buff,colWidthVal[j]); FT(" %s",buff);}) \
             FS("\n"); \
            })
        FS("\n");
        free(colWidthKey);
        free(colWidthVal);
    }
    else{
        P("<Not a table/ktable> - From printTablePretty\n");
    }
    R 0;
}

L getColWidth(V x){
    V key = getColKey(x);
    V val = getColVal(x);
    C buff[BUFF_SIZE];
    L maxSize = getSymbolSize(vq(key));
    DOI(vn(val), {L t=getBasicItemStr(val,i,buff,0); if(t>maxSize)maxSize=t;})
    R maxSize;
}

L getStrPretty(S str, L maxSize){
    L len = strlen(str);
    while(len < maxSize) {
        str[len++] = ' ';
    }
    if(len == maxSize) {
        str[len] = 0;
    }
    R 0;
}

