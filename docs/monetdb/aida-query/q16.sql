SELECT p_brand                    AS p_brand, 
       p_type                     AS p_type, 
       p_size                     AS p_size, 
       Count(DISTINCT ps_suppkey) AS supplier_cnt 
FROM   (SELECT _tmp_4ed01870.p_partkey     AS p_partkey, 
               _tmp_4ed01870.p_name        AS p_name, 
               _tmp_4ed01870.p_mfgr        AS p_mfgr, 
               _tmp_4ed01870.p_brand       AS p_brand, 
               _tmp_4ed01870.p_type        AS p_type, 
               _tmp_4ed01870.p_size        AS p_size, 
               _tmp_4ed01870.p_container   AS p_container, 
               _tmp_4ed01870.p_retailprice AS p_retailprice, 
               _tmp_4ed01870.p_comment     AS p_comment, 
               _tmp_3c7d3b27.ps_partkey    AS ps_partkey, 
               _tmp_3c7d3b27.ps_suppkey    AS ps_suppkey, 
               _tmp_3c7d3b27.ps_availqty   AS ps_availqty, 
               _tmp_3c7d3b27.ps_supplycost AS ps_supplycost, 
               _tmp_3c7d3b27.ps_comment    AS ps_comment 
        FROM   (SELECT p_partkey, 
                       p_name, 
                       p_mfgr, 
                       p_brand, 
                       p_type, 
                       p_size, 
                       p_container, 
                       p_retailprice, 
                       p_comment 
                FROM   (SELECT part.p_partkey, 
                               part.p_name, 
                               part.p_mfgr, 
                               part.p_brand, 
                               part.p_type, 
                               part.p_size, 
                               part.p_container, 
                               part.p_retailprice, 
                               part.p_comment 
                        FROM   part) part 
                WHERE  ( p_brand <> 'Brand#45' ) 
                       AND ( p_type NOT LIKE 'MEDIUM POLISHED%' ) 
                       AND ( p_size IN ( 49, 14, 23, 45, 
                                         19, 3, 36, 9 ) )) _tmp_4ed01870 
               INNER JOIN (SELECT ps_partkey, 
                                  ps_suppkey, 
                                  ps_availqty, 
                                  ps_supplycost, 
                                  ps_comment 
                           FROM   (SELECT partsupp.ps_partkey, 
                                          partsupp.ps_suppkey, 
                                          partsupp.ps_availqty, 
                                          partsupp.ps_supplycost, 
                                          partsupp.ps_comment 
                                   FROM   partsupp) partsupp 
                           WHERE  ( ps_suppkey NOT IN (SELECT 
                                    s_suppkey AS s_suppkey 
                                                       FROM 
                                    (SELECT s_suppkey, 
                                            s_name, 
                                            s_address, 
                                            s_nationkey, 
                                            s_phone, 
                                            s_acctbal, 
                                            s_comment 
                                     FROM 
                                                      (SELECT 
                                    supplier.s_suppkey, 
supplier.s_name, 
supplier.s_address, 
supplier.s_nationkey, 
supplier.s_phone, 
supplier.s_acctbal, 
supplier.s_comment 
                   FROM   supplier) supplier 
                           WHERE  ( s_comment LIKE 
'%Customer%Complaints%' 
                                  )) 
                          _tmp_e7cbf350) )) 
_tmp_3c7d3b27 
ON _tmp_4ed01870.p_partkey = _tmp_3c7d3b27.ps_partkey) 
_tmp_5a5728d2 
GROUP  BY p_brand, 
          p_type, 
          p_size 
ORDER  BY supplier_cnt DESC, 
          p_brand, 
          p_type, 
          p_size 
