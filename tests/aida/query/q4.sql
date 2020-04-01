SELECT o_orderpriority AS o_orderpriority, 
       Count(*)        AS order_count 
FROM   (SELECT o_orderkey, 
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
        WHERE  ( o_orderdate >= '1993-07-01' ) 
               AND ( o_orderdate < '1993-10-01' ) 
               AND ( o_orderkey IN (SELECT l_orderkey AS l_orderkey 
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
                                                           lineitem.l_linenumber 
                                                           , 
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
WHERE  ( l_commitdate < l_receiptdate )) 
_tmp_4a49dcc2) )) _tmp_46b9a863 
GROUP  BY o_orderpriority 
ORDER  BY o_orderpriority 
