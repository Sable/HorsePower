#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

L readFile(S fileName, L op);
L readCSV(FILE* fp);
L getField(S line, C sp);
FILE* openFile(S s);

/* helper functions */
S trim(S s);
S trimLeft(S s);
S trimRight(S s);
void errorMsg(S msg);

#ifdef	__cplusplus
}
#endif