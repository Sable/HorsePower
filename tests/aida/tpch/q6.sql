SELECT Sum(rev) AS revenue 
FROM   (SELECT ( l_extendedprice * l_discount ) AS rev 
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
                WHERE  ( l_shipdate >= '1994-01-01' ) 
                       AND ( l_shipdate < '1995-01-01' ) 
                       AND ( l_discount >= Cast(0.05 AS FLOAT) ) 
                       AND ( l_discount <= Cast(0.07 AS FLOAT) ) 
                       AND ( l_quantity < 24 )) _tmp_a02ba39a) _tmp_507bfe4f 
