#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* methods */

void initSys();
void deleteSys();
L registerTable(S tableName, V tablePtr);
V findTableByName(L sid);
L copyTable(V x, V y);

/* error messages */
#define E_DOMAIN          1
#define E_GENERAL         2
#define E_INDEX           3
#define E_TABLE_NOT_FOUND 50
#define E_NOT_IMPL        99

#ifdef	__cplusplus
}
#endif
