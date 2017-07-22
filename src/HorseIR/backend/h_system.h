#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

#define copyColumnValue(z,x) copyV(z,x)

/* methods */

void initSys();
void deleteSys();

L registerTable     (S tableName, V tablePtr);
V findTableByName   (L sid);
L copyV             (V z, V x);
L findColFromTable  (V x, L cId);
V promoteValue      (V x, L typMax);
B checkZero         (V x);
L getSingleIntValue (V x);
L calcFact          (L n);
L getNumOfNonZero   (V x);

B isTypeGroupInt    (L t);
B isTypeGroupFloat  (L t);
B isTypeGroupReal   (L t);
B isTypeGroupRealX  (L t);
B isTypeGroupTime   (L t);
B isTypeGroupNumeric(L t);
B isTypeGroupBasic  (L t);


L inferReal2Int     (L t);
L inferNeg          (L t);
L inferRecip        (L t);
L inferSignum       (L t);
L inferPi           (L t);

/* error messages */
#define E_DOMAIN          1
#define E_GENERAL         2
#define E_INDEX           3
#define E_DIV_ZERO        4
#define E_LENGTH          5
#define E_TYPE            6
#define E_TABLE_NOT_FOUND 50
#define E_COL_NOT_FOUND   51
#define E_NOT_IMPL        99

#ifdef	__cplusplus
}
#endif
