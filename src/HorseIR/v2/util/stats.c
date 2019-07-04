#include "../global.h"

extern L listTableCur;
extern ListT listTable;

static void dumpTable(Q id, V table){
    P("Table: %s\n", getSymbolStr(id));
}

static void dumpStats(){
    initStats();
    DOI(listTableCur, dumpTable(listTable[i].sid, listTable[i].table))
    P("Data dumped done.\n");
}

static void loadStats(){
}

void handleStats(S cmd){
    if(!strcmp(cmd, "dump")) dumpStats();
    else if(!strcmp(cmd, "load")) loadStats();
    else EP("Add impl. for %s", qStats);
}



