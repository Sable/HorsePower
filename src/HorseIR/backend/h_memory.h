#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

/* methods */

void initMain ();

L getTypeSize (L typ);
G getCacheMem (L typ, L len);
G getHeapMem  (L typ, L len);
G allocMem    (G heap, L *cur, L top, L typ, L len);

V allocV      (L typ, L len);
V allocNode   ();
V allocDict   ();
V allocList   (L numItems);
V allocTable  (L numCols);
V allocKTable ();

V initV       (V x, L typ, L len);
V initDict    (V x);
V initSymbol  (V x, L val);
V initList    (V x, L numItems);
V initValue   (V x, L typ, L len);
V initTable   (V x, L numCols);
V initKTable  (V x);

void printType    (L x);
void printTypeStr (L x, S buff);
void printHeapInfo();

V getDictKey  (V x);
V getDictVal  (V x);
V getTableDict(V x, L k);

#ifdef	__cplusplus
}
#endif