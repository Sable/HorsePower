SELECT l_shipmode    AS l_shipmode, 
       Sum(hl_count) AS high_line_count, 
       Sum(ll_count) AS low_line_count 
FROM   (SELECT l_shipmode AS l_shipmode, 
               CASE 
                 WHEN ( o_orderpriority IN ( '1-URGENT', '2-HIGH' ) ) THEN 1 
                 ELSE 0 
               end        AS hl_count, 
               CASE 
                 WHEN ( o_orderpriority NOT IN ( '1-URGENT', '2-HIGH' ) ) THEN 1 
                 ELSE 0 
               end        AS ll_count 
        FROM   (SELECT _tmp_4b93c28f.l_orderkey      AS l_orderkey, 
                       _tmp_4b93c28f.l_partkey       AS l_partkey, 
                       _tmp_4b93c28f.l_suppkey       AS l_suppkey, 
                       _tmp_4b93c28f.l_linenumber    AS l_linenumber, 
                       _tmp_4b93c28f.l_quantity      AS l_quantity, 
                       _tmp_4b93c28f.l_extendedprice AS l_extendedprice, 
                       _tmp_4b93c28f.l_discount      AS l_discount, 
                       _tmp_4b93c28f.l_tax           AS l_tax, 
                       _tmp_4b93c28f.l_returnflag    AS l_returnflag, 
                       _tmp_4b93c28f.l_linestatus    AS l_linestatus, 
                       _tmp_4b93c28f.l_shipdate      AS l_shipdate, 
                       _tmp_4b93c28f.l_commitdate    AS l_commitdate, 
                       _tmp_4b93c28f.l_receiptdate   AS l_receiptdate, 
                       _tmp_4b93c28f.l_shipinstruct  AS l_shipinstruct, 
                       _tmp_4b93c28f.l_shipmode      AS l_shipmode, 
                       _tmp_4b93c28f.l_comment       AS l_comment, 
                       orders.o_orderkey             AS o_orderkey, 
                       orders.o_custkey              AS o_custkey, 
                       orders.o_orderstatus          AS o_orderstatus, 
                       orders.o_totalprice           AS o_totalprice, 
                       orders.o_orderdate            AS o_orderdate, 
                       orders.o_orderpriority        AS o_orderpriority, 
                       orders.o_clerk                AS o_clerk, 
                       orders.o_shippriority         AS o_shippriority, 
                       orders.o_comment              AS o_comment 
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
                        WHERE  ( l_shipmode IN ( 'MAIL', 'SHIP' ) ) 
                               AND ( l_commitdate < l_receiptdate ) 
                               AND ( l_shipdate < l_commitdate ) 
                               AND ( l_receiptdate >= '1994-01-01' ) 
                               AND ( l_receiptdate < '1995-01-01' )) 
                       _tmp_4b93c28f 
                       INNER JOIN (SELECT orders.o_orderkey, 
                                          orders.o_custkey, 
                                          orders.o_orderstatus, 
                                          orders.o_totalprice, 
                                          orders.o_orderdate, 
                                          orders.o_orderpriority, 
                                          orders.o_clerk, 
                                          orders.o_shippriority, 
                                          orders.o_comment 
                                   FROM   orders) orders 
                               ON _tmp_4b93c28f.l_orderkey = orders.o_orderkey) 
               _tmp_563b6880) _tmp_f80e5057 
GROUP  BY l_shipmode 
ORDER  BY l_shipmode 
