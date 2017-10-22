
#include "tpch/load_db.h"

#define TEST_QUERY(n) case n: testTPCHQ##n(); break

#include "tpch/test_q1.h"   // pass (slow group and index)
#include "tpch/test_q3.h"   // pass (very slow)
#include "tpch/test_q4.h"   // pass (very slow)
#include "tpch/test_q6.h"   // pass (good, optimized)
#include "tpch/test_q9.h"   // not working
#include "tpch/test_q14.h"  // pass (good, optimized)
#include "tpch/test_q16.h"  // bug  (one left)
#include "tpch/test_q17.h"  // pass (good, optimized)
#include "tpch/test_q18.h"  // pass (very slow)
#include "tpch/test_q19.h"  // pass (good, optimized)
#include "tpch/test_q99.h"

void testTPCH(L x){
	switch(x){
		TEST_QUERY(1);
		TEST_QUERY(3);
		TEST_QUERY(4);
		TEST_QUERY(6);
		TEST_QUERY(9);  /* testing */
		TEST_QUERY(14);
		TEST_QUERY(16);
		TEST_QUERY(17);
		TEST_QUERY(18);
		TEST_QUERY(19);
		TEST_QUERY(99); /* experiments */
		default: P("No such test for query %lld yet\n",x); break;
	}
}
