SELECT ( tot_price / Cast(7.0 AS FLOAT) ) AS avg_yearly 
FROM   (SELECT Sum(l_extendedprice) AS tot_price 
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
                       p_comment, 
                       i_partkey, 
                       avg_qty 
                FROM   (SELECT _tmp_ef0e6f52.l_orderkey      AS l_orderkey, 
                               _tmp_ef0e6f52.l_partkey       AS l_partkey, 
                               _tmp_ef0e6f52.l_suppkey       AS l_suppkey, 
                               _tmp_ef0e6f52.l_linenumber    AS l_linenumber, 
                               _tmp_ef0e6f52.l_quantity      AS l_quantity, 
                               _tmp_ef0e6f52.l_extendedprice AS l_extendedprice, 
                               _tmp_ef0e6f52.l_discount      AS l_discount, 
                               _tmp_ef0e6f52.l_tax           AS l_tax, 
                               _tmp_ef0e6f52.l_returnflag    AS l_returnflag, 
                               _tmp_ef0e6f52.l_linestatus    AS l_linestatus, 
                               _tmp_ef0e6f52.l_shipdate      AS l_shipdate, 
                               _tmp_ef0e6f52.l_commitdate    AS l_commitdate, 
                               _tmp_ef0e6f52.l_receiptdate   AS l_receiptdate, 
                               _tmp_ef0e6f52.l_shipinstruct  AS l_shipinstruct, 
                               _tmp_ef0e6f52.l_shipmode      AS l_shipmode, 
                               _tmp_ef0e6f52.l_comment       AS l_comment, 
                               _tmp_ef0e6f52.p_partkey       AS p_partkey, 
                               _tmp_ef0e6f52.p_name          AS p_name, 
                               _tmp_ef0e6f52.p_mfgr          AS p_mfgr, 
                               _tmp_ef0e6f52.p_brand         AS p_brand, 
                               _tmp_ef0e6f52.p_type          AS p_type, 
                               _tmp_ef0e6f52.p_size          AS p_size, 
                               _tmp_ef0e6f52.p_container     AS p_container, 
                               _tmp_ef0e6f52.p_retailprice   AS p_retailprice, 
                               _tmp_ef0e6f52.p_comment       AS p_comment, 
                               _tmp_67d7ecc9.i_partkey       AS i_partkey, 
                               _tmp_67d7ecc9.avg_qty         AS avg_qty 
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
                                FROM   (SELECT lineitem.l_orderkey         AS 
                                               l_orderkey, 
                                               lineitem.l_partkey          AS 
                                               l_partkey 
                                               , 
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
                               _tmp_ca004b4a.p_partkey     AS p_partkey, 
                               _tmp_ca004b4a.p_name        AS p_name, 
                               _tmp_ca004b4a.p_mfgr        AS p_mfgr, 
                               _tmp_ca004b4a.p_brand       AS p_brand, 
                               _tmp_ca004b4a.p_type        AS p_type, 
                               _tmp_ca004b4a.p_size        AS p_size, 
                               _tmp_ca004b4a.p_container   AS p_container, 
                               _tmp_ca004b4a.p_retailprice AS p_retailprice, 
                               _tmp_ca004b4a.p_comment     AS p_comment 
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
                                               INNER JOIN (SELECT 
                                               part.p_partkey, 
part.p_name, 
part.p_mfgr, 
part.p_brand, 
part.p_type, 
part.p_size, 
part.p_container, 
part.p_retailprice, 
part.p_comment 
            FROM   part) _tmp_ca004b4a 
        ON lineitem.l_partkey = 
           _tmp_ca004b4a.p_partkey) 
_tmp_d6e4db70 
WHERE  ( p_brand = 'Brand#23' ) 
AND ( p_container = 'MED BOX' )) _tmp_ef0e6f52 
INNER JOIN (SELECT i_partkey                      AS 
   i_partkey, 
   ( Cast(0.2 AS FLOAT) * avg_q ) AS 
   avg_qty 
FROM   (SELECT p_partkey       AS i_partkey, 
           Avg(l_quantity) AS avg_q 
    FROM   (SELECT lineitem.l_orderkey 
                   AS 
                   l_orderkey, 
                   lineitem.l_partkey 
                   AS 
                   l_partkey, 
                   lineitem.l_suppkey 
                   AS 
                   l_suppkey, 
                   lineitem.l_linenumber 
                   AS 
                   l_linenumber, 
                   lineitem.l_quantity 
                   AS 
                   l_quantity, 
lineitem.l_extendedprice 
AS 
l_extendedprice, 
lineitem.l_discount 
AS 
l_discount, 
lineitem.l_tax 
AS l_tax, 
lineitem.l_returnflag 
AS 
l_returnflag, 
lineitem.l_linestatus 
AS 
l_linestatus, 
lineitem.l_shipdate 
AS 
l_shipdate, 
lineitem.l_commitdate 
AS 
l_commitdate, 
lineitem.l_receiptdate 
AS 
l_receiptdate, 
lineitem.l_shipinstruct 
AS 
l_shipinstruct 
, 
lineitem.l_shipmode         AS l_shipmode, 
lineitem.l_comment          AS l_comment, 
_tmp_ca004b4a.p_partkey     AS p_partkey, 
_tmp_ca004b4a.p_name        AS p_name, 
_tmp_ca004b4a.p_mfgr        AS p_mfgr, 
_tmp_ca004b4a.p_brand       AS p_brand, 
_tmp_ca004b4a.p_type        AS p_type, 
_tmp_ca004b4a.p_size        AS p_size, 
_tmp_ca004b4a.p_container   AS p_container, 
_tmp_ca004b4a.p_retailprice AS p_retailprice, 
_tmp_ca004b4a.p_comment     AS p_comment 
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
INNER JOIN (SELECT part.p_partkey, 
part.p_name, 
part.p_mfgr, 
part.p_brand, 
part.p_type, 
part.p_size, 
part.p_container, 
part.p_retailprice, 
part.p_comment 
FROM   part) _tmp_ca004b4a 
ON lineitem.l_partkey = _tmp_ca004b4a.p_partkey) 
_tmp_d6e4db70 
GROUP  BY p_partkey) _tmp_d19836b0) _tmp_67d7ecc9 
ON _tmp_ef0e6f52.p_partkey = _tmp_67d7ecc9.i_partkey) 
_tmp_ce6cf641 
WHERE  ( l_quantity < avg_qty )) _tmp_1f4fe819) _tmp_5c339c79 
