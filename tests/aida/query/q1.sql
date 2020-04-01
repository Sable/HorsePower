SELECT l_returnflag         AS l_returnflag, 
       l_linestatus         AS l_linestatus, 
       Sum(l_quantity)      AS sum_qty, 
       Sum(l_extendedprice) AS sum_base_price, 
       Sum(disc_price)      AS sum_disc_price, 
       Sum(charge)          AS sum_charge, 
       Avg(l_quantity)      AS avg_qty, 
       Avg(l_extendedprice) AS avg_price, 
       Avg(l_discount)      AS avg_disc, 
       Count(*)             AS count_order 
FROM   (SELECT l_returnflag                                                 AS 
                      l_returnflag, 
               l_linestatus                                                 AS 
                      l_linestatus, 
               l_quantity                                                   AS 
                      l_quantity, 
               l_extendedprice                                              AS 
                      l_extendedprice, 
               ( l_extendedprice * ( 1 - l_discount ) )                     AS 
                      disc_price, 
               ( ( l_extendedprice * ( 1 - l_discount ) ) * ( 1 + l_tax ) ) AS 
               charge, 
               l_discount                                                   AS 
                      l_discount 
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
                WHERE  ( l_shipdate <= '1998-09-02' )) _tmp_f0503b15) 
       _tmp_7b54caa9 
GROUP  BY l_returnflag, 
          l_linestatus 
ORDER  BY l_returnflag, 
          l_linestatus 
