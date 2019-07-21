SELECT c_count  AS c_count, 
       Count(*) AS custdist 
FROM   (SELECT c_custkey         AS c_custkey, 
               Count(o_orderkey) AS c_count 
        FROM   (SELECT customer.c_custkey            AS c_custkey, 
                       customer.c_name               AS c_name, 
                       customer.c_address            AS c_address, 
                       customer.c_nationkey          AS c_nationkey, 
                       customer.c_phone              AS c_phone, 
                       customer.c_acctbal            AS c_acctbal, 
                       customer.c_mktsegment         AS c_mktsegment, 
                       customer.c_comment            AS c_comment, 
                       _tmp_0e24131e.o_orderkey      AS o_orderkey, 
                       _tmp_0e24131e.o_custkey       AS o_custkey, 
                       _tmp_0e24131e.o_orderstatus   AS o_orderstatus, 
                       _tmp_0e24131e.o_totalprice    AS o_totalprice, 
                       _tmp_0e24131e.o_orderdate     AS o_orderdate, 
                       _tmp_0e24131e.o_orderpriority AS o_orderpriority, 
                       _tmp_0e24131e.o_clerk         AS o_clerk, 
                       _tmp_0e24131e.o_shippriority  AS o_shippriority, 
                       _tmp_0e24131e.o_comment       AS o_comment 
                FROM   (SELECT customer.c_custkey, 
                               customer.c_name, 
                               customer.c_address, 
                               customer.c_nationkey, 
                               customer.c_phone, 
                               customer.c_acctbal, 
                               customer.c_mktsegment, 
                               customer.c_comment 
                        FROM   customer) customer 
                       LEFT OUTER JOIN (SELECT o_orderkey, 
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
                                        WHERE  ( o_comment NOT LIKE 
                                                 '%special%requests%' )) 
                                                                  _tmp_0e24131e 
                                    ON customer.c_custkey = 
                                       _tmp_0e24131e.o_custkey) 
               _tmp_f82799dc 
        GROUP  BY c_custkey) _tmp_9c4685cb 
GROUP  BY c_count 
ORDER  BY custdist DESC, 
          c_count DESC 
