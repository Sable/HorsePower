#ifndef __H_IO__
#define __H_IO__

#ifdef	__cplusplus
extern "C" {
#endif

// L readFile(S fileName, L op);
V    readCSV          (S fileName, L numCols, L *types, Q *symList);
L    loadCSV          (void *fp, B isLoading, V table, L numCols, L *types);
L    getField         (S line, C sp, V x, L rowID, L *types, L *errCode);
FILE*openFile         (S s);
void loadItem         (V x, L k, L typ, S s);

/* helper functions */
S    trim             (S s);
S    trimLeft         (S s);
S    trimRight        (S s);
S    trimSelf         (S s);
void errorMsg         (S msg);

/* output */

L getTypeStr          (L x, S buff);
L getComplexStr       (X x, S buff);
L getBasicItemStr     (V x, L k, S buff, B hasTick);
L getStringItemStr    (V x, L k);
L getColWidth         (V x, L k, L rowLimit);
I getStrPretty        (S str, L maxSize);
V readMatrix          (S fileName);

#define ALL -1
#define printValue(x) printValueItem(x,ALL)
#define printList(x)  printListItem(x,ALL)
#define printStr(x)   printStrItem(x,ALL)
#define printDict(x)  printDictItem(x,ALL)
#define printEnum(x)  printEnumItem(x,ALL)

L printType2          (L x);
L printTag            (L x);
L printBasicItem      (V x, L k);
I printInfo           (V x);
I printBasicValue     (V x, L k, B hasTag);
I printStrItem        (V x, L k);
O printListItem       (V x, L k);
I printDictItem       (V x, L k);
I printEnumItem       (V x, L k);
I printValueItem      (V x, L k);
I printV              (V x);
I printV2             (V x, L n);
I printV3             (V x, L a, L b, B isR);
I printTable          (V x);
I printKTable         (V x);
I printTablePretty    (V x, L rowLimit);
O printFormat         (V x, I op);

void serializeV(V x, FILE *fp);
void readSerializeV(V x, FILE *fp);

#ifdef	__cplusplus
}
#endif

#endif
