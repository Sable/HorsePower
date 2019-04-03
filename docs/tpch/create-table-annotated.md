# TPC-H table information

## List of tables

- [nation](#nation)
- [region](#region)
- [part](#part)
- [supplier](#supplier)
- [partsupp](#partsupp)
- [customer](#customer)
- [orders](#orders)
- [lineitem](#lineitem)

## Details of tables

### nation

```sql
  CREATE TABLE NATION  ( N_NATIONKEY  INTEGER NOT NULL,   -- i32
                         N_NAME       CHAR(25) NOT NULL,  -- sym
                         N_REGIONKEY  INTEGER NOT NULL,   -- i32
                         N_COMMENT    VARCHAR(152));      -- str
``` 

- N_NATIONKEY

### region

```sql
  CREATE TABLE REGION  ( R_REGIONKEY  INTEGER NOT NULL,  -- i32
                         R_NAME       CHAR(25) NOT NULL, -- sym
                         R_COMMENT    VARCHAR(152));     -- str
```

- R_REGIONKEY

### part

```sql
  CREATE TABLE PART  ( P_PARTKEY     INTEGER NOT NULL,       -- i32
                       P_NAME        VARCHAR(55) NOT NULL,   -- sym
                       P_MFGR        CHAR(25) NOT NULL,      -- sym
                       P_BRAND       CHAR(10) NOT NULL,      -- sym
                       P_TYPE        VARCHAR(25) NOT NULL,   -- sym
                       P_SIZE        INTEGER NOT NULL,       -- i32
                       P_CONTAINER   CHAR(10) NOT NULL,      -- sym
                       P_RETAILPRICE DECIMAL(15,2) NOT NULL, -- f64
                       P_COMMENT     VARCHAR(23) NOT NULL ); -- str
```

- P_PARTKEY

### supplier

```sql
  CREATE TABLE SUPPLIER ( S_SUPPKEY     INTEGER NOT NULL,        -- i32
                          S_NAME        CHAR(25) NOT NULL,       -- sym
                          S_ADDRESS     VARCHAR(40) NOT NULL,    -- str
                          S_NATIONKEY   INTEGER NOT NULL,        -- i32
                          S_PHONE       CHAR(15) NOT NULL,       -- sym
                          S_ACCTBAL     DECIMAL(15,2) NOT NULL,  -- f64
                          S_COMMENT     VARCHAR(101) NOT NULL);  -- str
```

- S_SUPPKEY

### partsupp

```sql
  CREATE TABLE PARTSUPP ( PS_PARTKEY     INTEGER NOT NULL,        -- i32
                          PS_SUPPKEY     INTEGER NOT NULL,        -- i32
                          PS_AVAILQTY    INTEGER NOT NULL,        -- i32
                          PS_SUPPLYCOST  DECIMAL(15,2)  NOT NULL, -- f64
                          PS_COMMENT     VARCHAR(199) NOT NULL ); -- str
```

- PS_PARTKEY
- PS_SUPPKEY

### customer

```sql 
  CREATE TABLE CUSTOMER ( C_CUSTKEY     INTEGER NOT NULL,          -- i32
                          C_NAME        VARCHAR(25) NOT NULL,      -- sym
                          C_ADDRESS     VARCHAR(40) NOT NULL,      -- sym
                          C_NATIONKEY   INTEGER NOT NULL,          -- i32
                          C_PHONE       CHAR(15) NOT NULL,         -- sym
                          C_ACCTBAL     DECIMAL(15,2)   NOT NULL,  -- f64
                          C_MKTSEGMENT  CHAR(10) NOT NULL,         -- sym
                          C_COMMENT     VARCHAR(117) NOT NULL);    -- str
```

- C_CUSTKEY

### orders

```sql
  CREATE TABLE ORDERS  ( O_ORDERKEY       INTEGER NOT NULL,       -- i32
                         O_CUSTKEY        INTEGER NOT NULL,       -- i32
                         O_ORDERSTATUS    CHAR(1) NOT NULL,       -- small
                         O_TOTALPRICE     DECIMAL(15,2) NOT NULL, -- f64
                         O_ORDERDATE      DATE NOT NULL,          -- i32
                         O_ORDERPRIORITY  CHAR(15) NOT NULL,      -- sym
                         O_CLERK          CHAR(15) NOT NULL,      -- sym
                         O_SHIPPRIORITY   INTEGER NOT NULL,       -- i32
                         O_COMMENT        VARCHAR(79) NOT NULL);  -- str
```

- O_ORDERKEY

### lineitem

```sql
  CREATE TABLE LINEITEM ( L_ORDERKEY        INTEGER NOT NULL,        -- i32
                          L_PARTKEY         INTEGER NOT NULL,        -- i32
                          L_SUPPKEY         INTEGER NOT NULL,        -- i32
                          L_LINENUMBER      INTEGER NOT NULL,        -- i32
                          L_QUANTITY        DECIMAL(15,2) NOT NULL,  -- f64
                          L_EXTENDEDPRICE   DECIMAL(15,2) NOT NULL,  -- f64
                          L_DISCOUNT        DECIMAL(15,2) NOT NULL,  -- f64
                          L_TAX             DECIMAL(15,2) NOT NULL,  -- f64
                          L_RETURNFLAG      CHAR(1) NOT NULL,        -- small
                          L_LINESTATUS      CHAR(1) NOT NULL,        -- small
                          L_SHIPDATE        DATE NOT NULL,           -- i32
                          L_COMMITDATE      DATE NOT NULL,           -- i32
                          L_RECEIPTDATE     DATE NOT NULL,           -- i32
                          L_SHIPINSTRUCT    CHAR(25) NOT NULL,       -- sym
                          L_SHIPMODE        CHAR(10) NOT NULL,       -- sym
                          L_COMMENT         VARCHAR(44) NOT NULL);   -- str
```

- L_ORDERKEY
- L_LINENUMBER

## Keys

Primary keys

```sql
-- Add primary keys
ALTER TABLE REGION   ADD PRIMARY KEY (R_REGIONKEY);
ALTER TABLE NATION   ADD PRIMARY KEY (N_NATIONKEY);
ALTER TABLE PART     ADD PRIMARY KEY (P_PARTKEY);
ALTER TABLE SUPPLIER ADD PRIMARY KEY (S_SUPPKEY);
ALTER TABLE PARTSUPP ADD PRIMARY KEY (PS_PARTKEY,PS_SUPPKEY);
ALTER TABLE CUSTOMER ADD PRIMARY KEY (C_CUSTKEY);
ALTER TABLE LINEITEM ADD PRIMARY KEY (L_ORDERKEY,L_LINENUMBER);
ALTER TABLE ORDERS   ADD PRIMARY KEY (O_ORDERKEY);
```

Foreign keys

```sql
-- Add foreign keys
ALTER TABLE NATION   ADD FOREIGN KEY (N_REGIONKEY) references REGION(R_REGIONKEY);
ALTER TABLE SUPPLIER ADD FOREIGN KEY (S_NATIONKEY) references NATION(N_NATIONKEY);
ALTER TABLE CUSTOMER ADD FOREIGN KEY (C_NATIONKEY) references NATION(N_NATIONKEY);
ALTER TABLE PARTSUPP ADD FOREIGN KEY (PS_SUPPKEY)  references SUPPLIER(S_SUPPKEY);
ALTER TABLE PARTSUPP ADD FOREIGN KEY (PS_PARTKEY)  references PART(P_PARTKEY);
ALTER TABLE ORDERS   ADD FOREIGN KEY (O_CUSTKEY)   references CUSTOMER(C_CUSTKEY);
ALTER TABLE LINEITEM ADD FOREIGN KEY (L_ORDERKEY)  references ORDERS(O_ORDERKEY);
ALTER TABLE LINEITEM ADD FOREIGN KEY (L_PARTKEY,L_SUPPKEY) references PARTSUPP(PS_PARTKEY, PS_SUPPKEY);
```



