#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

#define copyColumnValue(z,x) copyV(z,x)

/* methods */

void initSys();
void deleteSys();
L registerTable(S tableName, V tablePtr);
V findTableByName(L sid);
L copyV(V z, V x);
L findColFromTable(V x, L cId);

/* error messages */
#define E_DOMAIN          1
#define E_GENERAL         2
#define E_INDEX           3
#define E_TABLE_NOT_FOUND 50
#define E_COL_NOT_FOUND   51
#define E_NOT_IMPL        99

#ifdef	__cplusplus
}
#endif
