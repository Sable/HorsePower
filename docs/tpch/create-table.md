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

Note DECIMAL as shown below one of three different ways.

- DECIMAL - Precision defaults to 38, Scale defaults to 0
- DECIMAL(p) - Scale defaults to 0
- DECIMAL(p, s) - Precision and Scale are defined by the user

See more [database types](http://www.cs.toronto.edu/~nn/csc309-20085/guide/pointbase/docs/html/htmlfiles/dev_datatypesandconversionsFIN.html).

### nation

```sql
  CREATE TABLE NATION  ( N_NATIONKEY  INTEGER NOT NULL,
                         N_NAME       CHAR(25) NOT NULL,
                         N_REGIONKEY  INTEGER NOT NULL,
                         N_COMMENT    VARCHAR(152));
``` 

- N_NATIONKEY

### region

```sql
  CREATE TABLE REGION  ( R_REGIONKEY  INTEGER NOT NULL,
                         R_NAME       CHAR(25) NOT NULL,
                         R_COMMENT    VARCHAR(152));
```

- R_REGIONKEY

### part

```sql
  CREATE TABLE PART  ( P_PARTKEY     INTEGER NOT NULL,
                       P_NAME        VARCHAR(55) NOT NULL,
                       P_MFGR        CHAR(25) NOT NULL,
                       P_BRAND       CHAR(10) NOT NULL,
                       P_TYPE        VARCHAR(25) NOT NULL,
                       P_SIZE        INTEGER NOT NULL,
                       P_CONTAINER   CHAR(10) NOT NULL,
                       P_RETAILPRICE DECIMAL(15,2) NOT NULL,
                       P_COMMENT     VARCHAR(23) NOT NULL );
```

- P_PARTKEY

### supplier

```sql
  CREATE TABLE SUPPLIER ( S_SUPPKEY     INTEGER NOT NULL,
                          S_NAME        CHAR(25) NOT NULL,
                          S_ADDRESS     VARCHAR(40) NOT NULL,
                          S_NATIONKEY   INTEGER NOT NULL,
                          S_PHONE       CHAR(15) NOT NULL,
                          S_ACCTBAL     DECIMAL(15,2) NOT NULL,
                          S_COMMENT     VARCHAR(101) NOT NULL);
```

- S_SUPPKEY

### partsupp

```sql
  CREATE TABLE PARTSUPP ( PS_PARTKEY     INTEGER NOT NULL,
                          PS_SUPPKEY     INTEGER NOT NULL,
                          PS_AVAILQTY    INTEGER NOT NULL,
                          PS_SUPPLYCOST  DECIMAL(15,2)  NOT NULL,
                          PS_COMMENT     VARCHAR(199) NOT NULL );
```

- PS_PARTKEY
- PS_SUPPKEY

### customer

```sql 
  CREATE TABLE CUSTOMER ( C_CUSTKEY     INTEGER NOT NULL,
                          C_NAME        VARCHAR(25) NOT NULL,
                          C_ADDRESS     VARCHAR(40) NOT NULL,
                          C_NATIONKEY   INTEGER NOT NULL,
                          C_PHONE       CHAR(15) NOT NULL,
                          C_ACCTBAL     DECIMAL(15,2)   NOT NULL,
                          C_MKTSEGMENT  CHAR(10) NOT NULL,
                          C_COMMENT     VARCHAR(117) NOT NULL);
```

- C_CUSTKEY

### orders

```sql
  CREATE TABLE ORDERS  ( O_ORDERKEY       INTEGER NOT NULL,
                         O_CUSTKEY        INTEGER NOT NULL,
                         O_ORDERSTATUS    CHAR(1) NOT NULL,
                         O_TOTALPRICE     DECIMAL(15,2) NOT NULL,
                         O_ORDERDATE      DATE NOT NULL,
                         O_ORDERPRIORITY  CHAR(15) NOT NULL,
                         O_CLERK          CHAR(15) NOT NULL,
                         O_SHIPPRIORITY   INTEGER NOT NULL,
                         O_COMMENT        VARCHAR(79) NOT NULL);
```

- O_ORDERKEY

### lineitem

```sql
  CREATE TABLE LINEITEM ( L_ORDERKEY        INTEGER NOT NULL,
                          L_PARTKEY         INTEGER NOT NULL,
                          L_SUPPKEY         INTEGER NOT NULL,
                          L_LINENUMBER      INTEGER NOT NULL,
                          L_QUANTITY        DECIMAL(15,2) NOT NULL,
                          L_EXTENDEDPRICE   DECIMAL(15,2) NOT NULL,
                          L_DISCOUNT        DECIMAL(15,2) NOT NULL,
                          L_TAX             DECIMAL(15,2) NOT NULL,
                          L_RETURNFLAG      CHAR(1) NOT NULL,
                          L_LINESTATUS      CHAR(1) NOT NULL,
                          L_SHIPDATE        DATE NOT NULL,
                          L_COMMITDATE      DATE NOT NULL,
                          L_RECEIPTDATE     DATE NOT NULL,
                          L_SHIPINSTRUCT    CHAR(25) NOT NULL,
                          L_SHIPMODE        CHAR(10) NOT NULL,
                          L_COMMENT         VARCHAR(44) NOT NULL);
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



