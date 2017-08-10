#include "h_global.h"

const L LINE_MAX_CHAR = 1024;
const C LINE_SEP      = '|';

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
    DOI(numCols, {V newDict=xV(i); \
        initDict(newDict); \
        initSymbol(vV(newDict, 0), symList[i]); \
        initValue (vV(newDict, 1), types[i], numRow);});

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
    if(!isLoading)
        P(">> CSV info: %lld rows and %lld cols\n", rowSize, numCols);
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
            loadItem(getDictVal(getTableDict(x,numCols)),rowID,types[numCols],tmp);\
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
        caseQ xQ(k) = insertSym(createSymbol(s)); break;
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
        *(t+(t!=s))=0;
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

void printListItem(V x, L k, S strBuff){
    switch(xp){
        caseB SP(strBuff, "%d"  , xB(k)); break;
        caseH SP(strBuff, "%d"  , xH(k)); break;
        caseI SP(strBuff, "%d"  , xI(k)); break;
        caseL SP(strBuff, "%lld", xL(k)); break;
        caseF SP(strBuff, "%f",   xF(k)); break;
        caseE SP(strBuff, "%lf" , xE(k)); break;
        caseX SP(strBuff, "%g+%gi" , xReal(xX(k)),xImag(xX(k))); break;
        caseQ printSymbol(xQ(k), strBuff);break;
        caseA DOI(xn, {printListItem(xG(i),i,strBuff);}) return;
    }
    P(" %s", strBuff);
}

void printHead(V x){
    P("{["); printType(xp); P("]");
}

void printList(V x){
    C buff[128];
    printHead(x); P("*%lld*",xn);
    DOI(xn, printListItem(x,i,buff));
    P("}");
}

void printDict(V x){
    if(isDict(x)){
        printHead(x);
        printList(xV(0));
        P(",");
        printList(xV(1));
        P("}");
    }
    else{
        P("<Not a dictionary>");
    }
}

void printTable(V x){
    if(isTable(x)){
        printHead(x);
        DOI(xn, {if(i>0)P(","); printDict(xV(i));})
        P("}");
    }
    else {
        P("<Not a table>\n");
    }
}

/* pretty print */

void printTablePretty(V x, L rowLimit){
    if(isTable(x)){
        L *colWidth = (L*)malloc(sizeof(L) * vn(x));
        L totSize = 0;  C buff[99];
        DOI(vn(x), colWidth[i]=getColWidth(getTableDict(x,i)))
        DOI(vn(x), totSize+=colWidth[i]); totSize += vn(x);
        // DOI(vn(x), P("[%lld] %lld\n",i,colWidth[i]))
        /* print head */
        DOI(vn(x), {V d=getTableDict(x,i); V key = getDictKey(d); \
            printSymbol(vq(key),buff); printStrPretty(buff,colWidth[i]); P("%s|",buff); })
        P("\n");
        DOI(totSize, P("-"));
        P("\n");
        /* print body */
        L rowPrint = rowLimit<0?getTableRowNumber(x):rowLimit;
        DOI(rowPrint,
            {DOJ(vn(x), {V d=getTableDict(x,j); V val = getDictVal(d); \
                getListInfo(val,i,buff); printStrPretty(buff,colWidth[j]); P("%s|",buff);})
            P("\n");})
        P("\n");
        free(colWidth);
    }
    else{
        P("<Not a tbale> - From printTablePretty\n");
    }
}

void printKTablePretty(V x, L rowLimit){
}

void printEnumPretty(V x){
}

/* JSON like */
void printDictPretty(V x){
}

L getColWidth(V x){
    V key = getDictKey(x);
    V val = getDictVal(x);
    C buff[99];
    L maxSize = getSymbolSize(vq(key));
    DOI(vn(val), {L t=getListInfo(val,i,buff); if(t>maxSize)maxSize=t;})
    R maxSize;
}

L getListInfo(V x, L k, S strBuff){
    switch(xp){
        caseB SP(strBuff, "%d"  , xB(k)); break;
        caseI SP(strBuff, "%d"  , xI(k)); break;
        caseL SP(strBuff, "%lld", xL(k)); break;
        caseE SP(strBuff, "%lf" , xF(k)); break;
        caseQ printSymbol(xQ(k), strBuff);break;
        default: P("Error in getListInfo: type %lld\n",xp); exit(99);
    }
    R strlen(strBuff);
}


void printStrPretty(S str, L maxSize){
    L len = strlen(str);
    while(len < maxSize) {
        str[len++] = ' ';
    }
    if(len == maxSize) {
        str[len] = 0;
    }
}

