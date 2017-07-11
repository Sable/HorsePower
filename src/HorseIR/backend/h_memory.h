#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

/* methods */

void initMain();

L getTypeSize(L typ);
G getCacheMem(L typ, L len);
G getHeapMem(L typ, L len);
G allocMem(G heap, L *cur, L top, L typ, L len);

V allocV(L typ, L len);
V allocDict();
V allocList(L numItems);
V allocTable(L numCols);
V allocKTable();

void initV(V x, L typ, L len);
void initDict(V x);
void initSymbol(V x, L val);
void initList(V x, L numItems);
void initValue(V x, L typ, L len);

void printType(L x);
void printTypeStr(L x, S buff);

V getDictKey(V x);
V getDictVal(V x);
V getTableDict(V x, L k);

#ifdef	__cplusplus
}
#endif