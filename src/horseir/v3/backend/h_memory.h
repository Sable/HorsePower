#ifndef __H_MEMORY__
#define __H_MEMORY__

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

// slower than NEWL though more space efficient
#define HASH_AL(t,n) (t*)allocHashMem(sizeof(t)*(n))
#define HASH_AL1(t)  HASH_AL(t,1)


/* methods */

O initMemoryBasic ();
O initMemoryInterp();

L getTypeSize     (I typ, L len);
G allocMem        (G heap, L *cur, L top, I typ, L len);

V allocNode       ();
V allocTable      (L numCols);
G allocStrMem     (L n);

L getHeapOffset   ();
L setHeapOffset   (L cur);

V initV           (V x, I typ, L len);
V initTableDict   (V x, I typ, L numCols);
V initSymbol      (V x, L val);
V initList        (V x, L numItems);
V initFlatList    (V x, L numItems);
V initValue       (V x, I typ, L len);
V initDict        (V x, L numCols);
V initTable       (V x, L numCols);
V initKTable      (V x);

S insertString2   (S str);

/* buddy system */
L testMemory      ();
G allocHeap       (L size);
G getHeapMem      (I typ, L len);
G allocBlock      (I typ, L len);
L getRefCount     (G x);
L updateRefCount  (G x);
O printHeapInfo   ();

/* hash heap */
O initHashHeap    ();
G allocHashMem    (L size);
O printHashInfo   ();
L getHashHeap     ();
O setHashHeap     (L x);

#ifdef	__cplusplus
}
#endif

#endif
