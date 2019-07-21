SELECT ps_partkey, 
       value 
FROM   (SELECT ps_partkey AS ps_partkey, 
               Sum(val)   AS value 
        FROM   (SELECT ps_partkey                      AS ps_partkey, 
                       ( ps_supplycost * ps_availqty ) AS val 
                FROM   (SELECT _tmp_d80da789.ps_partkey    AS ps_partkey, 
                               _tmp_d80da789.ps_suppkey    AS ps_suppkey, 
                               _tmp_d80da789.ps_availqty   AS ps_availqty, 
                               _tmp_d80da789.ps_supplycost AS ps_supplycost, 
                               _tmp_d80da789.ps_comment    AS ps_comment, 
                               _tmp_d80da789.s_suppkey     AS s_suppkey, 
                               _tmp_d80da789.s_name        AS s_name, 
                               _tmp_d80da789.s_address     AS s_address, 
                               _tmp_d80da789.s_nationkey   AS s_nationkey, 
                               _tmp_d80da789.s_phone       AS s_phone, 
                               _tmp_d80da789.s_acctbal     AS s_acctbal, 
                               _tmp_d80da789.s_comment     AS s_comment, 
                               _tmp_a8d64373.n_nationkey   AS n_nationkey, 
                               _tmp_a8d64373.n_name        AS n_name, 
                               _tmp_a8d64373.n_regionkey   AS n_regionkey, 
                               _tmp_a8d64373.n_comment     AS n_comment 
                        FROM   (SELECT partsupp.ps_partkey       AS ps_partkey, 
                                       partsupp.ps_suppkey       AS ps_suppkey, 
                                       partsupp.ps_availqty      AS ps_availqty, 
                                       partsupp.ps_supplycost    AS 
                                       ps_supplycost, 
                                       partsupp.ps_comment       AS ps_comment, 
                                       _tmp_95aaaba2.s_suppkey   AS s_suppkey, 
                                       _tmp_95aaaba2.s_name      AS s_name, 
                                       _tmp_95aaaba2.s_address   AS s_address, 
                                       _tmp_95aaaba2.s_nationkey AS s_nationkey, 
                                       _tmp_95aaaba2.s_phone     AS s_phone, 
                                       _tmp_95aaaba2.s_acctbal   AS s_acctbal, 
                                       _tmp_95aaaba2.s_comment   AS s_comment 
                                FROM   (SELECT partsupp.ps_partkey, 
                                               partsupp.ps_suppkey, 
                                               partsupp.ps_availqty, 
                                               partsupp.ps_supplycost, 
                                               partsupp.ps_comment 
                                        FROM   partsupp) partsupp 
                                       INNER JOIN (SELECT supplier.s_suppkey, 
                                                          supplier.s_name, 
                                                          supplier.s_address, 
                                                          supplier.s_nationkey, 
                                                          supplier.s_phone, 
                                                          supplier.s_acctbal, 
                                                          supplier.s_comment 
                                                   FROM   supplier) 
                                                  _tmp_95aaaba2 
                                               ON partsupp.ps_suppkey = 
                               _tmp_95aaaba2.s_suppkey) 
                               _tmp_d80da789 
                               INNER JOIN (SELECT n_nationkey, 
                                                  n_name, 
                                                  n_regionkey, 
                                                  n_comment 
                                           FROM   (SELECT nation.n_nationkey, 
                                                          nation.n_name, 
                                                          nation.n_regionkey, 
                                                          nation.n_comment 
                                                   FROM   nation) nation 
                                           WHERE  ( n_name = 'GERMANY' )) 
                                          _tmp_a8d64373 
                                       ON _tmp_d80da789.s_nationkey = 
                       _tmp_a8d64373.n_nationkey) 
                       _tmp_026b9f73) _tmp_27f0caaa 
        GROUP  BY ps_partkey) _tmp_003ef52f 
