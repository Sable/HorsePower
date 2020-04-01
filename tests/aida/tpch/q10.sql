SELECT c_custkey AS c_custkey, 
       c_name    AS c_name, 
       Sum(rev)  AS revenue, 
       c_acctbal AS c_acctbal, 
       n_name    AS n_name, 
       c_address AS c_address, 
       c_phone   AS c_phone, 
       c_comment AS c_comment 
FROM   (SELECT _tmp_5901aa21.c_custkey       AS c_custkey, 
               _tmp_5901aa21.c_name          AS c_name, 
               _tmp_5901aa21.c_address       AS c_address, 
               _tmp_5901aa21.c_nationkey     AS c_nationkey, 
               _tmp_5901aa21.c_phone         AS c_phone, 
               _tmp_5901aa21.c_acctbal       AS c_acctbal, 
               _tmp_5901aa21.c_mktsegment    AS c_mktsegment, 
               _tmp_5901aa21.c_comment       AS c_comment, 
               _tmp_5901aa21.o_orderkey      AS o_orderkey, 
               _tmp_5901aa21.o_custkey       AS o_custkey, 
               _tmp_5901aa21.o_orderstatus   AS o_orderstatus, 
               _tmp_5901aa21.o_totalprice    AS o_totalprice, 
               _tmp_5901aa21.o_orderdate     AS o_orderdate, 
               _tmp_5901aa21.o_orderpriority AS o_orderpriority, 
               _tmp_5901aa21.o_clerk         AS o_clerk, 
               _tmp_5901aa21.o_shippriority  AS o_shippriority, 
               _tmp_5901aa21.o_comment       AS o_comment, 
               _tmp_5901aa21.l_orderkey      AS l_orderkey, 
               _tmp_5901aa21.rev             AS rev, 
               nation.n_nationkey            AS n_nationkey, 
               nation.n_name                 AS n_name, 
               nation.n_regionkey            AS n_regionkey, 
               nation.n_comment              AS n_comment 
        FROM   (SELECT _tmp_ebda7199.c_custkey       AS c_custkey, 
                       _tmp_ebda7199.c_name          AS c_name, 
                       _tmp_ebda7199.c_address       AS c_address, 
                       _tmp_ebda7199.c_nationkey     AS c_nationkey, 
                       _tmp_ebda7199.c_phone         AS c_phone, 
                       _tmp_ebda7199.c_acctbal       AS c_acctbal, 
                       _tmp_ebda7199.c_mktsegment    AS c_mktsegment, 
                       _tmp_ebda7199.c_comment       AS c_comment, 
                       _tmp_ebda7199.o_orderkey      AS o_orderkey, 
                       _tmp_ebda7199.o_custkey       AS o_custkey, 
                       _tmp_ebda7199.o_orderstatus   AS o_orderstatus, 
                       _tmp_ebda7199.o_totalprice    AS o_totalprice, 
                       _tmp_ebda7199.o_orderdate     AS o_orderdate, 
                       _tmp_ebda7199.o_orderpriority AS o_orderpriority, 
                       _tmp_ebda7199.o_clerk         AS o_clerk, 
                       _tmp_ebda7199.o_shippriority  AS o_shippriority, 
                       _tmp_ebda7199.o_comment       AS o_comment, 
                       _tmp_58b9d8ef.l_orderkey      AS l_orderkey, 
                       _tmp_58b9d8ef.rev             AS rev 
                FROM   (SELECT customer.c_custkey            AS c_custkey, 
                               customer.c_name               AS c_name, 
                               customer.c_address            AS c_address, 
                               customer.c_nationkey          AS c_nationkey, 
                               customer.c_phone              AS c_phone, 
                               customer.c_acctbal            AS c_acctbal, 
                               customer.c_mktsegment         AS c_mktsegment, 
                               customer.c_comment            AS c_comment, 
                               _tmp_a68f0b7b.o_orderkey      AS o_orderkey, 
                               _tmp_a68f0b7b.o_custkey       AS o_custkey, 
                               _tmp_a68f0b7b.o_orderstatus   AS o_orderstatus, 
                               _tmp_a68f0b7b.o_totalprice    AS o_totalprice, 
                               _tmp_a68f0b7b.o_orderdate     AS o_orderdate, 
                               _tmp_a68f0b7b.o_orderpriority AS o_orderpriority, 
                               _tmp_a68f0b7b.o_clerk         AS o_clerk, 
                               _tmp_a68f0b7b.o_shippriority  AS o_shippriority, 
                               _tmp_a68f0b7b.o_comment       AS o_comment 
                        FROM   (SELECT customer.c_custkey, 
                                       customer.c_name, 
                                       customer.c_address, 
                                       customer.c_nationkey, 
                                       customer.c_phone, 
                                       customer.c_acctbal, 
                                       customer.c_mktsegment, 
                                       customer.c_comment 
                                FROM   customer) customer 
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
                                                          orders.o_orderpriority 
                                                          , 
orders.o_clerk, 
orders.o_shippriority, 
orders.o_comment 
FROM   orders) orders 
WHERE  ( o_orderdate >= '1993-10-01' ) 
AND ( o_orderdate < '1994-01-01' )) 
_tmp_a68f0b7b 
ON customer.c_custkey = _tmp_a68f0b7b.o_custkey) 
_tmp_ebda7199 
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
WHERE  ( l_returnflag = 'R' )) _tmp_b2a8731c 
) 
_tmp_58b9d8ef 
ON _tmp_ebda7199.o_orderkey = _tmp_58b9d8ef.l_orderkey) 
_tmp_5901aa21 
INNER JOIN (SELECT nation.n_nationkey, 
nation.n_name, 
nation.n_regionkey, 
nation.n_comment 
FROM   nation) nation 
ON _tmp_5901aa21.c_nationkey = nation.n_nationkey) _tmp_dc85eb95 
GROUP  BY c_custkey, 
          c_name, 
          c_acctbal, 
          c_phone, 
          n_name, 
          c_address, 
          c_comment 
ORDER  BY revenue DESC 
LIMIT  20 
