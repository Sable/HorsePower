
#include "tpch/load_db.h"
#include "tpch/test_q6.h"

#define TEST_QUERY(n) case n: testTPCHQ##n(); break

#define PROFILE(n,x) { struct timeval p0, p1; \
        gettimeofday(&p0, NULL); e = x; CHECK(e,n); gettimeofday(&p1, NULL); \
        P("[Profiling Q6] Line %d: %g ms\n", n,calcInterval(p0,p1)/1000.0); }

void testTPCH(L x){
	switch(x){
		TEST_QUERY(4);
		TEST_QUERY(6);
		default: P("No such test for query %lld yet\n",x); break;
	}
}