WHERE  ( value > (SELECT ( ( sum_totsupcost * Cast(0.0001 AS FLOAT) ) / 
                                  Cast(1.0 AS FLOAT) ) AS 
                         sumtotsupcost 
                  FROM   (SELECT Sum(totsupcost) AS sum_totsupcost 
                          FROM   (SELECT ( ps_supplycost * ps_availqty ) AS 
                                         totsupcost 
                                  FROM   (SELECT _tmp_d80da789.ps_partkey    AS 
                                                 ps_partkey, 
                                                 _tmp_d80da789.ps_suppkey    AS 
                                                 ps_suppkey, 
                                                 _tmp_d80da789.ps_availqty   AS 
                                                 ps_availqty, 
                                                 _tmp_d80da789.ps_supplycost AS 
                                                 ps_supplycost, 
                                                 _tmp_d80da789.ps_comment    AS 
                                                 ps_comment, 
                                                 _tmp_d80da789.s_suppkey     AS 
                                                 s_suppkey 
                                                 , 
                                 _tmp_d80da789.s_name        AS s_name, 
                                 _tmp_d80da789.s_address     AS s_address, 
                                 _tmp_d80da789.s_nationkey   AS s_nationkey, 
                                 _tmp_d80da789.s_phone       AS s_phone, 
                                 _tmp_d80da789.s_acctbal     AS s_acctbal, 
                                 _tmp_d80da789.s_comment     AS s_comment, 
                                 _tmp_a8d64373.n_nationkey   AS n_nationkey, 
                                 _tmp_a8d64373.n_name        AS n_name, 
                                 _tmp_a8d64373.n_regionkey   AS n_regionkey, 
                                 _tmp_a8d64373.n_comment     AS n_comment 
                                          FROM   (SELECT 
                                         partsupp.ps_partkey 
                                         AS 
                                         ps_partkey, 
                                         partsupp.ps_suppkey 
                                         AS 
                                         ps_suppkey, 
                                         partsupp.ps_availqty 
                                         AS 
                                         ps_availqty 
                                         , 
                                         partsupp.ps_supplycost    AS 
                                         ps_supplycost 
                                         , 
                         partsupp.ps_comment       AS ps_comment, 
                         _tmp_95aaaba2.s_suppkey   AS s_suppkey, 
                         _tmp_95aaaba2.s_name      AS s_name, 
                         _tmp_95aaaba2.s_address   AS s_address, 
                         _tmp_95aaaba2.s_nationkey AS s_nationkey, 
                         _tmp_95aaaba2.s_phone     AS s_phone, 
                         _tmp_95aaaba2.s_acctbal   AS s_acctbal, 
                         _tmp_95aaaba2.s_comment   AS s_comment 
                                  FROM   (SELECT partsupp.ps_partkey, 
                                                 partsupp.ps_suppkey, 
                                                 partsupp.ps_availqty, 
                                                 partsupp.ps_supplycost 
                                                 , 
       partsupp.ps_comment 
       FROM   partsupp) partsupp 
       INNER JOIN 
       (SELECT supplier.s_suppkey, 
       supplier.s_name, 
       supplier.s_address, 
       supplier.s_nationkey, 
       supplier.s_phone, 
       supplier.s_acctbal, 
       supplier.s_comment 
       FROM   supplier) 
       _tmp_95aaaba2 
       ON partsupp.ps_suppkey = 
       _tmp_95aaaba2.s_suppkey) 
       _tmp_d80da789 
       INNER JOIN (SELECT n_nationkey, 
          n_name, 
          n_regionkey, 
          n_comment 
       FROM 
       (SELECT nation.n_nationkey, 
       nation.n_name, 
       nation.n_regionkey, 
       nation.n_comment 
       FROM   nation) nation 
       WHERE  ( n_name = 'GERMANY' 
          )) 
       _tmp_a8d64373 
       ON _tmp_d80da789.s_nationkey = 
       _tmp_a8d64373.n_nationkey) 
       _tmp_026b9f73) _tmp_5387203d) _tmp_a9b0dcd1) ) 
ORDER  BY value DESC 
