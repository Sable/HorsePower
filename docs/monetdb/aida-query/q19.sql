SELECT Sum(rev) AS revenue 
FROM   (SELECT ( l_extendedprice * ( 1 - l_discount ) ) AS rev 
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
                       l_comment, 
                       p_partkey, 
                       p_name, 
                       p_mfgr, 
                       p_brand, 
                       p_type, 
                       p_size, 
                       p_container, 
                       p_retailprice, 
                       p_comment 
                FROM   (SELECT lineitem.l_orderkey         AS l_orderkey, 
                               lineitem.l_partkey          AS l_partkey, 
                               lineitem.l_suppkey          AS l_suppkey, 
                               lineitem.l_linenumber       AS l_linenumber, 
                               lineitem.l_quantity         AS l_quantity, 
                               lineitem.l_extendedprice    AS l_extendedprice, 
                               lineitem.l_discount         AS l_discount, 
                               lineitem.l_tax              AS l_tax, 
                               lineitem.l_returnflag       AS l_returnflag, 
                               lineitem.l_linestatus       AS l_linestatus, 
                               lineitem.l_shipdate         AS l_shipdate, 
                               lineitem.l_commitdate       AS l_commitdate, 
                               lineitem.l_receiptdate      AS l_receiptdate, 
                               lineitem.l_shipinstruct     AS l_shipinstruct, 
                               lineitem.l_shipmode         AS l_shipmode, 
                               lineitem.l_comment          AS l_comment, 
                               _tmp_b8f4653a.p_partkey     AS p_partkey, 
                               _tmp_b8f4653a.p_name        AS p_name, 
                               _tmp_b8f4653a.p_mfgr        AS p_mfgr, 
                               _tmp_b8f4653a.p_brand       AS p_brand, 
                               _tmp_b8f4653a.p_type        AS p_type, 
                               _tmp_b8f4653a.p_size        AS p_size, 
                               _tmp_b8f4653a.p_container   AS p_container, 
                               _tmp_b8f4653a.p_retailprice AS p_retailprice, 
                               _tmp_b8f4653a.p_comment     AS p_comment 
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
                               CROSS JOIN (SELECT part.p_partkey, 
                                                  part.p_name, 
                                                  part.p_mfgr, 
                                                  part.p_brand, 
                                                  part.p_type, 
                                                  part.p_size, 
                                                  part.p_container, 
                                                  part.p_retailprice, 
                                                  part.p_comment 
                                           FROM   part) _tmp_b8f4653a) 
                       _tmp_a794540f 
                WHERE  ( ( ( ( ( ( ( ( ( ( ( p_partkey = l_partkey ) 
                                           AND ( p_brand = 'Brand#12' ) ) 
                                         AND ( p_container IN ( 
                                               'SM CASE', 'SM BOX', 
                                               'SM PACK', 
                                               'SM PKG' 
                                                              ) 
                                             ) 
                                                     ) 
                                       AND ( l_quantity >= 1 ) ) 
                                     AND ( l_quantity <= 11 ) ) 
                                   AND ( p_size >= 1 ) ) 
                                 AND ( p_size <= 5 ) ) 
                               AND ( l_shipmode IN ( 'AIR', 'AIR REG' ) ) ) 
                             AND ( l_shipinstruct = 'DELIVER IN PERSON' ) ) 
                            OR ( ( ( ( ( ( ( ( ( p_partkey = l_partkey ) 
                                               AND ( p_brand = 'Brand#23' ) ) 
                                             AND ( p_container IN ( 
                                                   'MED BAG', 'MED BOX', 
                                                   'MED PKG', 
                                                   'MED PACK' 
                                                                  ) 
                                                 ) 
                                                   ) 
                                           AND ( l_quantity >= 10 ) ) 
                                         AND ( l_quantity <= 20 ) ) 
                                       AND ( p_size >= 1 ) ) 
                                     AND ( p_size <= 10 ) ) 
                                   AND ( l_shipmode IN ( 'AIR', 'AIR REG' ) ) ) 
                                 AND ( l_shipinstruct = 'DELIVER IN PERSON' ) ) 
                         ) 
                          OR ( ( ( ( ( ( ( ( ( p_partkey = l_partkey ) 
                                             AND ( p_brand = 'Brand#34' ) ) 
                                           AND ( p_container IN ( 
                                                 'LG CASE', 'LG BOX', 
                                                 'LG PACK', 
                                                 'LG PKG' ) 
                                               ) 
                                               ) 
                                         AND ( l_quantity >= 20 ) ) 
                                       AND ( l_quantity <= 30 ) ) 
                                     AND ( p_size >= 1 ) ) 
                                   AND ( p_size <= 15 ) ) 
                                 AND ( l_shipmode IN ( 'AIR', 'AIR REG' ) ) ) 
                               AND ( l_shipinstruct = 'DELIVER IN PERSON' ) ) )) 
               _tmp_4303e1b9) _tmp_b12c7440 
