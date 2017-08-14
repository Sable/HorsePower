#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

#define copyColumnValue(z,x) copyV(z,x)

#define getKTableKey(x) vV(x,0)
#define getKTableVal(x) vV(x,1)
#define getEnumName(x)  ((x)->y.name)

/* methods */

void initSys();
void deleteSys();

L registerTable         (S tableName, V tablePtr);
V findTableByName       (L sid);
L copyV                 (V z, V x);
L findColFromTable      (V x, L cId);
L promoteValue          (V z, V x, L typMax);
B checkZero             (V x);
B checkMatch            (V x);
L getSingleIntValue     (V x);
L calcFact              (L n);
L getNumOfNonZero       (V x);
L appendList            (V z, V x, V y);
L appendEnum            (V z, V x, V y);
S genLikeString         (S inStr, L inLen);
V getValueFromSymbol    (Q id);
L getCommonType         (V x, L *typ, L *len);
L fillRaze              (V z, L *n0, V x);
L matchPair             (B *t, V x, V y);

L getEnumValue          (V z, V x);
L getDictValue          (V z, V x);
L getColumnValue        (V z, V x);
V getColKey             (V x);
V getColVal             (V x);
V getTableCol           (V x, L k);
L getTableRowNumber     (V x);
L getTableColNumber     (V x);

B isTypeGroupInt        (L t);
B isTypeGroupFloat      (L t);
B isTypeGroupReal       (L t);
B isTypeGroupDate       (L t);
B isTypeGroupTime       (L t);
B isTypeGroupString     (L t);
B isTypeGroupCompound   (L t);

B isTypeGroupNumber     (L t);
B isTypeGroupDTime      (L t);
B isTypeGroupColumn     (L t);
B isTypeGroupComparable (L t);
B isTypeGroupBasic      (L t);
B isTypeGroupAny        (L t);

L inferReal2Int         (L t);
L inferNeg              (L t);
L inferRecip            (L t);
L inferSignum           (L t);
L inferPi               (L t);

/* error messages */
#define E_DOMAIN          1
#define E_GENERAL         2
#define E_INDEX           3
#define E_DIV_ZERO        4
#define E_LENGTH          5
#define E_TYPE            6
#define E_NULL_VALUE      7
#define E_LIKE_PATTERN    8
#define E_MATCH           9
#define E_ENUM_INDEX      10
#define E_RAZE_LIST       11
#define E_UNKNOWN         12
#define E_TABLE_NOT_FOUND 50
#define E_COL_NOT_FOUND   51
#define E_NOT_IMPL        99

#ifdef	__cplusplus
}
#endif
