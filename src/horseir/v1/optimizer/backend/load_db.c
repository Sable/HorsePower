#include "../global.h"
C CSV_FILE_ROOT[] = "../data/tpch/db";
L CSV_FILE_SCALE = 1;
L TEST_RUNS = 1;
extern B isReadBin;

#define H_INT H_L
#define H_FLT H_E

static L initDBTable(L n, const C* PRE_DEFINED[], L* SYM_LIST_LINE){
    DOI(n, insertSym(createSymbol((S)PRE_DEFINED[i])));
    // printAllSymol();
    DOI(n, SYM_LIST_LINE[i]= getSymbol((S)PRE_DEFINED[i]));
    R 0;
}

static L readTableRegion(){
    // C CSV_LINE[] = "data/tpch/db1/region.tbl";
    C CSV_LINE[128]; SP(CSV_LINE, "%s%lld/region.tbl", CSV_FILE_ROOT, CSV_FILE_SCALE);
    L TYPE_LINE[]  = {H_INT, H_Q, H_S};
    const L NUM_COL_LINE = 3;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "r_regionkey", "r_name", "r_comment"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableRegion = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"region", tableRegion);
    R 0;
}

static L readTableNation(){
    // C CSV_LINE[] = "data/tpch/db1/nation.tbl";
    C CSV_LINE[128]; SP(CSV_LINE, "%s%lld/nation.tbl", CSV_FILE_ROOT, CSV_FILE_SCALE);
    L TYPE_LINE[]  = {H_INT, H_Q, H_INT, H_S};
    const L NUM_COL_LINE = 4;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "n_nationkey", "n_name", "n_regionkey", "n_comment"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableNation = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"nation", tableNation);
    R 0;
}

static L readTableCustomer(){
    // C CSV_LINE[] = "data/tpch/db1/customer.tbl";
    C CSV_LINE[128]; SP(CSV_LINE, "%s%lld/customer.tbl", CSV_FILE_ROOT, CSV_FILE_SCALE);
    L TYPE_LINE[]  = {H_INT, H_Q, H_S, H_INT,\
                      H_S, H_FLT, H_Q, H_S};
    const L NUM_COL_LINE = 8;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "c_custkey", "c_name",    "c_address",    "c_nationkey",
        "c_phone"  , "c_acctbal", "c_mktsegment", "c_comment"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableCustomer = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"customer", tableCustomer);
    R 0;
}

static L readTableOrders(){
    // C CSV_LINE[] = "data/tpch/db1/orders.tbl";
    // C CSV_LINE[] = "data/test-tables/orders-small.tbl";
    C CSV_LINE[128]; SP(CSV_LINE, "%s%lld/orders.tbl", CSV_FILE_ROOT, CSV_FILE_SCALE);
    L TYPE_LINE[]  = {H_INT, H_INT, H_C, H_FLT,\
                      H_D, H_Q, H_S, H_INT, H_S};
    const L NUM_COL_LINE = 9;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "o_orderkey", "o_custkey", "o_orderstatus", "o_totalprice",
        "o_orderdate", "o_orderpriority","o_clerk", "o_shippriority", "o_comment"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableOrders = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"orders", tableOrders);
    R 0;
}

static L readTableLineitem(){
    // C CSV_LINE[] = "data/tpch/db1/lineitem.tbl";
    // C CSV_LINE[] = "data/test-tables/lineitem-small.tbl";
    C CSV_LINE[128]; SP(CSV_LINE, "%s%lld/lineitem.tbl", CSV_FILE_ROOT, CSV_FILE_SCALE);
    L TYPE_LINE[]  = {H_INT, H_INT, H_INT, H_INT, \
                      H_FLT, H_FLT, H_FLT, H_FLT, \
                      H_C, H_C, H_D, H_D, \
                      H_D, H_Q, H_Q, H_S };
    const L NUM_COL_LINE = 16;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "l_orderkey",   "l_partkey",       "l_suppkey",  "l_linenumber",
        "l_quantity",   "l_extendedprice", "l_discount", "l_tax",
        "l_returnflag", "l_linestatus",    "l_shipdate", "l_commitdate",
        "l_receiptdate","l_shipinstruct",  "l_shipmode", "l_comment"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableLineitem = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"lineitem", tableLineitem);
    R 0;
}

