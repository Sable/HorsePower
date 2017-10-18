
#include "tpch/load_db.h"

#define TEST_QUERY(n) case n: testTPCHQ##n(); break

#include "tpch/test_q1.h"
#include "tpch/test_q3.h"
#include "tpch/test_q4.h"
#include "tpch/test_q6.h"
#include "tpch/test_q9.h"
#include "tpch/test_q12.h"
#include "tpch/test_q14.h"
#include "tpch/test_q16.h"
#include "tpch/test_q17.h"
#include "tpch/test_q99.h"

void testTPCH(L x){
	switch(x){
		TEST_QUERY(1);
		TEST_QUERY(3);
		TEST_QUERY(4);
		TEST_QUERY(6);
		TEST_QUERY(9);  /* testing */
		TEST_QUERY(12);
		TEST_QUERY(14);
		TEST_QUERY(16);
		TEST_QUERY(17);
		TEST_QUERY(99); /* experiments */
		default: P("No such test for query %lld yet\n",x); break;
	}
}
