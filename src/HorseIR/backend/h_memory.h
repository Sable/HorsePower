#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

/* methods */

void initMain       ();
void useNaiveSystem ();
void useBuddySystem ();

L getTypeSize       (L typ, L len);
G allocMem          (G heap, L *cur, L top, L typ, L len);

V allocV            (L typ, L len);
V allocNode         ();
V allocDict         ();
V allocList         (L numItems);
V allocTable        (L numCols);
V allocKTable       ();
G allocStrMem       (L n);

V initV             (V x, L typ, L len);
V initDict          (V x);
V initSymbol        (V x, L val);
V initList          (V x, L numItems);
V initValue         (V x, L typ, L len);
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
