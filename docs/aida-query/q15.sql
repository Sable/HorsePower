SELECT s_suppkey     AS s_suppkey, 
       s_name        AS s_name, 
       s_address     AS s_address, 
       s_phone       AS s_phone, 
       total_revenue AS total_revenue 
FROM   (SELECT s_suppkey, 
               s_name, 
               s_address, 
               s_nationkey, 
               s_phone, 
               s_acctbal, 
               s_comment, 
               supplier_no, 
               total_revenue 
        FROM   (SELECT supplier.s_suppkey          AS s_suppkey, 
                       supplier.s_name             AS s_name, 
                       supplier.s_address          AS s_address, 
                       supplier.s_nationkey        AS s_nationkey, 
                       supplier.s_phone            AS s_phone, 
                       supplier.s_acctbal          AS s_acctbal, 
                       supplier.s_comment          AS s_comment, 
                       _tmp_a12d226a.supplier_no   AS supplier_no, 
                       _tmp_a12d226a.total_revenue AS total_revenue 
                FROM   (SELECT supplier.s_suppkey, 
                               supplier.s_name, 
                               supplier.s_address, 
                               supplier.s_nationkey, 
                               supplier.s_phone, 
                               supplier.s_acctbal, 
                               supplier.s_comment 
                        FROM   supplier) supplier 
                       INNER JOIN (SELECT supplier_no AS supplier_no, 
                                          Sum(rev)    AS total_revenue 
                                   FROM   (SELECT l_suppkey 
                                                  AS 
                                                  supplier_no, 
                                                  ( l_extendedprice * 
                                                    ( 1 - l_discount ) ) AS 
                                                  rev 
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
       WHERE  ( l_shipdate >= '1996-01-01' ) 
       AND ( l_shipdate < '1996-04-01' )) 
       _tmp_1061607a) 
       _tmp_212acc37 
       GROUP  BY supplier_no) _tmp_a12d226a 
       ON supplier.s_suppkey = _tmp_a12d226a.supplier_no) _tmp_1500fa5b 
        WHERE  ( total_revenue = (SELECT Max(total_revenue) AS max_total_revenue 
                                  FROM   (SELECT supplier_no AS supplier_no, 
                                                 Sum(rev)    AS total_revenue 
                                          FROM   (SELECT l_suppkey 
                                                         AS supplier_no, 
                                                         ( l_extendedprice * ( 1 
                                                           - l_discount ) 
                                                         ) 
                                                         AS rev 
                                                  FROM   (SELECT 
                                                 l_orderkey, 
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
        lineitem.l_linenumber, 
        lineitem.l_quantity 
, 
        lineitem.l_extendedprice 
        , 
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
WHERE  ( l_shipdate >= '1996-01-01' ) 
AND ( l_shipdate < '1996-04-01' )) _tmp_1061607a 
) 
_tmp_212acc37 
GROUP  BY supplier_no) _tmp_a12d226a) )) _tmp_ef1ae24f 
ORDER  BY s_suppkey 
