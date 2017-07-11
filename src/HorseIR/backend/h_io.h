#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

// L readFile(S fileName, L op);
V readCSV(S fileName, L numCols, L *types, L *symList);
L loadCSV(FILE *fp, B isLoading, V table, L numCols, L *types);
L getField(S line, C sp, V x, L rowID, L *types, L *errCode);
FILE* openFile(S s);
void loadItem(V x, L k, L typ, S s);

/* helper functions */
S trim(S s);
S trimLeft(S s);
S trimRight(S s);
S trimSelf(S s);
void errorMsg(S msg);

/* output */

void printItem(V x, S strBuff);
void printListItem(V x, L k, S strBuff);
void printList(V x);
void printDict(V x);
void printTable(V x);

#ifdef	__cplusplus
}
#endif