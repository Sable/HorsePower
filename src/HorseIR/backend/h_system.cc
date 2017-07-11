/*
 * system functions
 */

#include "h_global.h"

V findTableByName()

/* Error messages */

#define errCaseCell(label, msg) case label: P(msg); break

void printErrMsg(L eid){
	P("Error: ")
	switch(eid){
		errCaseCell(E_GENERAL,         "General error"  );
		errCaseCell(E_INDEX,           "Index error"    );
		errCaseCell(E_TABLE_NOT_FOUND, "Table not found");
	}
	P(".\n");
}
