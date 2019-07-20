SELECT supp_nation AS supp_nation, 
       cust_nation AS cust_nation, 
       l_year      AS l_year, 
       Sum(volume) AS revenue 
FROM   (SELECT s_suppkey, 
               s_name, 
               s_address, 
               s_nationkey, 
               s_phone, 
               s_acctbal, 
               s_comment, 
               l_suppkey, 
               l_orderkey, 
               l_year, 
               volume, 
               o_orderkey, 
               o_custkey, 
               o_orderstatus, 
               o_totalprice, 
               o_orderdate, 
               o_orderpriority, 
               o_clerk, 
               o_shippriority, 
               o_comment, 
               c_custkey, 
               c_name, 
               c_address, 
               c_nationkey, 
               c_phone, 
               c_acctbal, 
               c_mktsegment, 
               c_comment, 
               supp_nation, 
               ns_nationkey, 
               cust_nation, 
               nc_nationkey 
        FROM   (SELECT _tmp_2c1c2289.s_suppkey       AS s_suppkey, 
                       _tmp_2c1c2289.s_name          AS s_name, 
                       _tmp_2c1c2289.s_address       AS s_address, 
                       _tmp_2c1c2289.s_nationkey     AS s_nationkey, 
                       _tmp_2c1c2289.s_phone         AS s_phone, 
                       _tmp_2c1c2289.s_acctbal       AS s_acctbal, 
                       _tmp_2c1c2289.s_comment       AS s_comment, 
                       _tmp_2c1c2289.l_suppkey       AS l_suppkey, 
                       _tmp_2c1c2289.l_orderkey      AS l_orderkey, 
                       _tmp_2c1c2289.l_year          AS l_year, 
                       _tmp_2c1c2289.volume          AS volume, 
                       _tmp_2c1c2289.o_orderkey      AS o_orderkey, 
                       _tmp_2c1c2289.o_custkey       AS o_custkey, 
                       _tmp_2c1c2289.o_orderstatus   AS o_orderstatus, 
                       _tmp_2c1c2289.o_totalprice    AS o_totalprice, 
                       _tmp_2c1c2289.o_orderdate     AS o_orderdate, 
                       _tmp_2c1c2289.o_orderpriority AS o_orderpriority, 
                       _tmp_2c1c2289.o_clerk         AS o_clerk, 
                       _tmp_2c1c2289.o_shippriority  AS o_shippriority, 
                       _tmp_2c1c2289.o_comment       AS o_comment, 
                       _tmp_2c1c2289.c_custkey       AS c_custkey, 
                       _tmp_2c1c2289.c_name          AS c_name, 
                       _tmp_2c1c2289.c_address       AS c_address, 
                       _tmp_2c1c2289.c_nationkey     AS c_nationkey, 
                       _tmp_2c1c2289.c_phone         AS c_phone, 
                       _tmp_2c1c2289.c_acctbal       AS c_acctbal, 
                       _tmp_2c1c2289.c_mktsegment    AS c_mktsegment, 
                       _tmp_2c1c2289.c_comment       AS c_comment, 
                       _tmp_2c1c2289.supp_nation     AS supp_nation, 
                       _tmp_2c1c2289.ns_nationkey    AS ns_nationkey, 
                       _tmp_1bb8369f.cust_nation     AS cust_nation, 
                       _tmp_1bb8369f.nc_nationkey    AS nc_nationkey 
                FROM   (SELECT _tmp_c7581b3c.s_suppkey       AS s_suppkey, 
                               _tmp_c7581b3c.s_name          AS s_name, 
                               _tmp_c7581b3c.s_address       AS s_address, 
                               _tmp_c7581b3c.s_nationkey     AS s_nationkey, 
                               _tmp_c7581b3c.s_phone         AS s_phone, 
                               _tmp_c7581b3c.s_acctbal       AS s_acctbal, 
                               _tmp_c7581b3c.s_comment       AS s_comment, 
                               _tmp_c7581b3c.l_suppkey       AS l_suppkey, 
                               _tmp_c7581b3c.l_orderkey      AS l_orderkey, 
                               _tmp_c7581b3c.l_year          AS l_year, 
                               _tmp_c7581b3c.volume          AS volume, 
                               _tmp_c7581b3c.o_orderkey      AS o_orderkey, 
                               _tmp_c7581b3c.o_custkey       AS o_custkey, 
                               _tmp_c7581b3c.o_orderstatus   AS o_orderstatus, 
                               _tmp_c7581b3c.o_totalprice    AS o_totalprice, 
                               _tmp_c7581b3c.o_orderdate     AS o_orderdate, 
                               _tmp_c7581b3c.o_orderpriority AS o_orderpriority, 
                               _tmp_c7581b3c.o_clerk         AS o_clerk, 
                               _tmp_c7581b3c.o_shippriority  AS o_shippriority, 
                               _tmp_c7581b3c.o_comment       AS o_comment, 
                               _tmp_c7581b3c.c_custkey       AS c_custkey, 
                               _tmp_c7581b3c.c_name          AS c_name, 
                               _tmp_c7581b3c.c_address       AS c_address, 
                               _tmp_c7581b3c.c_nationkey     AS c_nationkey, 
                               _tmp_c7581b3c.c_phone         AS c_phone, 
                               _tmp_c7581b3c.c_acctbal       AS c_acctbal, 
                               _tmp_c7581b3c.c_mktsegment    AS c_mktsegment, 
                               _tmp_c7581b3c.c_comment       AS c_comment, 
                               _tmp_b7d73d53.supp_nation     AS supp_nation, 
                               _tmp_b7d73d53.ns_nationkey    AS ns_nationkey 
                        FROM   (SELECT _tmp_446b7fa9.s_suppkey       AS 
                                       s_suppkey, 
                                       _tmp_446b7fa9.s_name          AS s_name, 
                                       _tmp_446b7fa9.s_address       AS 
                                       s_address, 
                                       _tmp_446b7fa9.s_nationkey     AS 
                                       s_nationkey, 
                                       _tmp_446b7fa9.s_phone         AS s_phone, 
                                       _tmp_446b7fa9.s_acctbal       AS 
                                       s_acctbal, 
                                       _tmp_446b7fa9.s_comment       AS 
                                       s_comment, 
                                       _tmp_446b7fa9.l_suppkey       AS 
                                       l_suppkey, 
                                       _tmp_446b7fa9.l_orderkey      AS 
                                       l_orderkey, 
                                       _tmp_446b7fa9.l_year          AS l_year, 
                                       _tmp_446b7fa9.volume          AS volume, 
                                       _tmp_446b7fa9.o_orderkey      AS 
                                       o_orderkey, 
                                       _tmp_446b7fa9.o_custkey       AS 
                                       o_custkey, 
                                       _tmp_446b7fa9.o_orderstatus   AS 
                                       o_orderstatus, 
                                       _tmp_446b7fa9.o_totalprice    AS 
                                       o_totalprice, 
                                       _tmp_446b7fa9.o_orderdate     AS 
                                       o_orderdate, 
                                       _tmp_446b7fa9.o_orderpriority AS 
                                       o_orderpriority 
                                       , 
       _tmp_446b7fa9.o_clerk         AS o_clerk, 
       _tmp_446b7fa9.o_shippriority  AS o_shippriority, 
       _tmp_446b7fa9.o_comment       AS o_comment, 
       customer.c_custkey            AS c_custkey, 
       customer.c_name               AS c_name, 
       customer.c_address            AS c_address, 
       customer.c_nationkey          AS c_nationkey, 
       customer.c_phone              AS c_phone, 
       customer.c_acctbal            AS c_acctbal, 
       customer.c_mktsegment         AS c_mktsegment, 
       customer.c_comment            AS c_comment 
       FROM   (SELECT _tmp_077b4dfc.s_suppkey   AS s_suppkey, 
               _tmp_077b4dfc.s_name      AS s_name, 
               _tmp_077b4dfc.s_address   AS s_address, 
               _tmp_077b4dfc.s_nationkey AS s_nationkey, 
               _tmp_077b4dfc.s_phone     AS s_phone, 
               _tmp_077b4dfc.s_acctbal   AS s_acctbal, 
               _tmp_077b4dfc.s_comment   AS s_comment, 
               _tmp_077b4dfc.l_suppkey   AS l_suppkey, 
               _tmp_077b4dfc.l_orderkey  AS l_orderkey, 
               _tmp_077b4dfc.l_year      AS l_year, 
               _tmp_077b4dfc.volume      AS volume, 
               orders.o_orderkey         AS o_orderkey, 
               orders.o_custkey          AS o_custkey, 
               orders.o_orderstatus      AS o_orderstatus, 
               orders.o_totalprice       AS o_totalprice, 
               orders.o_orderdate        AS o_orderdate, 
               orders.o_orderpriority    AS o_orderpriority, 
               orders.o_clerk            AS o_clerk, 
               orders.o_shippriority     AS o_shippriority, 
               orders.o_comment          AS o_comment 
        FROM   (SELECT supplier.s_suppkey       AS s_suppkey, 
                       supplier.s_name          AS s_name, 
                       supplier.s_address       AS s_address, 
                       supplier.s_nationkey     AS s_nationkey, 
                       supplier.s_phone         AS s_phone, 
                       supplier.s_acctbal       AS s_acctbal, 
                       supplier.s_comment       AS s_comment, 
                       _tmp_57302e8f.l_suppkey  AS l_suppkey, 
                       _tmp_57302e8f.l_orderkey AS l_orderkey, 
                       _tmp_57302e8f.l_year     AS l_year, 
                       _tmp_57302e8f.volume     AS volume 
                FROM   (SELECT supplier.s_suppkey, 
                               supplier.s_name, 
                               supplier.s_address, 
                               supplier.s_nationkey, 
                               supplier.s_phone, 
                               supplier.s_acctbal, 
                               supplier.s_comment 
                        FROM   supplier) supplier 
                       INNER JOIN (SELECT l_suppkey 
                                          AS 
                                          l_suppkey 
                                          , 
                                          l_orderkey 
                                          AS l_orderkey, 
                                          Extract( 
                                  year FROM l_shipdate) 
                                          AS 
                                          l_year, 
                                          ( l_extendedprice * 
                                            ( 1 - l_discount ) 
                                          ) AS 
                                          volume 
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
       WHERE  ( l_shipdate >= '1995-01-01' ) 
       AND ( l_shipdate <= '1996-12-31' )) 
       _tmp_df4a4a86) 
       _tmp_57302e8f 
       ON supplier.s_suppkey = _tmp_57302e8f.l_suppkey) 
       _tmp_077b4dfc 
       INNER JOIN (SELECT orders.o_orderkey, 
       orders.o_custkey, 
       orders.o_orderstatus, 
       orders.o_totalprice, 
       orders.o_orderdate, 
       orders.o_orderpriority, 
       orders.o_clerk, 
       orders.o_shippriority, 
       orders.o_comment 
       FROM   orders) orders 
       ON _tmp_077b4dfc.l_orderkey = orders.o_orderkey) _tmp_446b7fa9 
       INNER JOIN (SELECT customer.c_custkey, 
       customer.c_name, 
       customer.c_address, 
       customer.c_nationkey, 
       customer.c_phone, 
       customer.c_acctbal, 
       customer.c_mktsegment, 
       customer.c_comment 
       FROM   customer) customer 
       ON _tmp_446b7fa9.o_custkey = customer.c_custkey) _tmp_c7581b3c 
       INNER JOIN (SELECT n_name      AS supp_nation, 
                  n_nationkey AS ns_nationkey 
           FROM   (SELECT nation.n_nationkey, 
                          nation.n_name, 
                          nation.n_regionkey, 
                          nation.n_comment 
                   FROM   nation) nation) _tmp_b7d73d53 
       ON _tmp_c7581b3c.s_nationkey = 
          _tmp_b7d73d53.ns_nationkey) 
       _tmp_2c1c2289 
       INNER JOIN (SELECT n_name      AS cust_nation, 
          n_nationkey AS nc_nationkey 
       FROM   (SELECT nation.n_nationkey, 
                  nation.n_name, 
                  nation.n_regionkey, 
                  nation.n_comment 
           FROM   nation) nation) _tmp_1bb8369f 
       ON _tmp_2c1c2289.c_nationkey = _tmp_1bb8369f.nc_nationkey) 
       _tmp_1e2e1618 
        WHERE  ( ( ( supp_nation = 'FRANCE' ) 
                   AND ( cust_nation = 'GERMANY' ) ) 
                  OR ( ( supp_nation = 'GERMANY' ) 
                       AND ( cust_nation = 'FRANCE' ) ) )) _tmp_86c4b23f 
GROUP  BY supp_nation, 
          cust_nation, 
          l_year 
ORDER  BY supp_nation, 
          cust_nation, 
          l_year 
