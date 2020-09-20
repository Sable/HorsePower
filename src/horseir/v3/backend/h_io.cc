#include "../global.h"
#include <fcntl.h>     /* open, mmap */
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

L LINE_MAX_CHAR = 1024;
C LINE_SEP      = '|';
L BUFF_SIZE     = 256;
B LINE_HEADER   = false;  // true: ignore the 1st row
const L OUTPUT_SIZE = 100;

#define ERROR_CODE 99

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

static O *openMMapFile(S s){
    L fd;
    void* data;
    struct stat sbuf;
    if((fd=open(s, O_RDONLY))==-1){
        fprintf(stderr, "Can't find file %s\n", s);
        exit(ERROR_CODE);
    }
    if(stat(s,&sbuf)==-1){
        fprintf(stderr, "File %s state not available\n", s);
        exit(ERROR_CODE);
    }
    data = mmap(NULL, sbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(data == MAP_FAILED){
        fprintf(stderr, "mmap error\n");
        exit(ERROR_CODE);
    }
    R data;
}

V readCSV(S fileName, L numCols, L *types, Q *symList){
#ifdef USE_MMAP
    S fp = (S)openMMapFile(fileName);
#else
    FILE *fp = openFile(fileName);
#endif
    L numRow = loadCSV(fp, false, NULL, numCols, NULL);
    V x = allocTable(numCols);
    DOI(numCols, {V key=getTableKeys(x); vQ(key,i)=symList[i]; \
        V val=getTableVals(x); initValue(getTableCol(val,i), types[i], numRow);})    
    if(H_DEBUG) WP("** Done with initialization **\n");
#ifndef USE_MMAP
    rewind(fp);
#endif
    WP("loading data\n");
    loadCSV(fp, true, x, numCols, types);
    //if(H_DEBUG) {printTablePretty(x,20); WP("\n");} // input preview
#ifndef USE_MMAP
    fclose(fp);
#endif
    R x;
}

L mgets(S line, L maxSize, S data){
    S temp = data;
    while(*temp!='\n' && *temp!='\0') temp++;
    L k = temp - data;
    if(k>=maxSize) {
        fprintf(stderr, "A line should no longer than %lld.\n", maxSize);
        exit(ERROR_CODE);
    }
    memcpy(line,data,k);
    line[k]=0;
    R k;
}

L loadCSV(void *fp, B isLoading, V table, L numCols, L *types){
    C line[LINE_MAX_CHAR];
    L rowSize = 0, rowID = 0;
    L errCode = 0;
#ifdef USE_MMAP
    L lineSize=0;
    S fp_data = (S)fp;
    if(LINE_HEADER && mgets(line, LINE_MAX_CHAR, fp_data))
        ;
    while(lineSize=mgets(line, LINE_MAX_CHAR, fp_data)){
        fp_data += lineSize;
        while(*fp_data == '\r' || *fp_data == '\n') fp_data++;
#else
    if(LINE_HEADER && fgets(line, LINE_MAX_CHAR, (FILE*)fp))
        ;
    while(fgets(line, LINE_MAX_CHAR, (FILE*)fp)){
#endif
        if(STRING_NONEMPTY(line)){
            if(isLoading){
                getField(line, LINE_SEP, table, rowSize, types, &errCode);
            }
            rowSize++;
        }
        rowID++;
    }
    if(errCode != 0){
        EP("Error at line %lld", rowID);
    }
    if(!isLoading){
        WP(">> CSV info: %lld rows and %lld cols\n", rowSize, numCols);
    }
    else {
        tableRow(table) = rowSize;
        tableCol(table) = numCols;
    }
    R rowSize;
}

L getField(S line, C sp, V x, L rowID, L *types, L *errCode){
    C tmp[LINE_MAX_CHAR];
    S lineT = trim(line);
    L cnt = 0, numCols = 0, strLen = strlen(lineT);
    V val = getTableVals(x);
    //WP("line[%lld] = %s", rowID,lineT); printType2(xp); WP("\n");
    DOI(strLen, {\
        if(sp==lineT[i]){\
            tmp[cnt]=0; trimSelf(tmp); \
            loadItem(getTableCol(val,numCols),rowID,types[numCols],tmp);\
            cnt=0;numCols++;}\
        else{tmp[cnt++]=lineT[i];}})
    if(' '==sp){ // remainder
        tmp[cnt]=0; trimSelf(tmp);
        loadItem(getTableCol(val,numCols),rowID,types[numCols],tmp);
        cnt=0; numCols++;
    }
    R numCols;
}

FILE *openFile(S s){
    FILE* fp = fopen(s, "r");
    if(NULL == fp){
        fprintf(stderr, "%s\n", strerror(errno));
        exit(ERROR_CODE);
    }
    R fp;
}

#define ATOF(x) strtof(x, NULL)
#define ATOE(x) atof(x) /* atof -> double */

/* x[k] = (typ) s */
O loadItem(V x, L k, L typ, S s){
    switch(typ){
        caseB xB(k) = atoi(s); break;
        caseJ xJ(k) = atoi(s); break;
        caseH xH(k) = atoi(s); break;
        caseI xI(k) = atoi(s); break;
        caseL xL(k) = atol(s); break;
        caseF xF(k) = ATOF(s); break;
        caseE xE(k) = ATOE(s); break; 
        caseC xC(k) = s[0];    break;
        caseQ xQ(k) = getSymbol(s); break;
        caseS {S t=allocStrMem(strlen(s)); strcpy(t,s); xS(k)=t;} break;
        caseD {I a,b,c; sscanf(s,"%d-%d-%d",&a,&b,&c); xD(k) = a*10000+b*100+c;} break;
        default: WP("Loaditem type exception.\n"); break;
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
        //strcpy(s, t);  //runtime error??
        L n = t - s; DOI(strlen(t)+1, s[i]=t[i])
    }
    R s;
}

O errorMsg(S msg){
    fprintf(stderr, "%s\n", msg);
    exit(ERROR_CODE);
}

/* output */

L getTypeStr(L x, S buff){
    L c = 0;
    switch(x){
        caseB c=SP(buff, "bool");          break;
        caseJ c=SP(buff, "i8");            break;
        caseH c=SP(buff, "i16");           break;
        caseI c=SP(buff, "i32");           break;
        caseL c=SP(buff, "i64");           break;
        caseF c=SP(buff, "f32");           break;
        caseE c=SP(buff, "f64");           break;
        caseQ c=SP(buff, "sym");           break;
        caseC c=SP(buff, "char");          break;
        caseS c=SP(buff, "str");           break;
        caseX c=SP(buff, "complex");       break;
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

L printType2(L x){
    C buff[BUFF_SIZE];
    getTypeStr(x, buff);
    R FS(buff);
}

L printTag(L x){
    C buff[BUFF_SIZE];
    switch(x){
        caseL caseE caseQ caseX
            buff[0]=0; break;
        default:
            buff[0]=':';
            getTypeStr(x, buff+1); break;
    }
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
        caseJ c=SP(buff, "%d"   , xJ(k));   break;
        caseH c=SP(buff, "%d"   , xH(k));   break;
        caseI c=SP(buff, "%d"   , xI(k));   break;
        caseL c=SP(buff, "%lld" , xL(k));   break;
        caseF c=SP(buff, "%f"   , xF(k));   break;
        caseE c=SP(buff, "%lf"  , xE(k));   break;
        caseX c=getComplexStr(xX(k),buff);  break;
        /* deal with caseC and caseS carefully */
        caseQ c=hasTick? \
                printSymTick(xQ(k), buff): \
                printSymbol(xQ(k), buff);   break;
        caseS {S t=xS(k); c=strlen(t); \
               if(BUFF_SIZE<=c) { EP("Buff exceeds"); }\
               strcpy(buff,t); }            break;
        caseC c=SP(buff, "%c", xC(k));      break;
        /* date time */
        caseM {I m=xM(k);
               c=SP(buff, "%d.%02d", \
                    CHOPM(0,m),CHOPM(1,m)); }              break;
        caseD {I d=xD(k);
               c=SP(buff, "%d.%02d.%02d", \
                   CHOPD(0,d),CHOPD(1,d),CHOPD(2,d)); }    break;
        caseZ {L d=Z2D(xZ(k)),t=Z2T(xZ(k)), ll=t%1000, w=t/1000;
               c=SP(buff, "%lld.%02lld.%02lldT"\
                   "%02lld:%02lld:%02lld.%03lld",\
                   CHOPD(0,d),CHOPD(1,d),CHOPD(2,d),\
                   CHOPW(0,w),CHOPW(1,w),CHOPW(2,w),ll); } break;
        caseU {I u=xU(k);
               c=SP(buff,"%02d:%02d", \
                   CHOPU(0,u),CHOPU(1,u)); }               break;
        caseW {I w=xW(k);
               c=SP(buff,"%02d:%02d:%02d", \
                   CHOPW(0,w),CHOPW(1,w),CHOPW(2,w));}     break;
        caseT {I t=xT(k),ll=t%1000, w=t/1000;
               c=SP(buff,"%02d:%02d:%02d.%03d", \
                   CHOPW(0,w),CHOPW(1,w),CHOPW(2,w),ll); } break;
        caseG c=SP(buff, "%lldL", k);                      break;
        caseY c=getBasicItemStr(\
                   (V)getEnumKey(x),vY(x,k),buff,0);    break;
        default: EP("Unexpected type : %s", getTypeName(xp));  break;
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

I printInfo(V x){
    FS("{["); printType2(xp); FT("*%lld*",xn); FS("]}"); R 0;
}

I printBasicValue(V x, L k, B hasTag){
    if(0>k){
        if(xn==1) printBasicItem(x,0);
        else {
            L size= xn>OUTPUT_SIZE?OUTPUT_SIZE:xn;
            FS("(");
            DOI(size, {if(i>0)FS(","); printBasicItem(x,i);});
            if(xn > OUTPUT_SIZE) FT(",<... %lld more>", xn-OUTPUT_SIZE);
            FS(")");
        }
    }
    else {
        if(k<xn) printBasicItem(x,k);
        else R E_INDEX;
    }
    if(hasTag) {
        printTag(xp);
    }
    R 0;
}

L getStringItemStr(V x, L k){
    R FT("'%s'",xS(k));
}

I printStrItem(V x, L k){
    getStringItemStr(x,0);
    FS("(");
    if(k<0) { DOI(xn, {if(i>0)FS(","); getStringItemStr(x,i);}) }
    else { getStringItemStr(x,k); }
    FS("):str"); R 0;
}

static L printListFlatItem(V x, L k){
    L a0=0;
    FS("(");
    if(k<0) {
        DOI(xn, {if(i>0)FS(","); \
            V g2=vg2(x); L a1=a0+vL(x,i); \
            FS("("); printV3(g2,a0,a1,false); FS(")"); a0=a1;})
    }
    else { EP("Pending flat list print"); }
    FS("):list"); R 0;
}

static L printListNormalItem(V x, L k){
    FS("(");
    if(k<0) { DOI(xn, {if(i>0)FS(","); printValue(xV(i));}) }
    else { printValue(xV(k)); }
    FS("):list"); R 0;
}

O printListItem(V x, L k){
    if(isListFlat(x)) printListFlatItem(x, k);
    else printListNormalItem(x, k);
}

I printDictItem(V x, L k){
    FS("{");
    V key = getDictKeys(x), val = getDictVals(x);
    if(k<0) { DOI(dictNum(x), {if(i>0)FS(",\n"); printValueItem(key,i); FS(" -> "); printValueItem(val,i);})}
    else if(k<dictNum(x)){ printValueItem(key,k); FS(" -> "); printValueItem(val,k); }
    else R E_INDEX;
    FS("}"); R 0;
}

I printEnumItem(V x, L k){
    FS("<");
    if(getEnumName(x)<0) FS("<Enum>");
    else FS(getSymbolStr(getEnumName(x)));
    FS(",");
    /* print enum */
    if(k<0){
        if(xn==1) printBasicItem((V)getEnumKey(x),vY(x,0));
        else {
            FS("(");
            DOI(xn, {if(i>0)FS(","); printBasicItem((V)getEnumKey(x),vY(x,i));})
            FS(")");
        }
    }
    else {
        if(k<xn) printBasicItem((V)getEnumKey(x),vY(x,k));
        else R E_INDEX;
    }
    FS(">"); R 0;
}

I printValueItem(V x, L k){
    if(H_DEBUG) printInfo(x);
    if(isTypeGroupBasic(xp)){
        printBasicValue(x, k, true);
    }
    else {
        switch(xp){
            caseG printListItem(x,k);   break;
            caseN printDictItem(x,k);   break;
            caseY printEnumItem(x,k);   break;
            caseA printTable(x);  break;
            caseK printKTable(x); break;
            default: R E_DOMAIN;
        }
    }
    R 0;
}

I printV(V x){
    printValue(x); FS("\n"); R 0;
}

I printV2(V x, L n){
    printV3(x, 0, n, true); R 0;
}

I printV3(V x, L k0, L k1, B isR){
    switch(xp){
        caseA printTablePretty(x, k1); break;
        caseK printTablePretty(x, k1); break;
        caseG printListItem(x, -1);    break;
        default:
        if(xn < k1) printV(x);
        else { B f = 0;
          DOI3(k0, k1, {if(i>k0)FS(" ");if(printValueItem(x, i)){f=1;break;};});
          if(f) EP("Type is not supported: %s", getTypeName(xp)); } break;
    }
    if(isR) FS("\n"); R 0;
}

I printTable(V x){
    printTablePretty(x,-1);
    R 0;
}

I printKTable(V x){
    printTablePretty(x,-1);
    R 0;
}

/* pretty print */

I printTablePretty(V x, L rowLimit){
    C buff[BUFF_SIZE];
    if(isTable(x)){
        WP("\nTable: row = %lld, col = %lld\n", tableRow(x),tableCol(x));
        L totSize = 0;
        L *colWidth = (L*)malloc(sizeof(L) * tableCol(x));
        L rows=tableRow(x), rowPrint = rowLimit<0?rows:rowLimit>rows?rows:rowLimit;
        DOI(tableCol(x), colWidth[i]=getColWidth(x,i,rowPrint))
        DOI(tableCol(x), totSize+=colWidth[i]); totSize += tableCol(x);
        // DOI(vn(x), WP("[%lld] %lld\n",i,colWidth[i]))
        /* print head */
        DOI(tableCol(x), {if(i>0) FS(" "); V key=getTableKeys(x); \
            printSymbol(vQ(key,i),buff); getStrPretty(buff,colWidth[i]); FT("%s",buff);})
        FS("\n");
        DOI(totSize-1, FS("-"));
        FS("\n");
        // test the type of each column
        // DOI(tableCol(x), {V val = getTableCol(getTableVals(x),i); WP("type[%lld] = %lld\n",i,vp(val));})
        /* print body */
        DOI(rowPrint,  { \
            DOJ(tableCol(x), {if(j>0) FS(" "); V val = getTableCol(getTableVals(x),j); \
                getBasicItemStr(val,i,buff,0); getStrPretty(buff,colWidth[j]); FT("%s",buff);}) \
            FS("\n"); \
            })
        FS("\n");
        free(colWidth);
    }
    else if(isKTable(x)){
        WP("\n KTable: row = %lld, col = %lld\n", tableRow(x),tableCol(x));
        V key = getKTableKey(x);
        V val = getKTableVal(x);
        L *colWidthKey = (L*)malloc(sizeof(L) * tableCol(key));
        L *colWidthVal = (L*)malloc(sizeof(L) * tableCol(val));
        L rows=tableRow(x), rowPrint = rowLimit<0?rows:rowLimit>rows?rows:rowLimit;
        L totSize1 = 0, totSize2 = 0;
        DOI(tableCol(key), colWidthKey[i]=getColWidth(key,i,rowPrint))
        DOI(tableCol(key), totSize1+=colWidthKey[i]); totSize1 += tableCol(key);
        DOI(tableCol(val), colWidthVal[i]=getColWidth(val,i,rowPrint))
        DOI(tableCol(val), totSize2+=colWidthVal[i]); totSize2 += tableCol(val);
        /* print head */
        DOI(tableCol(key),{if(i>0) FS(" "); V t=getTableKeys(key); \
            printSymbol(vQ(t,i),buff); getStrPretty(buff,colWidthKey[i]); FT("%s",buff); })
        FS("|");
        DOI(tableCol(val),{V t=getTableKeys(val); \
            printSymbol(vQ(t,i),buff); getStrPretty(buff,colWidthVal[i]); FT(" %s",buff); })
        FS("\n");
        DOI(totSize1-1, FS("-"));
        FS("| ");
        DOI(totSize2-1, FS("-"));
        FS("\n");
        /* print body */
        DOI(rowPrint, {\
             DOJ(tableCol(key), {if(j>0) FS(" "); V t=getTableCol(getTableVals(key),j); \
                 getBasicItemStr(t,i,buff,0); getStrPretty(buff,colWidthKey[j]); FT("%s",buff);}) \
             FS("|"); \
             DOJ(tableCol(val), {V t=getTableCol(getTableVals(val),j); \
                 getBasicItemStr(t,i,buff,0); getStrPretty(buff,colWidthVal[j]); FT(" %s",buff);}) \
             FS("\n"); \
            })
        FS("\n");
        free(colWidthKey);
        free(colWidthVal);
    }
    else{
        EP("Not a table/ktable");
    }
    R 0;
}

#define TABLE_CELL_MAX 30
#define TABLE_CELL_DOT 3   //...

/*
 * x: table
 * k: index
 * rowLimit: limit row
 */
L getColWidth(V x, L k, L rowLimit){
    V key = getTableKeys(x);
    V val = getTableVals(x);
    V valK= vV(val,k);
    C buff[BUFF_SIZE];
    L maxSize = getSymbolSize(vQ(key,k));
    DOI(rowLimit, {L t=getBasicItemStr(valK,i,buff,0); if(t>maxSize)maxSize=t;})
    R maxSize>TABLE_CELL_MAX?TABLE_CELL_MAX:maxSize;
}

I getStrPretty(S str, L maxSize){
    L len = strlen(str);
    L maxLen = TABLE_CELL_MAX - TABLE_CELL_DOT;
    if(len <= maxLen){
        while(len < maxSize) {
            str[len++] = ' ';
        }
        str[maxSize] = 0;
    }
    else {
        DOI(TABLE_CELL_DOT, str[maxLen+i]='.')
        str[maxSize]=0;
    }
    R 0;
}

/* TODO: csv(0)/xml(1)/json(2) */

static O printCSV(V x){;}
static O printXML(V x){;}
//static void printJSON(V x){;}

O printFormat(V x, I op){
    switch(op){
        case 0: printCSV(x); break;
        case 1: printXML(x); break;
        //case 2: printJSON(x);break; // TODO
        default: EP("Format not supported: %d", op);
    }
}

/* read a matrix */

//const L LINE_SPECIAL_MAX = 29999;
#define LINE_SPECIAL_MAX 29999LL
C line_special[LINE_SPECIAL_MAX];

V readMatrix(S fileName){
    FILE *fp = openFile(fileName);
    L lineNo = 0, numRows, numCols;
    V x = allocNode();
    while(fgets(line_special, LINE_SPECIAL_MAX, (FILE*)fp)){
        if(0 == lineNo){
            sscanf(line_special, "%lld %lld", &numRows, &numCols);
            initList(x, numRows);
            WP("Loading %s: %lld, %lld\n", fileName, numRows, numCols);
        }
        else {
            S ptr = line_special;
            V y = vV(x,lineNo-1);  initV(y, H_L, numCols);
            DOI(numCols, {L k=sscanf(ptr, "%lld", sL(y)+i); while(ptr[k]==' ')k++; ptr+=k;})
        }
        lineNo++;
    }
    fclose(fp);
    R x;
}

/* serialization */

#define serializeV1(x)  fwrite(x,sizeof(V0),1,fp)
#define serializeV1A(x) fwrite(va(x),sizeof(A0),1,fp)
#define serializeV1Y(x) fwrite(vy(x),sizeof(Y0),1,fp)

static O serializeStr(S x, L n, FILE *fp){
    fwrite(x, 1, n + 1, fp);
}

static O serializeInt(L x, FILE *fp){
    fwrite(&x, sizeof(L), 1, fp);
}

static O serializeBasic(V x, FILE *fp){
    L size = getTypeSize(xp, xn);
    fwrite(xg, 1, size, fp);
}

static O serializeQ(V x,FILE *fp){
    DOI(xn, serializeInt(getSymbolSize(vQ(x,i)), fp));
    DOI(xn, serializeStr(getSymbolStr(vQ(x,i)), getSymbolSize(vQ(x,i)), fp));
}

static O serializeS(V x,FILE *fp){
    //DOI(xn, WP(" %lld",strlen(vS(x,i)))); WP("\n");
    DOI(xn, serializeInt(strlen(vS(x,i)), fp));
    DOI(xn, serializeStr(vS(x,i), strlen(vS(x,i)), fp));
}

static O serializeG(V x, FILE *fp){
    DOI(xn, {V val=vV(x,i);serializeV(val, fp);})
}

static O serializeA(V x, FILE *fp){
    serializeV1A(x);
    V keyV = getTableKeys(x);
    V valV = getTableVals(x);
    /* write keys */
    serializeV(keyV, fp);  // symbol
    /* write columns */
    serializeV(valV, fp);  // list
}

O serializeV(V x, FILE *fp){
    serializeV1(x);
    // check if size > 1 => xg is not NULL
    switch(xp){
        caseB serializeBasic(x, fp); break;
        caseJ serializeBasic(x, fp); break;
        caseH serializeBasic(x, fp); break;
        caseI serializeBasic(x, fp); break;
        caseL serializeBasic(x, fp); break;
        caseF serializeBasic(x, fp); break;
        caseE serializeBasic(x, fp); break;
        caseC serializeBasic(x, fp); break;
        caseQ serializeQ    (x, fp); break;
        caseS serializeS    (x, fp); break;
        caseM serializeBasic(x, fp); break;
        caseD serializeBasic(x, fp); break;
        caseZ serializeBasic(x, fp); break;
        caseU serializeBasic(x, fp); break;
        caseW serializeBasic(x, fp); break;
        caseT serializeBasic(x, fp); break;
        caseX serializeBasic(x, fp); break;
        caseA serializeA    (x, fp); break;
        caseG serializeG    (x, fp); break;
        default: EP("Type not supported: %s", getTypeName(xp));
    }
}

// read from bin
#define readSerializeV1(x)  readDataBySize(x,sizeof(V0),1,fp)
#define readSerializeV1A(x) readDataBySize(va(x),sizeof(A0),1,fp);
#define readSerializeV1Y(x) readDataBySize(vy(x),sizeof(Y0),1,fp);

static O readDataBySize(void *ptr, L t_size, L size, FILE *fp){
    if(size != fread(ptr, t_size, size, fp)){
        EP("Fread error");
    }
}

static O readSerializeBasic(V x, FILE *fp){
    L size = getTypeSize(xp, xn);
    //switch(xp){ // no performance up
    //    caseI fread(xg, sizeof(I), size/sizeof(I), fp); break;
    //    caseL fread(xg, sizeof(L), size/sizeof(L), fp); break;
    //    caseF fread(xg, sizeof(F), size/sizeof(F), fp); break;
    //    caseE fread(xg, sizeof(E), size/sizeof(E), fp); break;
    //    caseD fread(xg, sizeof(D), size/sizeof(D), fp); break;
    //    default: fread(xg, 1, size, fp);
    //}
    readDataBySize(xg, 1, size, fp);
}

static O readSerializeQ(V x,FILE *fp){
    DOI(xn, readDataBySize(sQ(x)+i, sizeof(L), 1, fp));
    L maxSize=-1; DOI(xn, if(maxSize < vQ(x,i)) maxSize=vQ(x,i))
    S temp = (S)malloc(maxSize + 1);
    DOI(xn, {L size=vQ(x,i)+1; readDataBySize(temp, 1, size, fp); vQ(x,i)=getSymbol(temp);})
    free(temp);
}

static O readSerializeS(V x, FILE *fp){
    L *temp = (L*)malloc(sizeof(L)*(xn));
    DOI(xn, readDataBySize(temp+i, sizeof(L), 1, fp))
    DOI(xn, {S t=allocStrMem(temp[i]); readDataBySize(t,1,temp[i]+1,fp); vS(x,i)=t;})
    free(temp);
}

static O readSerializeG(V x, FILE *fp){
    DOI(xn, {V val=vV(x,i);readSerializeV(val, fp);})
}

static O readSerializeA(V x, FILE *fp){
    readSerializeV1A(x);
    V keyV = getTableKeys(x); initV(keyV, vp(keyV), vn(keyV));
    V valV = getTableVals(x); initV(valV, vp(valV), vn(valV));
    /* write keys */
    readSerializeV(keyV, fp);  // symbol
    /* write columns */
    readSerializeV(valV, fp);  // list
}

O readSerializeV(V x, FILE *fp){
    readSerializeV1(x);
    initV(x, xp, xn);
    switch(xp){
        caseB readSerializeBasic(x, fp); break; 
        caseJ readSerializeBasic(x, fp); break; 
        caseH readSerializeBasic(x, fp); break; 
        caseI readSerializeBasic(x, fp); break; 
        caseL readSerializeBasic(x, fp); break; 
        caseF readSerializeBasic(x, fp); break; 
        caseE readSerializeBasic(x, fp); break; 
        caseC readSerializeBasic(x, fp); break; 
        caseQ readSerializeQ    (x, fp); break; 
        caseS readSerializeS    (x, fp); break; 
        caseM readSerializeBasic(x, fp); break; 
        caseD readSerializeBasic(x, fp); break; 
        caseZ readSerializeBasic(x, fp); break; 
        caseU readSerializeBasic(x, fp); break; 
        caseW readSerializeBasic(x, fp); break; 
        caseT readSerializeBasic(x, fp); break; 
        caseX readSerializeBasic(x, fp); break; 
        caseA readSerializeA    (x, fp); break; 
        caseG readSerializeG    (x, fp); break; 
        default: EP("Type not supported: %s", getTypeName(xp));
    }
}

/*
 * Functions for profiling data
 */

static O profile_write(V x, S fn){
    FILE *fp = fopen(fn, "w");
    switch(xp){
        caseI
            fprintf(fp, "%lld\n", vn(x));
            DOI(vn(x), fprintf(fp, "%d\n", vI(x,i))) break;
        caseQ
            fprintf(fp, "%lld\n", vn(x));
            DOI(vn(x), fprintf(fp, "%s\n", getSymbolStr(vQ(x,i)))) break;
        default:
            getInfoVar(x);
            fprintf(fp, "%s is not integer (I)\n", fn);
    }
    fclose(fp);
}

static void profile_join_write_one(V x, L i, FILE *fp){
    switch(xp){
        caseB
            FP(fp, "%d ", vB(x,i)); break;
        caseH
            FP(fp, "%d ", vH(x,i)); break;
        caseI
            FP(fp, "%d ", vI(x,i)); break;
        caseL
            FP(fp, "%lld ", vL(x,i)); break;
        caseE
            FP(fp, "%g ", vE(x,i)); break;
        caseD
            FP(fp, "%d ", vD(x,i)); break;
        caseQ
            FP(fp, "%lld ", vQ(x,i)); break;
        caseS
            FP(fp, "\"%s\" ", vS(x,i)); break;
        default:
            getInfoVar(x);
            EP("%s is not supported\n", getTypeName(xp));
    }
}


static void print_type_alias(V x, FILE *fp){
    switch(xp){
        caseH FP(fp, "H "); break;
        caseI FP(fp, "I "); break;
        caseL FP(fp, "L "); break;
        caseE FP(fp, "E "); break;
        caseQ FP(fp, "Q "); break;
        caseS FP(fp, "S "); break;
        default: EP("Unknown type: %s\n", getTypeName(xp));
    }
}

static void profile_join_write_multiple(V x, S fn){
    FILE *fp = fopen(fn, "w");
    L size = vn(x), row = vn(vV(x,0));
    FP(fp, "%lld %lld\n", size, row);
    DOI(size, print_type_alias(vV(x,i),fp)) FP(fp, "\n");
    DOI(row, { DOJ(size, profile_join_write_one(vV(x,j),i,fp)) FP(fp,"\n"); })
    fclose(fp);
}

static O profile_join_data_single(V x, V y, V f){
    P("Profiling join with a single column: %d\n", join_id);
    C fn_x[99], fn_y[99], fn_f[99];
    sprintf(fn_x, "/tmp/d%d-left.txt" , join_id);
    sprintf(fn_y, "/tmp/d%d-right.txt", join_id);
    sprintf(fn_f, "/tmp/d%d-op.txt"   , join_id);
    if(x)
        profile_write(x, fn_x);
    if(y)
        profile_write(y, fn_y);
    if(f)
        profile_write(f, fn_f);
    join_id++;
}

static O profile_join_data_multiple(V x, V y, V f){
    P("Profiling join with multiple columns: %d\n", join_id);
    C fn_x[99], fn_y[99], fn_f[99];
    sprintf(fn_x, "/tmp/d%d-left.txt" , join_id);
    sprintf(fn_y, "/tmp/d%d-right.txt", join_id);
    sprintf(fn_f, "/tmp/d%d-op.txt"   , join_id);
    if(x)
        profile_join_write_multiple(x, fn_x);
    if(y)
        profile_join_write_multiple(y, fn_y);
    if(f)
        profile_write(f, fn_f);
    join_id++;
}


static O profile_groupby_data_single(V x){
    P("Profiling groupby with a single column: %d\n", group_id);
    C fn_x[99];
    sprintf(fn_x, "/tmp/g%d.txt" , group_id);
    if(x)
        profile_write(x, fn_x);
    group_id++;
}


static O profile_groupby_data_multiple(V x){
    C fn_x[99];
    sprintf(fn_x, "/tmp/g%d.txt" , join_id);
    profile_join_write_multiple(x, fn_x);
    group_id++;
}

O profileGroupbyData(V x){
    if(xp == H_G){
        if (xn == 1)
            profile_groupby_data_single(vV(x,0));
        else
            profile_groupby_data_multiple(x);
    }
    else
        profile_groupby_data_single(x);
}

O profileJoinData(V x, V y, V f){
    if(xp == H_G) // isList
        profile_join_data_multiple(x,y,f);
    else
        profile_join_data_single(x,y,f);
}








