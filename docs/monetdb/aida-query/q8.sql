SELECT o_year             AS o_year, 
       ( sum_v1 / sum_v ) AS mkt_share 
FROM   (SELECT o_year       AS o_year, 
               Sum(volume1) AS sum_v1, 
               Sum(volume)  AS sum_v 
        FROM   (SELECT Extract(year FROM o_orderdate)           AS o_year, 
                       CASE 
                         WHEN ( n2_name = 'BRAZIL' ) THEN ( l_extendedprice * 
                                                            ( 1 - l_discount ) ) 
                         ELSE 0 
                       end                                      AS volume1, 
                       ( l_extendedprice * ( 1 - l_discount ) ) AS volume, 
                       n2_name                                  AS nation 
                FROM   (SELECT _tmp_6dca1fa1.l_orderkey      AS l_orderkey, 
                               _tmp_6dca1fa1.l_partkey       AS l_partkey, 
                               _tmp_6dca1fa1.l_suppkey       AS l_suppkey, 
                               _tmp_6dca1fa1.l_linenumber    AS l_linenumber, 
                               _tmp_6dca1fa1.l_quantity      AS l_quantity, 
                               _tmp_6dca1fa1.l_extendedprice AS l_extendedprice, 
                               _tmp_6dca1fa1.l_discount      AS l_discount, 
                               _tmp_6dca1fa1.l_tax           AS l_tax, 
                               _tmp_6dca1fa1.l_returnflag    AS l_returnflag, 
                               _tmp_6dca1fa1.l_linestatus    AS l_linestatus, 
                               _tmp_6dca1fa1.l_shipdate      AS l_shipdate, 
                               _tmp_6dca1fa1.l_commitdate    AS l_commitdate, 
                               _tmp_6dca1fa1.l_receiptdate   AS l_receiptdate, 
                               _tmp_6dca1fa1.l_shipinstruct  AS l_shipinstruct, 
                               _tmp_6dca1fa1.l_shipmode      AS l_shipmode, 
                               _tmp_6dca1fa1.l_comment       AS l_comment, 
                               _tmp_6dca1fa1.p_partkey       AS p_partkey, 
                               _tmp_6dca1fa1.p_name          AS p_name, 
                               _tmp_6dca1fa1.p_mfgr          AS p_mfgr, 
                               _tmp_6dca1fa1.p_brand         AS p_brand, 
                               _tmp_6dca1fa1.p_type          AS p_type, 
                               _tmp_6dca1fa1.p_size          AS p_size, 
                               _tmp_6dca1fa1.p_container     AS p_container, 
                               _tmp_6dca1fa1.p_retailprice   AS p_retailprice, 
                               _tmp_6dca1fa1.p_comment       AS p_comment, 
                               _tmp_6dca1fa1.s_suppkey       AS s_suppkey, 
                               _tmp_6dca1fa1.s_name          AS s_name, 
                               _tmp_6dca1fa1.s_address       AS s_address, 
                               _tmp_6dca1fa1.s_nationkey     AS s_nationkey, 
                               _tmp_6dca1fa1.s_phone         AS s_phone, 
                               _tmp_6dca1fa1.s_acctbal       AS s_acctbal, 
                               _tmp_6dca1fa1.s_comment       AS s_comment, 
                               _tmp_6dca1fa1.o_orderkey      AS o_orderkey, 
                               _tmp_6dca1fa1.o_custkey       AS o_custkey, 
                               _tmp_6dca1fa1.o_orderstatus   AS o_orderstatus, 
                               _tmp_6dca1fa1.o_totalprice    AS o_totalprice, 
                               _tmp_6dca1fa1.o_orderdate     AS o_orderdate, 
                               _tmp_6dca1fa1.o_orderpriority AS o_orderpriority, 
                               _tmp_6dca1fa1.o_clerk         AS o_clerk, 
                               _tmp_6dca1fa1.o_shippriority  AS o_shippriority, 
                               _tmp_6dca1fa1.o_comment       AS o_comment, 
                               _tmp_6dca1fa1.c_custkey       AS c_custkey, 
                               _tmp_6dca1fa1.c_name          AS c_name, 
                               _tmp_6dca1fa1.c_address       AS c_address, 
                               _tmp_6dca1fa1.c_nationkey     AS c_nationkey, 
                               _tmp_6dca1fa1.c_phone         AS c_phone, 
                               _tmp_6dca1fa1.c_acctbal       AS c_acctbal, 
                               _tmp_6dca1fa1.c_mktsegment    AS c_mktsegment, 
                               _tmp_6dca1fa1.c_comment       AS c_comment, 
                               _tmp_6dca1fa1.n1_name         AS n1_name, 
                               _tmp_6dca1fa1.n1_nationkey    AS n1_nationkey, 
                               _tmp_6dca1fa1.n1_regionkey    AS n1_regionkey, 
                               _tmp_6dca1fa1.r_regionkey     AS r_regionkey, 
                               _tmp_6dca1fa1.r_name          AS r_name, 
                               _tmp_6dca1fa1.r_comment       AS r_comment, 
                               _tmp_6c53f576.n2_name         AS n2_name, 
                               _tmp_6c53f576.n2_nationkey    AS n2_nationkey, 
                               _tmp_6c53f576.n2_regionkey    AS n2_regionkey 
                        FROM   (SELECT _tmp_1d6e69a0.l_orderkey      AS 
                                       l_orderkey, 
                                       _tmp_1d6e69a0.l_partkey       AS 
                                       l_partkey, 
                                       _tmp_1d6e69a0.l_suppkey       AS 
                                       l_suppkey, 
                                       _tmp_1d6e69a0.l_linenumber    AS 
                                       l_linenumber, 
                                       _tmp_1d6e69a0.l_quantity      AS 
                                       l_quantity, 
                                       _tmp_1d6e69a0.l_extendedprice AS 
                                       l_extendedprice 
                                       , 
       _tmp_1d6e69a0.l_discount      AS l_discount, 
       _tmp_1d6e69a0.l_tax           AS l_tax, 
       _tmp_1d6e69a0.l_returnflag    AS l_returnflag, 
       _tmp_1d6e69a0.l_linestatus    AS l_linestatus, 
       _tmp_1d6e69a0.l_shipdate      AS l_shipdate, 
       _tmp_1d6e69a0.l_commitdate    AS l_commitdate, 
       _tmp_1d6e69a0.l_receiptdate   AS l_receiptdate, 
       _tmp_1d6e69a0.l_shipinstruct  AS l_shipinstruct, 
       _tmp_1d6e69a0.l_shipmode      AS l_shipmode, 
       _tmp_1d6e69a0.l_comment       AS l_comment, 
       _tmp_1d6e69a0.p_partkey       AS p_partkey, 
       _tmp_1d6e69a0.p_name          AS p_name, 
       _tmp_1d6e69a0.p_mfgr          AS p_mfgr, 
       _tmp_1d6e69a0.p_brand         AS p_brand, 
       _tmp_1d6e69a0.p_type          AS p_type, 
       _tmp_1d6e69a0.p_size          AS p_size, 
       _tmp_1d6e69a0.p_container     AS p_container, 
       _tmp_1d6e69a0.p_retailprice   AS p_retailprice, 
       _tmp_1d6e69a0.p_comment       AS p_comment, 
       _tmp_1d6e69a0.s_suppkey       AS s_suppkey, 
       _tmp_1d6e69a0.s_name          AS s_name, 
       _tmp_1d6e69a0.s_address       AS s_address, 
       _tmp_1d6e69a0.s_nationkey     AS s_nationkey, 
       _tmp_1d6e69a0.s_phone         AS s_phone, 
       _tmp_1d6e69a0.s_acctbal       AS s_acctbal, 
       _tmp_1d6e69a0.s_comment       AS s_comment, 
       _tmp_1d6e69a0.o_orderkey      AS o_orderkey, 
       _tmp_1d6e69a0.o_custkey       AS o_custkey, 
       _tmp_1d6e69a0.o_orderstatus   AS o_orderstatus, 
       _tmp_1d6e69a0.o_totalprice    AS o_totalprice, 
       _tmp_1d6e69a0.o_orderdate     AS o_orderdate, 
       _tmp_1d6e69a0.o_orderpriority AS o_orderpriority, 
       _tmp_1d6e69a0.o_clerk         AS o_clerk, 
       _tmp_1d6e69a0.o_shippriority  AS o_shippriority, 
       _tmp_1d6e69a0.o_comment       AS o_comment, 
       _tmp_1d6e69a0.c_custkey       AS c_custkey, 
       _tmp_1d6e69a0.c_name          AS c_name, 
       _tmp_1d6e69a0.c_address       AS c_address, 
       _tmp_1d6e69a0.c_nationkey     AS c_nationkey, 
       _tmp_1d6e69a0.c_phone         AS c_phone, 
       _tmp_1d6e69a0.c_acctbal       AS c_acctbal, 
       _tmp_1d6e69a0.c_mktsegment    AS c_mktsegment, 
       _tmp_1d6e69a0.c_comment       AS c_comment, 
       _tmp_1d6e69a0.n1_name         AS n1_name, 
       _tmp_1d6e69a0.n1_nationkey    AS n1_nationkey, 
       _tmp_1d6e69a0.n1_regionkey    AS n1_regionkey, 
       _tmp_1a19079f.r_regionkey     AS r_regionkey, 
       _tmp_1a19079f.r_name          AS r_name, 
       _tmp_1a19079f.r_comment       AS r_comment 
       FROM   (SELECT _tmp_2a2c1739.l_orderkey      AS l_orderkey, 
               _tmp_2a2c1739.l_partkey       AS l_partkey, 
               _tmp_2a2c1739.l_suppkey       AS l_suppkey, 
               _tmp_2a2c1739.l_linenumber    AS l_linenumber, 
               _tmp_2a2c1739.l_quantity      AS l_quantity, 
               _tmp_2a2c1739.l_extendedprice AS l_extendedprice 
               , 
       _tmp_2a2c1739.l_discount      AS l_discount, 
       _tmp_2a2c1739.l_tax           AS l_tax, 
       _tmp_2a2c1739.l_returnflag    AS l_returnflag, 
       _tmp_2a2c1739.l_linestatus    AS l_linestatus, 
       _tmp_2a2c1739.l_shipdate      AS l_shipdate, 
       _tmp_2a2c1739.l_commitdate    AS l_commitdate, 
       _tmp_2a2c1739.l_receiptdate   AS l_receiptdate, 
       _tmp_2a2c1739.l_shipinstruct  AS l_shipinstruct, 
       _tmp_2a2c1739.l_shipmode      AS l_shipmode, 
       _tmp_2a2c1739.l_comment       AS l_comment, 
       _tmp_2a2c1739.p_partkey       AS p_partkey, 
       _tmp_2a2c1739.p_name          AS p_name, 
       _tmp_2a2c1739.p_mfgr          AS p_mfgr, 
       _tmp_2a2c1739.p_brand         AS p_brand, 
       _tmp_2a2c1739.p_type          AS p_type, 
       _tmp_2a2c1739.p_size          AS p_size, 
       _tmp_2a2c1739.p_container     AS p_container, 
       _tmp_2a2c1739.p_retailprice   AS p_retailprice, 
       _tmp_2a2c1739.p_comment       AS p_comment, 
       _tmp_2a2c1739.s_suppkey       AS s_suppkey, 
       _tmp_2a2c1739.s_name          AS s_name, 
       _tmp_2a2c1739.s_address       AS s_address, 
       _tmp_2a2c1739.s_nationkey     AS s_nationkey, 
       _tmp_2a2c1739.s_phone         AS s_phone, 
       _tmp_2a2c1739.s_acctbal       AS s_acctbal, 
       _tmp_2a2c1739.s_comment       AS s_comment, 
       _tmp_2a2c1739.o_orderkey      AS o_orderkey, 
       _tmp_2a2c1739.o_custkey       AS o_custkey, 
       _tmp_2a2c1739.o_orderstatus   AS o_orderstatus, 
       _tmp_2a2c1739.o_totalprice    AS o_totalprice, 
       _tmp_2a2c1739.o_orderdate     AS o_orderdate, 
       _tmp_2a2c1739.o_orderpriority AS o_orderpriority, 
       _tmp_2a2c1739.o_clerk         AS o_clerk, 
       _tmp_2a2c1739.o_shippriority  AS o_shippriority, 
       _tmp_2a2c1739.o_comment       AS o_comment, 
       _tmp_2a2c1739.c_custkey       AS c_custkey, 
       _tmp_2a2c1739.c_name          AS c_name, 
       _tmp_2a2c1739.c_address       AS c_address, 
       _tmp_2a2c1739.c_nationkey     AS c_nationkey, 
       _tmp_2a2c1739.c_phone         AS c_phone, 
       _tmp_2a2c1739.c_acctbal       AS c_acctbal, 
       _tmp_2a2c1739.c_mktsegment    AS c_mktsegment, 
       _tmp_2a2c1739.c_comment       AS c_comment, 
       _tmp_b5bbf8eb.n1_name         AS n1_name, 
       _tmp_b5bbf8eb.n1_nationkey    AS n1_nationkey, 
       _tmp_b5bbf8eb.n1_regionkey    AS n1_regionkey 
       FROM   (SELECT _tmp_12cfb003.l_orderkey      AS l_orderkey, 
       _tmp_12cfb003.l_partkey       AS l_partkey, 
       _tmp_12cfb003.l_suppkey       AS l_suppkey, 
       _tmp_12cfb003.l_linenumber    AS l_linenumber, 
       _tmp_12cfb003.l_quantity      AS l_quantity, 
       _tmp_12cfb003.l_extendedprice AS l_extendedprice 
       , 
       _tmp_12cfb003.l_discount      AS l_discount, 
       _tmp_12cfb003.l_tax           AS l_tax, 
       _tmp_12cfb003.l_returnflag    AS l_returnflag, 
       _tmp_12cfb003.l_linestatus    AS l_linestatus, 
       _tmp_12cfb003.l_shipdate      AS l_shipdate, 
       _tmp_12cfb003.l_commitdate    AS l_commitdate, 
       _tmp_12cfb003.l_receiptdate   AS l_receiptdate, 
       _tmp_12cfb003.l_shipinstruct  AS l_shipinstruct, 
       _tmp_12cfb003.l_shipmode      AS l_shipmode, 
       _tmp_12cfb003.l_comment       AS l_comment, 
       _tmp_12cfb003.p_partkey       AS p_partkey, 
       _tmp_12cfb003.p_name          AS p_name, 
       _tmp_12cfb003.p_mfgr          AS p_mfgr, 
       _tmp_12cfb003.p_brand         AS p_brand, 
       _tmp_12cfb003.p_type          AS p_type, 
       _tmp_12cfb003.p_size          AS p_size, 
       _tmp_12cfb003.p_container     AS p_container, 
       _tmp_12cfb003.p_retailprice   AS p_retailprice, 
       _tmp_12cfb003.p_comment       AS p_comment, 
       _tmp_12cfb003.s_suppkey       AS s_suppkey, 
       _tmp_12cfb003.s_name          AS s_name, 
       _tmp_12cfb003.s_address       AS s_address, 
       _tmp_12cfb003.s_nationkey     AS s_nationkey, 
       _tmp_12cfb003.s_phone         AS s_phone, 
       _tmp_12cfb003.s_acctbal       AS s_acctbal, 
       _tmp_12cfb003.s_comment       AS s_comment, 
       _tmp_12cfb003.o_orderkey      AS o_orderkey, 
       _tmp_12cfb003.o_custkey       AS o_custkey, 
       _tmp_12cfb003.o_orderstatus   AS o_orderstatus, 
       _tmp_12cfb003.o_totalprice    AS o_totalprice, 
       _tmp_12cfb003.o_orderdate     AS o_orderdate, 
       _tmp_12cfb003.o_orderpriority AS o_orderpriority, 
       _tmp_12cfb003.o_clerk         AS o_clerk, 
       _tmp_12cfb003.o_shippriority  AS o_shippriority, 
       _tmp_12cfb003.o_comment       AS o_comment, 
       customer.c_custkey            AS c_custkey, 
       customer.c_name               AS c_name, 
       customer.c_address            AS c_address, 
       customer.c_nationkey          AS c_nationkey, 
       customer.c_phone              AS c_phone, 
       customer.c_acctbal            AS c_acctbal, 
       customer.c_mktsegment         AS c_mktsegment, 
       customer.c_comment            AS c_comment 
       FROM   (SELECT _tmp_3915678a.l_orderkey      AS l_orderkey, 
       _tmp_3915678a.l_partkey       AS l_partkey, 
       _tmp_3915678a.l_suppkey       AS l_suppkey, 
       _tmp_3915678a.l_linenumber    AS l_linenumber, 
       _tmp_3915678a.l_quantity      AS l_quantity, 
       _tmp_3915678a.l_extendedprice AS l_extendedprice 
       , 
       _tmp_3915678a.l_discount      AS l_discount, 
       _tmp_3915678a.l_tax           AS l_tax, 
       _tmp_3915678a.l_returnflag    AS l_returnflag, 
       _tmp_3915678a.l_linestatus    AS l_linestatus, 
       _tmp_3915678a.l_shipdate      AS l_shipdate, 
       _tmp_3915678a.l_commitdate    AS l_commitdate, 
       _tmp_3915678a.l_receiptdate   AS l_receiptdate, 
       _tmp_3915678a.l_shipinstruct  AS l_shipinstruct, 
       _tmp_3915678a.l_shipmode      AS l_shipmode, 
       _tmp_3915678a.l_comment       AS l_comment, 
       _tmp_3915678a.p_partkey       AS p_partkey, 
       _tmp_3915678a.p_name          AS p_name, 
       _tmp_3915678a.p_mfgr          AS p_mfgr, 
       _tmp_3915678a.p_brand         AS p_brand, 
       _tmp_3915678a.p_type          AS p_type, 
       _tmp_3915678a.p_size          AS p_size, 
       _tmp_3915678a.p_container     AS p_container, 
       _tmp_3915678a.p_retailprice   AS p_retailprice, 
       _tmp_3915678a.p_comment       AS p_comment, 
       _tmp_3915678a.s_suppkey       AS s_suppkey, 
       _tmp_3915678a.s_name          AS s_name, 
       _tmp_3915678a.s_address       AS s_address, 
       _tmp_3915678a.s_nationkey     AS s_nationkey, 
       _tmp_3915678a.s_phone         AS s_phone, 
       _tmp_3915678a.s_acctbal       AS s_acctbal, 
       _tmp_3915678a.s_comment       AS s_comment, 
       _tmp_0801bbf6.o_orderkey      AS o_orderkey, 
       _tmp_0801bbf6.o_custkey       AS o_custkey, 
       _tmp_0801bbf6.o_orderstatus   AS o_orderstatus, 
       _tmp_0801bbf6.o_totalprice    AS o_totalprice, 
       _tmp_0801bbf6.o_orderdate     AS o_orderdate, 
       _tmp_0801bbf6.o_orderpriority AS o_orderpriority, 
       _tmp_0801bbf6.o_clerk         AS o_clerk, 
       _tmp_0801bbf6.o_shippriority  AS o_shippriority, 
       _tmp_0801bbf6.o_comment       AS o_comment 
       FROM   (SELECT _tmp_0c729bca.l_orderkey      AS l_orderkey, 
       _tmp_0c729bca.l_partkey       AS l_partkey, 
       _tmp_0c729bca.l_suppkey       AS l_suppkey, 
       _tmp_0c729bca.l_linenumber    AS l_linenumber, 
       _tmp_0c729bca.l_quantity      AS l_quantity, 
       _tmp_0c729bca.l_extendedprice AS l_extendedprice 
       , 
       _tmp_0c729bca.l_discount      AS l_discount, 
       _tmp_0c729bca.l_tax           AS l_tax, 
       _tmp_0c729bca.l_returnflag    AS l_returnflag, 
       _tmp_0c729bca.l_linestatus    AS l_linestatus, 
       _tmp_0c729bca.l_shipdate      AS l_shipdate, 
       _tmp_0c729bca.l_commitdate    AS l_commitdate, 
       _tmp_0c729bca.l_receiptdate   AS l_receiptdate, 
       _tmp_0c729bca.l_shipinstruct  AS l_shipinstruct, 
       _tmp_0c729bca.l_shipmode      AS l_shipmode, 
       _tmp_0c729bca.l_comment       AS l_comment, 
       _tmp_0c729bca.p_partkey       AS p_partkey, 
       _tmp_0c729bca.p_name          AS p_name, 
       _tmp_0c729bca.p_mfgr          AS p_mfgr, 
       _tmp_0c729bca.p_brand         AS p_brand, 
       _tmp_0c729bca.p_type          AS p_type, 
       _tmp_0c729bca.p_size          AS p_size, 
       _tmp_0c729bca.p_container     AS p_container, 
       _tmp_0c729bca.p_retailprice   AS p_retailprice, 
       _tmp_0c729bca.p_comment       AS p_comment, 
       supplier.s_suppkey            AS s_suppkey, 
       supplier.s_name               AS s_name, 
       supplier.s_address            AS s_address, 
       supplier.s_nationkey          AS s_nationkey, 
       supplier.s_phone              AS s_phone, 
       supplier.s_acctbal            AS s_acctbal, 
       supplier.s_comment            AS s_comment 
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
       _tmp_52e9211a.p_partkey     AS p_partkey, 
       _tmp_52e9211a.p_name        AS p_name, 
       _tmp_52e9211a.p_mfgr        AS p_mfgr, 
       _tmp_52e9211a.p_brand       AS p_brand, 
       _tmp_52e9211a.p_type        AS p_type, 
       _tmp_52e9211a.p_size        AS p_size, 
       _tmp_52e9211a.p_container   AS p_container, 
       _tmp_52e9211a.p_retailprice AS p_retailprice, 
       _tmp_52e9211a.p_comment     AS p_comment 
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
       WHERE 
       ( p_type = 'ECONOMY ANODIZED STEEL' ) 
       ) 
       _tmp_52e9211a 
       ON lineitem.l_partkey = 
       _tmp_52e9211a.p_partkey) 
       _tmp_0c729bca 
       INNER JOIN (SELECT supplier.s_suppkey, 
       supplier.s_name, 
       supplier.s_address, 
       supplier.s_nationkey, 
       supplier.s_phone, 
       supplier.s_acctbal, 
       supplier.s_comment 
       FROM   supplier) supplier 
       ON _tmp_0c729bca.l_suppkey = supplier.s_suppkey) 
       _tmp_3915678a 
       INNER JOIN (SELECT o_orderkey, 
       o_custkey, 
       o_orderstatus, 
       o_totalprice, 
       o_orderdate, 
       o_orderpriority, 
       o_clerk, 
       o_shippriority, 
       o_comment 
       FROM   (SELECT orders.o_orderkey, 
       orders.o_custkey, 
       orders.o_orderstatus, 
       orders.o_totalprice, 
       orders.o_orderdate, 
       orders.o_orderpriority, 
       orders.o_clerk, 
       orders.o_shippriority, 
       orders.o_comment 
       FROM   orders) orders 
       WHERE  ( o_orderdate >= '1995-01-01' ) 
       AND ( o_orderdate <= '1996-12-31' )) 
       _tmp_0801bbf6 
       ON _tmp_3915678a.l_orderkey = _tmp_0801bbf6.o_orderkey) 
       _tmp_12cfb003 
       INNER JOIN (SELECT customer.c_custkey, 
       customer.c_name, 
       customer.c_address, 
       customer.c_nationkey, 
       customer.c_phone, 
       customer.c_acctbal, 
       customer.c_mktsegment, 
       customer.c_comment 
       FROM   customer) customer 
       ON _tmp_12cfb003.o_custkey = customer.c_custkey) _tmp_2a2c1739 
       INNER JOIN (SELECT n_name      AS n1_name, 
       n_nationkey AS n1_nationkey, 
       n_regionkey AS n1_regionkey 
       FROM   (SELECT nation.n_nationkey, 
       nation.n_name, 
       nation.n_regionkey, 
       nation.n_comment 
       FROM   nation) nation) _tmp_b5bbf8eb 
       ON _tmp_2a2c1739.c_nationkey = _tmp_b5bbf8eb.n1_nationkey) _tmp_1d6e69a0 
       INNER JOIN (SELECT r_regionkey, 
       r_name, 
       r_comment 
       FROM   (SELECT region.r_regionkey, 
       region.r_name, 
       region.r_comment 
       FROM   region) region 
       WHERE  ( r_name = 'AMERICA' )) _tmp_1a19079f 
       ON _tmp_1d6e69a0.n1_regionkey = _tmp_1a19079f.r_regionkey) _tmp_6dca1fa1 
       INNER JOIN (SELECT n_name      AS n2_name, 
       n_nationkey AS n2_nationkey, 
       n_regionkey AS n2_regionkey 
       FROM   (SELECT nation.n_nationkey, 
       nation.n_name, 
       nation.n_regionkey, 
       nation.n_comment 
       FROM   nation) nation) _tmp_6c53f576 
       ON _tmp_6dca1fa1.s_nationkey = _tmp_6c53f576.n2_nationkey) _tmp_85ed655a) 
       _tmp_eda06781 
        GROUP  BY o_year) _tmp_85fdfdb4 
ORDER  BY o_year 
