

#include "tpch/test_helper.h"
#include "tpch/load_db.h"
#include "tpch/test_q6.h"

#define TEST_QUERY(n) case n: testTPCHQ##n(); break

void testTPCH(L x){
	switch(x){
		TEST_QUERY(6);
		default: P("No such test for query %lld yet\n",x); break;
	}
}
