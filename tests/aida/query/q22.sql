SELECT   cntrycode      AS cntrycode, 
         Count(*)       AS numcust, 
         Sum(c_acctbal) AS totacctbal 
FROM     ( 
                SELECT c_acctbal , 
                       cntrycode , 
                       c_custkey 
                FROM   ( 
                              SELECT c_acctbal , 
                                     cntrycode , 
                                     c_custkey 
                              FROM   ( 
                                            SELECT c_acctbal                       AS c_acctbal, 
                                                   Substring(c_phone FROM 1 FOR 2) AS cntrycode, 
                                                   c_custkey                       AS c_custkey 
                                            FROM   ( 
                                                          SELECT customer.c_custkey , 
                                                                 customer.c_name , 
                                                                 customer.c_address , 
                                                                 customer.c_nationkey , 
                                                                 customer.c_phone , 
                                                                 customer.c_acctbal , 
                                                                 customer.c_mktsegment , 
                                                                 customer.c_comment 
                                                          FROM   customer) customer) _tmp_16b2b685
                              WHERE  ( 
                                            cntrycode IN ('13', 
                                                          '31', 
                                                          '23', 
                                                          '29', 
                                                          '30', 
                                                          '18', 
                                                          '17'))) _tmp_783ea3ad 
                WHERE  ( 
                              c_acctbal > 
                              ( 
                                     SELECT Avg(c_acctbal) AS avg_acctbal 
                                     FROM   ( 
                                                   SELECT c_acctbal , 
                                                          cntrycode , 
                                                          c_custkey 
                                                   FROM   ( 
                                                                 SELECT c_acctbal , 
                                                                        cntrycode , 
                                                                        c_custkey 
                                                                 FROM   ( 
                                                                               SELECT c_acctbal                       AS c_acctbal,
                                                                                      Substring(c_phone FROM 1 FOR 2) AS cntrycode,
                                                                                      c_custkey                       AS c_custkey
                                                                               FROM   ( 
                                                                                             SELECT customer.c_custkey ,
                                                                                                    customer.c_name ,
                                                                                                    customer.c_address ,
                                                                                                    customer.c_nationkey ,
                                                                                                    customer.c_phone ,
                                                                                                    customer.c_acctbal ,
                                                                                                    customer.c_mktsegment ,
                                                                                                    customer.c_comment
                                                                                             FROM   customer) customer) _tmp_16b2b685
                                                                 WHERE  ( 
                                                                               cntrycode IN ('13',
                                                                                             '31',
                                                                                             '23',
                                                                                             '29',
                                                                                             '30',
                                                                                             '18',
                                                                                             '17'))) _tmp_783ea3ad
                                                   WHERE  ( 
                                                                 c_acctbal > cast(0.0 AS float))) _tmp_de04ecbe))
                AND    ( 
                              c_custkey NOT IN 
                              ( 
                                     SELECT o_custkey AS o_custkey 
                                     FROM   ( 
                                                   SELECT orders.o_orderkey , 
                                                          orders.o_custkey , 
                                                          orders.o_orderstatus , 
                                                          orders.o_totalprice , 
                                                          orders.o_orderdate , 
                                                          orders.o_orderpriority , 
                                                          orders.o_clerk , 
                                                          orders.o_shippriority , 
                                                          orders.o_comment 
                                                   FROM   orders) orders))) _tmp_79fea305 
GROUP BY cntrycode 
ORDER BY cntrycode
