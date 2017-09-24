#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

#define CELL_FIRST(x)  vV(x,0)
#define CELL_SECOND(x) vV(x,1)

#define getTableKeys(x)  CELL_FIRST(x)
#define getTableVals(x)  CELL_SECOND(x)
#define getDictKeys(x)   CELL_FIRST(x)
#define getDictVals(x)   CELL_SECOND(x)
#define getTableCol(x,k) vV(x,k)
#define getKTableKey(x)  CELL_FIRST(x)
#define getKTableVal(x)  CELL_SECOND(x)

/* methods */

void initMain       ();
void useNaiveSystem ();
void useBuddySystem ();

L getTypeSize       (L typ, L len);
G allocMem          (G heap, L *cur, L top, L typ, L len);

V allocNode         ();
V allocTable        (L numCols);
G allocStrMem       (L n);

V initV             (V x, L typ, L len);
V initTableDict     (V x, L typ, L numCols);
V initSymbol        (V x, L val);
V initList          (V x, L numItems);
V initValue         (V x, L typ, L len);
V initDict          (V x, L numCols);
V initTable         (V x, L numCols);
V initKTable        (V x);

S insertString      (S str);

/* buddy system */
L testMemory        ();
G allocHeap         (L size);
G getHeapMem        (L typ, L len);
G allocBlock        (L typ, L len);
L getRefCount       (G x);
L updateRefCount    (G x);
void printHeapInfo  ();

#ifdef	__cplusplus
}
#endif
