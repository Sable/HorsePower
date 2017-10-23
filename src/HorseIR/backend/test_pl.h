
#include "pl/blackscholes.h"

void testPL(L x){
	switch(x){
		case 0: runBS(); break;
		default: P("No such test for pl %lld yet\n",x); break;
	}
}
