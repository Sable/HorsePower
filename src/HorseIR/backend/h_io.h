#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

// L readFile(S fileName, L op);
V    readCSV          (S fileName, L numCols, L *types, L *symList);
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
L getColWidth         (V x);
L getStrPretty        (S str, L maxSize);

L printType           (L x);
L printBasicItem      (V x, L k);
L printInfo           (V x);
L printBasicValue     (V x, B hasTag);
L printStr            (V x);
L printList           (V x);
L printDict           (V x);
L printEnum           (V x);
L printValue          (V x);
L printV              (V x);
L printTable          (V x);
L printKTable         (V x);
L printTablePretty    (V x, L rowLimit);

#ifdef	__cplusplus
}
#endif