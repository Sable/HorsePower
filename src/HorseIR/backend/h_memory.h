#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

/* methods */

void hInit();

L getTypeSize(L typ);
G getCacheMem(L typ, L len);
G getHeapMem(L typ, L len);
G allocMem(G heap, L cur, L top, L typ, L len);

V allocV(L typ, L len);
V allocDict();
V allocList(L numItems);
V allocTable(L numCols);
V allocKTable();

#ifdef	__cplusplus
}
#endif