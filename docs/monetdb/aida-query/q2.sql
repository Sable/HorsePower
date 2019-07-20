SELECT s_acctbal AS s_acctbal, 
       s_name    AS s_name, 
       n_name    AS n_name, 
       p_partkey AS p_partkey, 
       p_mfgr    AS p_mfgr, 
       s_address AS s_address, 
       s_phone   AS s_phone, 
       s_comment AS s_comment 
FROM   (SELECT _tmp_960ae5c2.ps_partkey      AS ps_partkey, 
               _tmp_960ae5c2.ps_suppkey      AS ps_suppkey, 
               _tmp_960ae5c2.ps_availqty     AS ps_availqty, 
               _tmp_960ae5c2.ps_supplycost   AS ps_supplycost, 
               _tmp_960ae5c2.ps_comment      AS ps_comment, 
               _tmp_960ae5c2.s_suppkey       AS s_suppkey, 
               _tmp_960ae5c2.s_name          AS s_name, 
               _tmp_960ae5c2.s_address       AS s_address, 
               _tmp_960ae5c2.s_nationkey     AS s_nationkey, 
               _tmp_960ae5c2.s_phone         AS s_phone, 
               _tmp_960ae5c2.s_acctbal       AS s_acctbal, 
               _tmp_960ae5c2.s_comment       AS s_comment, 
               _tmp_960ae5c2.n_nationkey     AS n_nationkey, 
               _tmp_960ae5c2.n_name          AS n_name, 
               _tmp_960ae5c2.n_regionkey     AS n_regionkey, 
               _tmp_960ae5c2.n_comment       AS n_comment, 
               _tmp_960ae5c2.r_regionkey     AS r_regionkey, 
               _tmp_960ae5c2.r_name          AS r_name, 
               _tmp_960ae5c2.r_comment       AS r_comment, 
               _tmp_960ae5c2.p_partkey       AS p_partkey, 
               _tmp_960ae5c2.p_name          AS p_name, 
               _tmp_960ae5c2.p_mfgr          AS p_mfgr, 
               _tmp_960ae5c2.p_brand         AS p_brand, 
               _tmp_960ae5c2.p_type          AS p_type, 
               _tmp_960ae5c2.p_size          AS p_size, 
               _tmp_960ae5c2.p_container     AS p_container, 
               _tmp_960ae5c2.p_retailprice   AS p_retailprice, 
               _tmp_960ae5c2.p_comment       AS p_comment, 
               _tmp_751b3bb8.i_partkey       AS i_partkey, 
               _tmp_751b3bb8.min_supply_cost AS min_supply_cost 
        FROM   (SELECT _tmp_ca4d427b.ps_partkey    AS ps_partkey, 
                       _tmp_ca4d427b.ps_suppkey    AS ps_suppkey, 
                       _tmp_ca4d427b.ps_availqty   AS ps_availqty, 
                       _tmp_ca4d427b.ps_supplycost AS ps_supplycost, 
                       _tmp_ca4d427b.ps_comment    AS ps_comment, 
                       _tmp_ca4d427b.s_suppkey     AS s_suppkey, 
                       _tmp_ca4d427b.s_name        AS s_name, 
                       _tmp_ca4d427b.s_address     AS s_address, 
                       _tmp_ca4d427b.s_nationkey   AS s_nationkey, 
                       _tmp_ca4d427b.s_phone       AS s_phone, 
                       _tmp_ca4d427b.s_acctbal     AS s_acctbal, 
                       _tmp_ca4d427b.s_comment     AS s_comment, 
                       _tmp_ca4d427b.n_nationkey   AS n_nationkey, 
                       _tmp_ca4d427b.n_name        AS n_name, 
                       _tmp_ca4d427b.n_regionkey   AS n_regionkey, 
                       _tmp_ca4d427b.n_comment     AS n_comment, 
                       _tmp_ca4d427b.r_regionkey   AS r_regionkey, 
                       _tmp_ca4d427b.r_name        AS r_name, 
                       _tmp_ca4d427b.r_comment     AS r_comment, 
                       _tmp_2724aaee.p_partkey     AS p_partkey, 
                       _tmp_2724aaee.p_name        AS p_name, 
                       _tmp_2724aaee.p_mfgr        AS p_mfgr, 
                       _tmp_2724aaee.p_brand       AS p_brand, 
                       _tmp_2724aaee.p_type        AS p_type, 
                       _tmp_2724aaee.p_size        AS p_size, 
                       _tmp_2724aaee.p_container   AS p_container, 
                       _tmp_2724aaee.p_retailprice AS p_retailprice, 
                       _tmp_2724aaee.p_comment     AS p_comment 
                FROM   (SELECT _tmp_1cec7082.ps_partkey    AS ps_partkey, 
                               _tmp_1cec7082.ps_suppkey    AS ps_suppkey, 
                               _tmp_1cec7082.ps_availqty   AS ps_availqty, 
                               _tmp_1cec7082.ps_supplycost AS ps_supplycost, 
                               _tmp_1cec7082.ps_comment    AS ps_comment, 
                               _tmp_1cec7082.s_suppkey     AS s_suppkey, 
                               _tmp_1cec7082.s_name        AS s_name, 
                               _tmp_1cec7082.s_address     AS s_address, 
                               _tmp_1cec7082.s_nationkey   AS s_nationkey, 
                               _tmp_1cec7082.s_phone       AS s_phone, 
                               _tmp_1cec7082.s_acctbal     AS s_acctbal, 
                               _tmp_1cec7082.s_comment     AS s_comment, 
                               _tmp_1cec7082.n_nationkey   AS n_nationkey, 
                               _tmp_1cec7082.n_name        AS n_name, 
                               _tmp_1cec7082.n_regionkey   AS n_regionkey, 
                               _tmp_1cec7082.n_comment     AS n_comment, 
                               _tmp_7c4b1c89.r_regionkey   AS r_regionkey, 
                               _tmp_7c4b1c89.r_name        AS r_name, 
                               _tmp_7c4b1c89.r_comment     AS r_comment 
                        FROM   (SELECT _tmp_2397cb6f.ps_partkey    AS ps_partkey 
                                       , 
                       _tmp_2397cb6f.ps_suppkey    AS ps_suppkey, 
                       _tmp_2397cb6f.ps_availqty   AS ps_availqty, 
                       _tmp_2397cb6f.ps_supplycost AS ps_supplycost, 
                       _tmp_2397cb6f.ps_comment    AS ps_comment, 
                       _tmp_2397cb6f.s_suppkey     AS s_suppkey, 
                       _tmp_2397cb6f.s_name        AS s_name, 
                       _tmp_2397cb6f.s_address     AS s_address, 
                       _tmp_2397cb6f.s_nationkey   AS s_nationkey, 
                       _tmp_2397cb6f.s_phone       AS s_phone, 
                       _tmp_2397cb6f.s_acctbal     AS s_acctbal, 
                       _tmp_2397cb6f.s_comment     AS s_comment, 
                       nation.n_nationkey          AS n_nationkey, 
                       nation.n_name               AS n_name, 
                       nation.n_regionkey          AS n_regionkey, 
                       nation.n_comment            AS n_comment 
                                FROM   (SELECT partsupp.ps_partkey       AS 
                                               ps_partkey, 
                                               partsupp.ps_suppkey       AS 
                                               ps_suppkey, 
                                               partsupp.ps_availqty      AS 
                                               ps_availqty 
                                               , 
                               partsupp.ps_supplycost    AS ps_supplycost, 
                               partsupp.ps_comment       AS ps_comment, 
                               _tmp_1630ebee.s_suppkey   AS s_suppkey, 
                               _tmp_1630ebee.s_name      AS s_name, 
                               _tmp_1630ebee.s_address   AS s_address, 
                               _tmp_1630ebee.s_nationkey AS s_nationkey, 
                               _tmp_1630ebee.s_phone     AS s_phone, 
                               _tmp_1630ebee.s_acctbal   AS s_acctbal, 
                               _tmp_1630ebee.s_comment   AS s_comment 
                                        FROM   (SELECT partsupp.ps_partkey, 
                                                       partsupp.ps_suppkey, 
                                                       partsupp.ps_availqty, 
                                                       partsupp.ps_supplycost, 
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
                                               _tmp_1630ebee 
                                                       ON partsupp.ps_suppkey = 
                                       _tmp_1630ebee.s_suppkey) 
                                       _tmp_2397cb6f 
                                       INNER JOIN (SELECT nation.n_nationkey, 
                                                          nation.n_name, 
                                                          nation.n_regionkey, 
                                                          nation.n_comment 
                                                   FROM   nation) nation 
                                               ON _tmp_2397cb6f.s_nationkey = 
                                                  nation.n_nationkey) 
                               _tmp_1cec7082 
                               INNER JOIN (SELECT r_regionkey, 
                                                  r_name, 
                                                  r_comment 
                                           FROM   (SELECT region.r_regionkey, 
                                                          region.r_name, 
                                                          region.r_comment 
                                                   FROM   region) region 
                                           WHERE  ( r_name = 'EUROPE' )) 
                                          _tmp_7c4b1c89 
                                       ON _tmp_1cec7082.n_regionkey = 
                       _tmp_7c4b1c89.r_regionkey) 
                       _tmp_ca4d427b 
                       INNER JOIN (SELECT p_partkey, 
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
                                   WHERE  ( p_size = 15 ) 
                                          AND ( p_type LIKE '%BRASS' )) 
                                  _tmp_2724aaee 
                               ON _tmp_ca4d427b.ps_partkey = 
                                  _tmp_2724aaee.p_partkey) 
               _tmp_960ae5c2 
               INNER JOIN (SELECT ps_partkey         AS i_partkey, 
                                  Min(ps_supplycost) AS min_supply_cost 
                           FROM   (SELECT _tmp_1cec7082.ps_partkey    AS 
                                          ps_partkey, 
                                          _tmp_1cec7082.ps_suppkey    AS 
                                          ps_suppkey, 
                                          _tmp_1cec7082.ps_availqty   AS 
                                          ps_availqty, 
                                          _tmp_1cec7082.ps_supplycost AS 
                                          ps_supplycost, 
                                          _tmp_1cec7082.ps_comment    AS 
                                          ps_comment, 
                                          _tmp_1cec7082.s_suppkey     AS 
                                          s_suppkey, 
                                          _tmp_1cec7082.s_name        AS s_name, 
                                          _tmp_1cec7082.s_address     AS 
                                          s_address, 
                                          _tmp_1cec7082.s_nationkey   AS 
                                          s_nationkey, 
                                          _tmp_1cec7082.s_phone       AS s_phone 
                                          , 
_tmp_1cec7082.s_acctbal     AS s_acctbal, 
_tmp_1cec7082.s_comment     AS s_comment, 
_tmp_1cec7082.n_nationkey   AS n_nationkey, 
_tmp_1cec7082.n_name        AS n_name, 
_tmp_1cec7082.n_regionkey   AS n_regionkey, 
_tmp_1cec7082.n_comment     AS n_comment, 
_tmp_7c4b1c89.r_regionkey   AS r_regionkey, 
_tmp_7c4b1c89.r_name        AS r_name, 
_tmp_7c4b1c89.r_comment     AS r_comment 
FROM   (SELECT _tmp_2397cb6f.ps_partkey    AS 
        ps_partkey, 
        _tmp_2397cb6f.ps_suppkey    AS 
        ps_suppkey, 
        _tmp_2397cb6f.ps_availqty   AS 
        ps_availqty, 
        _tmp_2397cb6f.ps_supplycost AS 
        ps_supplycost, 
        _tmp_2397cb6f.ps_comment    AS 
        ps_comment, 
        _tmp_2397cb6f.s_suppkey     AS 
        s_suppkey, 
        _tmp_2397cb6f.s_name        AS s_name 
        , 
_tmp_2397cb6f.s_address     AS s_address, 
_tmp_2397cb6f.s_nationkey   AS s_nationkey, 
_tmp_2397cb6f.s_phone       AS s_phone, 
_tmp_2397cb6f.s_acctbal     AS s_acctbal, 
_tmp_2397cb6f.s_comment     AS s_comment, 
nation.n_nationkey          AS n_nationkey, 
nation.n_name               AS n_name, 
nation.n_regionkey          AS n_regionkey, 
nation.n_comment            AS n_comment 
FROM   (SELECT partsupp.ps_partkey       AS 
ps_partkey, 
partsupp.ps_suppkey       AS 
ps_suppkey, 
partsupp.ps_availqty      AS 
ps_availqty, 
partsupp.ps_supplycost    AS 
ps_supplycost, 
partsupp.ps_comment       AS 
ps_comment, 
_tmp_1630ebee.s_suppkey   AS 
s_suppkey, 
_tmp_1630ebee.s_name      AS s_name, 
_tmp_1630ebee.s_address   AS 
s_address, 
_tmp_1630ebee.s_nationkey AS 
s_nationkey, 
_tmp_1630ebee.s_phone     AS s_phone, 
_tmp_1630ebee.s_acctbal   AS 
s_acctbal, 
_tmp_1630ebee.s_comment   AS 
s_comment 
FROM   (SELECT partsupp.ps_partkey, 
partsupp.ps_suppkey, 
partsupp.ps_availqty, 
partsupp.ps_supplycost, 
partsupp.ps_comment 
FROM   partsupp) partsupp 
INNER JOIN (SELECT 
supplier.s_suppkey, 
supplier.s_name, 
supplier.s_address, 
supplier.s_nationkey, 
supplier.s_phone, 
supplier.s_acctbal, 
supplier.s_comment 
FROM   supplier) _tmp_1630ebee 
ON partsupp.ps_suppkey = 
_tmp_1630ebee.s_suppkey) 
_tmp_2397cb6f 
INNER JOIN (SELECT nation.n_nationkey, 
nation.n_name, 
nation.n_regionkey, 
nation.n_comment 
FROM   nation) nation 
ON _tmp_2397cb6f.s_nationkey = 
nation.n_nationkey) 
_tmp_1cec7082 
INNER JOIN (SELECT r_regionkey, 
r_name, 
r_comment 
FROM   (SELECT region.r_regionkey, 
region.r_name, 
region.r_comment 
FROM   region) region 
WHERE  ( r_name = 'EUROPE' )) _tmp_7c4b1c89 
ON _tmp_1cec7082.n_regionkey = 
_tmp_7c4b1c89.r_regionkey) 
_tmp_ca4d427b 
GROUP  BY ps_partkey) _tmp_751b3bb8 
ON _tmp_960ae5c2.ps_partkey = _tmp_751b3bb8.i_partkey 
AND _tmp_960ae5c2.ps_supplycost = _tmp_751b3bb8.min_supply_cost) _tmp_2a955452 
ORDER  BY s_acctbal DESC, 
          n_name, 
          s_name, 
          p_partkey 
LIMIT  100 
