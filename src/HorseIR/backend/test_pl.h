
#include "pl/blackscholes.h"

void testPL(L x, L sid){
	switch(x){
		case 0: runBS(sid); break;
		default: P("No such test for pl %lld yet\n",x); break;
	}
}
