SELECT l_orderkey     AS l_orderkey, 
       Sum(rev)       AS revenue, 
       o_orderdate    AS o_orderdate, 
       o_shippriority AS o_shippriority 
FROM   (SELECT _tmp_86dfd31b.c_custkey       AS c_custkey, 
               _tmp_86dfd31b.c_name          AS c_name, 
               _tmp_86dfd31b.c_address       AS c_address, 
               _tmp_86dfd31b.c_nationkey     AS c_nationkey, 
               _tmp_86dfd31b.c_phone         AS c_phone, 
               _tmp_86dfd31b.c_acctbal       AS c_acctbal, 
               _tmp_86dfd31b.c_mktsegment    AS c_mktsegment, 
               _tmp_86dfd31b.c_comment       AS c_comment, 
               _tmp_86dfd31b.o_orderkey      AS o_orderkey, 
               _tmp_86dfd31b.o_custkey       AS o_custkey, 
               _tmp_86dfd31b.o_orderstatus   AS o_orderstatus, 
               _tmp_86dfd31b.o_totalprice    AS o_totalprice, 
               _tmp_86dfd31b.o_orderdate     AS o_orderdate, 
               _tmp_86dfd31b.o_orderpriority AS o_orderpriority, 
               _tmp_86dfd31b.o_clerk         AS o_clerk, 
               _tmp_86dfd31b.o_shippriority  AS o_shippriority, 
               _tmp_86dfd31b.o_comment       AS o_comment, 
               _tmp_632bae60.l_orderkey      AS l_orderkey, 
               _tmp_632bae60.rev             AS rev 
        FROM   (SELECT _tmp_9b357395.c_custkey       AS c_custkey, 
                       _tmp_9b357395.c_name          AS c_name, 
                       _tmp_9b357395.c_address       AS c_address, 
                       _tmp_9b357395.c_nationkey     AS c_nationkey, 
                       _tmp_9b357395.c_phone         AS c_phone, 
                       _tmp_9b357395.c_acctbal       AS c_acctbal, 
                       _tmp_9b357395.c_mktsegment    AS c_mktsegment, 
                       _tmp_9b357395.c_comment       AS c_comment, 
                       _tmp_8b0868bb.o_orderkey      AS o_orderkey, 
                       _tmp_8b0868bb.o_custkey       AS o_custkey, 
                       _tmp_8b0868bb.o_orderstatus   AS o_orderstatus, 
                       _tmp_8b0868bb.o_totalprice    AS o_totalprice, 
                       _tmp_8b0868bb.o_orderdate     AS o_orderdate, 
                       _tmp_8b0868bb.o_orderpriority AS o_orderpriority, 
                       _tmp_8b0868bb.o_clerk         AS o_clerk, 
                       _tmp_8b0868bb.o_shippriority  AS o_shippriority, 
                       _tmp_8b0868bb.o_comment       AS o_comment 
                FROM   (SELECT c_custkey, 
                               c_name, 
                               c_address, 
                               c_nationkey, 
                               c_phone, 
                               c_acctbal, 
                               c_mktsegment, 
                               c_comment 
                        FROM   (SELECT customer.c_custkey, 
                                       customer.c_name, 
                                       customer.c_address, 
                                       customer.c_nationkey, 
                                       customer.c_phone, 
                                       customer.c_acctbal, 
                                       customer.c_mktsegment, 
                                       customer.c_comment 
                                FROM   customer) customer 
                        WHERE  ( c_mktsegment = 'BUILDING' )) _tmp_9b357395 
                       INNER JOIN (SELECT o_orderkey, 
                                          o_custkey, 
                                          o_orderstatus, 
                                          o_totalprice, 
                                          o_orderdate, 
                                          o_orderpriority, 
                                          o_clerk, 
                                          o_shippriority, 
                                          o_comment 
                                   FROM   (SELECT orders.o_orderkey, 
                                                  orders.o_custkey, 
                                                  orders.o_orderstatus, 
                                                  orders.o_totalprice, 
                                                  orders.o_orderdate, 
                                                  orders.o_orderpriority, 
                                                  orders.o_clerk, 
                                                  orders.o_shippriority, 
                                                  orders.o_comment 
                                           FROM   orders) orders 
                                   WHERE  ( o_orderdate < '1995-03-15' )) 
                                  _tmp_8b0868bb 
                               ON _tmp_9b357395.c_custkey = 
                                  _tmp_8b0868bb.o_custkey) 
               _tmp_86dfd31b 
               INNER JOIN (SELECT l_orderkey                               AS 
                                  l_orderkey, 
                                  ( l_extendedprice * ( 1 - l_discount ) ) AS 
                                  rev 
                           FROM   (SELECT l_orderkey, 
                                          l_partkey, 
                                          l_suppkey, 
                                          l_linenumber, 
                                          l_quantity, 
                                          l_extendedprice, 
                                          l_discount, 
                                          l_tax, 
                                          l_returnflag, 
                                          l_linestatus, 
                                          l_shipdate, 
                                          l_commitdate, 
                                          l_receiptdate, 
                                          l_shipinstruct, 
                                          l_shipmode, 
                                          l_comment 
                                   FROM   (SELECT lineitem.l_orderkey, 
                                                  lineitem.l_partkey, 
                                                  lineitem.l_suppkey, 
                                                  lineitem.l_linenumber, 
                                                  lineitem.l_quantity, 
                                                  lineitem.l_extendedprice, 
                                                  lineitem.l_discount, 
                                                  lineitem.l_tax, 
                                                  lineitem.l_returnflag, 
                                                  lineitem.l_linestatus, 
                                                  lineitem.l_shipdate, 
                                                  lineitem.l_commitdate, 
                                                  lineitem.l_receiptdate, 
                                                  lineitem.l_shipinstruct, 
                                                  lineitem.l_shipmode, 
                                                  lineitem.l_comment 
                                           FROM   lineitem) lineitem 
                                   WHERE  ( l_shipdate > '1995-03-15' )) 
                                  _tmp_a121d06f) 
                                        _tmp_632bae60 
                       ON _tmp_86dfd31b.o_orderkey = _tmp_632bae60.l_orderkey) 
       _tmp_d757f995 
GROUP  BY l_orderkey, 
          o_orderdate, 
          o_shippriority 
ORDER  BY revenue DESC, 
          o_orderdate 
LIMIT  10 
