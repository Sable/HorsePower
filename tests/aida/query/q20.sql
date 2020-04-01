SELECT s_name    AS s_name, 
       s_address AS s_address 
FROM   (SELECT s_suppkey, 
               s_name, 
               s_address, 
               s_nationkey, 
               s_phone, 
               s_acctbal, 
               s_comment, 
               n_nationkey, 
               n_name, 
               n_regionkey, 
               n_comment 
        FROM   (SELECT supplier.s_suppkey        AS s_suppkey, 
                       supplier.s_name           AS s_name, 
                       supplier.s_address        AS s_address, 
                       supplier.s_nationkey      AS s_nationkey, 
                       supplier.s_phone          AS s_phone, 
                       supplier.s_acctbal        AS s_acctbal, 
                       supplier.s_comment        AS s_comment, 
                       _tmp_7f78522d.n_nationkey AS n_nationkey, 
                       _tmp_7f78522d.n_name      AS n_name, 
                       _tmp_7f78522d.n_regionkey AS n_regionkey, 
                       _tmp_7f78522d.n_comment   AS n_comment 
                FROM   (SELECT supplier.s_suppkey, 
                               supplier.s_name, 
                               supplier.s_address, 
                               supplier.s_nationkey, 
                               supplier.s_phone, 
                               supplier.s_acctbal, 
                               supplier.s_comment 
                        FROM   supplier) supplier 
                       INNER JOIN (SELECT n_nationkey, 
                                          n_name, 
                                          n_regionkey, 
                                          n_comment 
                                   FROM   (SELECT nation.n_nationkey, 
                                                  nation.n_name, 
                                                  nation.n_regionkey, 
                                                  nation.n_comment 
                                           FROM   nation) nation 
                                   WHERE  ( n_name = 'CANADA' )) _tmp_7f78522d 
                               ON supplier.s_nationkey = 
                                  _tmp_7f78522d.n_nationkey) 
               _tmp_bf1ae8b2 
        WHERE  ( s_suppkey IN (SELECT ps_suppkey AS ps_suppkey 
                               FROM   (SELECT ps_partkey, 
                                              ps_suppkey, 
                                              ps_availqty, 
                                              ps_supplycost, 
                                              ps_comment, 
                                              l_partkey, 
                                              l_suppkey, 
                                              totqty 
                                       FROM   (SELECT partsupp.ps_partkey     AS 
                                                      ps_partkey, 
                                                      partsupp.ps_suppkey     AS 
                                                      ps_suppkey, 
                                                      partsupp.ps_availqty    AS 
                                                      ps_availqty, 
                                                      partsupp.ps_supplycost  AS 
                                                      ps_supplycost, 
                                                      partsupp.ps_comment     AS 
                                                      ps_comment, 
                                                      _tmp_2d72f114.l_partkey AS 
                                                      l_partkey, 
                                                      _tmp_2d72f114.l_suppkey AS 
                                                      l_suppkey, 
                                                      _tmp_2d72f114.totqty    AS 
                                                      totqty 
                                               FROM 
                                      (SELECT partsupp.ps_partkey, 
                                              partsupp.ps_suppkey, 
                                              partsupp.ps_availqty, 
                                              partsupp.ps_supplycost, 
                                              partsupp.ps_comment 
                                       FROM   partsupp) partsupp 
                                      INNER JOIN (SELECT l_partkey 
                                                         AS 
                                                         l_partkey, 
                                                         l_suppkey 
                                                         AS 
                                                         l_suppkey, 
                                                         ( Cast(0.5 AS 
                                                           FLOAT) * 
                                                           sumqty ) AS 
                                                         totqty 
                                                  FROM 
                                      (SELECT l_partkey 
                                              AS l_partkey, 
                                              l_suppkey 
                                              AS l_suppkey, 
                                              Sum(l_quantity) 
                                              AS sumqty 
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
                                               FROM 
                                      (SELECT lineitem.l_orderkey, 
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
                                      WHERE  ( l_shipdate >= '1994-01-01' ) 
                                      AND ( l_shipdate < '1995-01-01' )) 
                                      _tmp_a17031d9 
                                      GROUP  BY l_partkey, 
                                      l_suppkey) _tmp_f05a5d18) _tmp_2d72f114 
                                      ON partsupp.ps_partkey = 
                                         _tmp_2d72f114.l_partkey 
                                      AND partsupp.ps_suppkey = 
                                              _tmp_2d72f114.l_suppkey) 
                                      _tmp_c8a7504a 
                                       WHERE  ( ps_availqty > totqty ) 
                                              AND ( ps_partkey IN (SELECT 
                                                    p_partkey AS p_partkey 
                                                                   FROM   ( 
                                                    SELECT 
                                                                  p_partkey, 
                                                                  p_name, 
                                                                  p_mfgr, 
                                                                  p_brand, 
                                                                  p_type, 
                                                                  p_size, 
                                                                  p_container, 
                                                                  p_retailprice, 
               p_comment 
                        FROM 
               (SELECT part.p_partkey, 
                       part.p_name, 
                       part.p_mfgr, 
                       part.p_brand, 
                       part.p_type, 
                       part.p_size, 
                       part.p_container, 
                       part.p_retailprice, 
                       part.p_comment 
                FROM   part) part 
                        WHERE  ( p_name LIKE 
                                 'forest%' )) 
                       _tmp_4ff7a460) )) 
               _tmp_7527349e) )) _tmp_bf854002 
ORDER  BY s_name 
