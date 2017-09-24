#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

// L readFile(S fileName, L op);
V    readCSV          (S fileName, L numCols, L *types, Q *symList);
L    loadCSV          (FILE *fp, B isLoading, V table, L numCols, L *types);
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
L getStrPretty        (S str, L maxSize);


#define ALL -1
#define printValue(x) printValueItem(x,ALL)
#define printList(x)  printListItem(x,ALL)
#define printStr(x)   printStrItem(x,ALL)
#define printDict(x)  printDictItem(x,ALL)
#define printEnum(x)  printEnumItem(x,ALL)

L printType           (L x);
L printTag            (L x);
L printBasicItem      (V x, L k);
L printInfo           (V x);
L printBasicValue     (V x, B hasTag);
L printStrItem        (V x, L k);
L printListItem       (V x, L k);
L printDictItem       (V x, L k);
L printEnumItem       (V x, L k);
L printValueItem      (V x, L k);
L printV              (V x);
L printTable          (V x);
L printKTable         (V x);
L printTablePretty    (V x, L rowLimit);

#ifdef	__cplusplus
}
#endif