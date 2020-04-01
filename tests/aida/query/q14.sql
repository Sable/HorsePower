SELECT ( ( Cast(100.0 AS FLOAT) * sum_revenue1 ) / sum_revenue2 ) AS 
       promo_revenue 
FROM   (SELECT Sum(revenue1) AS sum_revenue1, 
               Sum(revenue2) AS sum_revenue2 
        FROM   (SELECT CASE 
                         WHEN ( p_type LIKE 'PROMO%' ) THEN ( 
                         l_extendedprice * ( 1 - l_discount ) ) 
                         ELSE NULL 
                       END                                      AS revenue1, 
                       ( l_extendedprice * ( 1 - l_discount ) ) AS revenue2 
                FROM   (SELECT _tmp_eb6395cf.l_orderkey      AS l_orderkey, 
                               _tmp_eb6395cf.l_partkey       AS l_partkey, 
                               _tmp_eb6395cf.l_suppkey       AS l_suppkey, 
                               _tmp_eb6395cf.l_linenumber    AS l_linenumber, 
                               _tmp_eb6395cf.l_quantity      AS l_quantity, 
                               _tmp_eb6395cf.l_extendedprice AS l_extendedprice, 
                               _tmp_eb6395cf.l_discount      AS l_discount, 
                               _tmp_eb6395cf.l_tax           AS l_tax, 
                               _tmp_eb6395cf.l_returnflag    AS l_returnflag, 
                               _tmp_eb6395cf.l_linestatus    AS l_linestatus, 
                               _tmp_eb6395cf.l_shipdate      AS l_shipdate, 
                               _tmp_eb6395cf.l_commitdate    AS l_commitdate, 
                               _tmp_eb6395cf.l_receiptdate   AS l_receiptdate, 
                               _tmp_eb6395cf.l_shipinstruct  AS l_shipinstruct, 
                               _tmp_eb6395cf.l_shipmode      AS l_shipmode, 
                               _tmp_eb6395cf.l_comment       AS l_comment, 
                               part.p_partkey                AS p_partkey, 
                               part.p_name                   AS p_name, 
                               part.p_mfgr                   AS p_mfgr, 
                               part.p_brand                  AS p_brand, 
                               part.p_type                   AS p_type, 
                               part.p_size                   AS p_size, 
                               part.p_container              AS p_container, 
                               part.p_retailprice            AS p_retailprice, 
                               part.p_comment                AS p_comment 
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
                                WHERE  ( l_shipdate >= '1995-09-01' ) 
                                       AND ( l_shipdate < '1995-10-01' )) 
                               _tmp_eb6395cf 
                               INNER JOIN (SELECT part.p_partkey, 
                                                  part.p_name, 
                                                  part.p_mfgr, 
                                                  part.p_brand, 
                                                  part.p_type, 
                                                  part.p_size, 
                                                  part.p_container, 
                                                  part.p_retailprice, 
                                                  part.p_comment 
                                           FROM   part) part 
                                       ON _tmp_eb6395cf.l_partkey = 
                                          part.p_partkey) 
                       _tmp_372e312a) _tmp_20283d9f) _tmp_c3a09dec 