static L readTablePart(){
    // C CSV_LINE[] = "data/tpch/db1/part.tbl";
    C CSV_LINE[128]; SP(CSV_LINE, "%s%lld/part.tbl", CSV_FILE_ROOT, CSV_FILE_SCALE);
    L TYPE_LINE[]  = {H_INT, H_S, H_S, H_Q, \
                      H_Q, H_INT, H_Q, H_FLT, H_S };
    const L NUM_COL_LINE = 9;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "p_partkey",  "p_name",  "p_mfgr",      "p_brand",
        "p_type",     "p_size",  "p_container", "p_retailprice", "p_comment"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tablePart = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"part", tablePart);
    R 0;
}

static L readTableSupplier(){
    // C CSV_LINE[] = "data/tpch/db1/supplier.tbl";
    C CSV_LINE[128]; SP(CSV_LINE, "%s%lld/supplier.tbl", CSV_FILE_ROOT, CSV_FILE_SCALE);
    L TYPE_LINE[]  = {H_INT, H_S, H_S, H_INT, \
                      H_S, H_FLT, H_S };
    const L NUM_COL_LINE = 7;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "s_suppkey", "s_name",    "s_address", "s_nationkey",
        "s_phone",   "s_acctbal", "s_comment"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableSupplier = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"supplier", tableSupplier);
    R 0;
}

static L readTablePartsupp(){
    // C CSV_LINE[] = "data/tpch/db1/partsupp.tbl";
    C CSV_LINE[128]; SP(CSV_LINE, "%s%lld/partsupp.tbl", CSV_FILE_ROOT, CSV_FILE_SCALE);
    L TYPE_LINE[]  = {H_INT, H_INT, H_INT, H_FLT, H_S};
    const L NUM_COL_LINE = 5;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "ps_partkey", "ps_suppkey", "ps_availqty", "ps_supplycost", "ps_comment"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tablePartsupp = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"partsupp", tablePartsupp);
    R 0;
}

const C* tpchName[] = { "Region",   "Nation",   "Customer", \
                        "Orders",   "Lineitem", "Part",     \
                        "Supplier", "Partsupp" };

L (*tpchDB[8])() = { readTableRegion,  readTableNation  , readTableCustomer, \
                     readTableOrders,  readTableLineitem, readTablePart, \
                     readTableSupplier,readTablePartsupp };

L readTpchTables(){
    // P("reading table region\n");
    // readTableRegion();
    // P("reading table nation\n");
    // readTableNation();
    // P("reading table customer\n");
    // readTableCustomer();
    // P("reading table orders\n");
    // readTableOrders();
    // P("reading table lineitem\n");
    // readTableLineitem();
    // P("reading table part\n");
    // readTablePart();
    // P("reading table supplier\n");   
    // readTableSupplier();
    // P("reading table partsupp\n");
    // readTablePartsupp();
    DOI(0, if(i==4){P("Loading table %s\n",tpchName[i]); (*tpchDB[i])();})
    // simulateQ6();
    R 0;
}

L initTableByName(S tableName){
    if(!findTableByName(getSymbol(tableName))){
        P("Loading table %s\n",tableName);
        if(isReadBin){
            P(">> Reading from bin\n");
            initTableFromBin(tableName);
        }
        else{
            if(!strcmp(tableName, "region"))
                readTableRegion();
            else if(!strcmp(tableName, "nation"))
                readTableNation();
            else if(!strcmp(tableName, "customer"))
                readTableCustomer();
            else if(!strcmp(tableName, "orders"))
                readTableOrders();
            else if(!strcmp(tableName, "lineitem"))
                readTableLineitem();
            else if(!strcmp(tableName, "part"))
                readTablePart();
            else if(!strcmp(tableName, "supplier"))
                readTableSupplier();
            else if(!strcmp(tableName, "partsupp"))
                readTablePartsupp();
            else EP("Table %s NOT FOUND\n",tableName);
        }
    }
    else {
        P("Table %s has been loaded\n",tableName);
    }
    R 0;
}

L metaTable(V x, S tableName){
    V keyV = getTableKeys(x);
    V valV = getTableVals(x);
    P("Table: %s\n", tableName);
    DOI(vn(keyV),{P("[%2lld] %-15s %s\n",i,getSymbolStr(vQ(keyV,i)),getTypeName(vp(vV(valV,i))));})
    R 0;
}

/* load from bin */
L initTableFromBin(S tableName){
    char temp[99];
    SP(temp, "../data/tpch-bin/db1/%s.bin",tableName);
    FILE *fp = fopen(temp, "rb");
    if(!fp) EP("File ../data/tpch-bin/db1/%s.bin open fails\n",tableName);
    V x = allocNode();
    readSerializeV(x, fp);
    fclose(fp);
    registerTable(tableName, x);
    metaTable(x, tableName);
    R 0;
}



