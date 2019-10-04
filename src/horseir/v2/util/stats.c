#include "../global.h"

extern L listTableCur;
extern ListT listTable;
static C numTemp[99];

static S int2Str(L x){
    SP(numTemp, "%lld", x); R numTemp;
}

static S float2Str(E x){
    SP(numTemp, "%g", x); R numTemp;
}

static void scanB(V x, JSON *item){
    L countTrue = 0;
    DOI(xn, if(xB(i))countTrue++)
    addJSONField(item, newJSONKeyValue("true", int2Str(countTrue)));
    addJSONField(item, newJSONKeyValue("false", int2Str(xn-countTrue)));
}

#define scanTemplateInt(x, t) \
    L nonZero = 0; L maxL = LLONG_MIN, minL = LLONG_MAX; \
    DOI(xn, {if(x##t(i))nonZero++; \
             if(x##t(i)>maxL) maxL=x##t(i);\
             if(x##t(i)<minL) minL=x##t(i); }) \
    addJSONField(item, newJSONKeyValue("nonZero", int2Str(nonZero))); \
    addJSONField(item, newJSONKeyValue("max", int2Str(maxL))); \
    addJSONField(item, newJSONKeyValue("min", int2Str(minL)));

static void scanJ(V x, JSON *item){ scanTemplateInt(x, J); }

static void scanH(V x, JSON *item){ scanTemplateInt(x, H); }

static void scanI(V x, JSON *item){ scanTemplateInt(x, I); }

static void scanL(V x, JSON *item){ scanTemplateInt(x, L); }

#define scanTemplateFloat(x, t) \
    L nonZero = 0; E maxE = DBL_MIN, minE = DBL_MAX; \
    DOI(xn, {if(x##t(i))nonZero++; \
             if(x##t(i)>maxE) maxE=x##t(i);\
             if(x##t(i)<minE) minE=x##t(i); }) \
    addJSONField(item, newJSONKeyValue("nonZero", int2Str(nonZero))); \
    addJSONField(item, newJSONKeyValue("max", float2Str(maxE))); \
    addJSONField(item, newJSONKeyValue("min", float2Str(minE)));

static void scanF(V x, JSON *item){ scanTemplateFloat(x, F); }

static void scanE(V x, JSON *item){ scanTemplateFloat(x, E); }

static void scanX(V x, JSON *item){ TODO("Support X"); }

static void scanM(V x, JSON *item){ scanTemplateInt(x, M); }
static void scanD(V x, JSON *item){ scanTemplateInt(x, D); }
static void scanZ(V x, JSON *item){ scanTemplateInt(x, Z); }
static void scanU(V x, JSON *item){ scanTemplateInt(x, U); }
static void scanW(V x, JSON *item){ scanTemplateInt(x, W); }
static void scanT(V x, JSON *item){ scanTemplateInt(x, T); }

static void scanC(V x, JSON *item){ }

static void scanS(V x, JSON *item){ }

static void scanQ(V x, JSON *item){ }

static JSON *scanColumn(V x, Q id){
#define SCAN_TYPE(t) case##t scan##t(x,col); break
    JSON *col = initJSON();
    addJSONField(col, newJSONKeyValue("col_name", getSymbolStr(id)));
    addJSONField(col, newJSONKeyValue("col_type", (S)getTypeName(xp)));
    switch(xp){
        SCAN_TYPE(B);
        SCAN_TYPE(J);
        SCAN_TYPE(H);
        SCAN_TYPE(I);
        SCAN_TYPE(L);
        SCAN_TYPE(F);
        SCAN_TYPE(E);
        SCAN_TYPE(X);
        SCAN_TYPE(M);
        SCAN_TYPE(D);
        SCAN_TYPE(Z);
        SCAN_TYPE(U);
        SCAN_TYPE(W);
        SCAN_TYPE(T);
        SCAN_TYPE(C);
        SCAN_TYPE(S);
        SCAN_TYPE(Q);
        default: TODO("Add impl. for %s",getTypeName(xp));
    }
    return col;
}

//static void scanTable(Q id, V table, L k){
//    if(k>0) P(",\n");
//    P("{\"table\":\"%s\",\n", getSymbolStr(id));
//    P(" \"cols_size\":\"%lld\",\n", tableRow(table));
//    P(" \"cols\":[\n");
//    V keys = getTableKeys(table);
//    V cols = getTableVals(table);
//    DOI(vn(cols), scanColumn(getTableCol(cols, i), vQ(keys,i), i))
//    P("\n]}");
//}

static JSON* scanTable(Q id, V table){
    JSON *tab = initJSON();
    addJSONField(tab, newJSONKeyValue("table", getSymbolStr(id)));
    addJSONField(tab, newJSONKeyValue("cols_size", int2Str(tableRow(table))));
    V keys = getTableKeys(table);
    V cols = getTableVals(table);
    JSON **columns = NEW2(JSON, vn(cols));
    DOI(vn(cols), columns[i]=scanColumn(getTableCol(cols, i), vQ(keys,i)))
    addJSONField(tab, newJSONKeyList("cols", vn(cols), columns));
    return tab;
}

static void dumpStats(){
    initStats();
    JSON *rt = initJSON();
    addJSONField(rt, newJSONKeyValue("database", "TPC-H"));
    JSON **tables = NEW2(JSON, listTableCur);
    DOI(listTableCur, tables[i]=scanTable(listTable[i].sid, listTable[i].table))
    addJSONField(rt, newJSONKeyList("tables", listTableCur, tables));
    printJSON(rt);
}

static void loadStats(){
    demoJSON();
}

void handleStats(S cmd){
    if(!strcmp(cmd, "dump")) dumpStats();
    else if(!strcmp(cmd, "load")) loadStats();
    else EP("Add impl. for %s", qStats);
}



