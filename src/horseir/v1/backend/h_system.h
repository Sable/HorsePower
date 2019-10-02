#ifndef __H_SYSTEM__
#define __H_SYSTEM__

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

#define copyColumnValue(z,x) copyV(z,x)

#define getEnumName(x)   (vy(x)->source)
#define getEnumKey(x)    (vy(x)->ek)  //getEnumTarget
#define getEnumVal(x)    (vy(x)->ev)
#define getEnumIndex(x)  (vg(x))

#define tableRow(x)  (va(x)->row)
#define tableCol(x)  (va(x)->col)
#define tableFKey(x) (va(x)->fkey)
#define dictNum(x)   tableCol(x)  // # of key-value pairs

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

void printFloat(E x);

void initSys();
void swap2(V z);

//#define time_tic()
//#define time_toc() 0
//#define time_clear()

#ifndef time_tic
void time_tic();   // set off global timer
E    time_toc();   // display and return the elapsed time
void time_clear(); // set index to 0
#endif

IL registerTable         (S tableName, V tablePtr);
V  findTableByName       (L sid);
I  copyV                 (V z, V x);
I  initListCopy          (V z, V x, L len);
//L copyByIndex           (V z, L p, V x, L q);
//L copyByIndexSimple     (V z, L p, V x, L q);
//L findColFromTable2     (V *z, V x, L cId);
I  findColFromTable      (V x, L cId);

#define promoteValue(z,x,typMax) promoteValueWithIndex(z,x,typMax,NULL)
I promoteValueWithIndex  (V z, V x, L typMax, V ind);
B checkZero              (V x);
B checkMatch             (V x);
L getSingleIntValue      (V x);
L calcFact               (L n);
I getNumOfNonZero        (V x, L *z);
I appendList             (V z, V x, V y);
I appendEnum             (V z, V x, V y);
V getValueFromSymbol     (Q id);
I checkLength            (V x);
I getCommonType          (V x, I *typ, L *len);
I fillRaze               (V z, L *n0, V x);
I matchPair              (B *t, V x, V y);
I getLikeFromString      (B *t, S src, S pat);
pcre2_code* getLikePatten(S pat);
B getLikeMatch           (S src, pcre2_code *re, pcre2_match_data *matchData);

I copyEnumIndex          (V z, V x);
I getDictValue           (V z, V x);
I getColumnValue         (V z, V x);

I getDatetimeOp          (Q id);
D calcDate               (D x, L v, I dop, I op);

B isAssignableType       (I x, I y);
B compareTuple           (V x, L ax, V y, L ay);
I isListIndexOf          (V x, V y, L *sizeX, L *sizeY);
I joinIndexHash          (V z, V x, V y, C op);
I setFKey                (V x, V colName, V fKey);
B isOrdered              (V x);
B isOrderWithIndex       (V x, V ind);
I searchOrdered          (V z, V x, V y);
I listFlat2Normal        (V z, V x);
I listFlatEachLen        (V z, V x);
I getOpFromSymbol        (L x);
B compareOpWithIndex     (V x, V y, L k0, L k1, I op);
I joinOtherColumns       (V z, V x, V y, V ind, L fx, V f);
I joinOneColumn          (V z, V x, V y, L sop);
L getListSize1           (V x);

F logBaseF               (F b, F x);
E logBaseE               (E b, E x);
B logicAnd               (B x, B y);
B logicNand              (B x, B y);
B logicOr                (B x, B y);
B logicNor               (B x, B y);
B logicXor               (B x, B y);

B isTypeGroupInt         (I t);
B isTypeGroupFloat       (I t);
B isTypeGroupReal        (I t);
B isTypeGroupDate        (I t);
B isTypeGroupTime        (I t);
B isTypeGroupString      (I t);
B isTypeGroupCompound    (I t);

B isTypeGroupNumber      (I t);
B isTypeGroupDTime       (I t);
B isTypeGroupColumn      (I t);
B isTypeGroupComparable  (I t);
B isTypeGroupBasic       (I t);
B isTypeGroupAny         (I t);
B isListSameType         (V x, I typ);

I inferReal2Int          (I t);
I inferNeg               (I t);
I inferRecip             (I t);
I inferSignum            (I t);
I inferPi                (I t);

void printErrMsg         (I eid);

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

#endif
