#include "../global.h"

#define H_INT H_I
#define H_FLT H_E
#define setTpchPath(path,table)    setDataPath(path,"tpch",table,"tbl")
#define setTpchBinPath(path,table) setDataPath(path,"tpch-bin",table,"bin")

L CSV_FILE_SCALE = 1;
L TEST_RUNS      = 1;
extern B isReadBin;

static O setDataPath(S path, const char *folder, S tableName, const char *format){
    CS base = getenv("HORSE_BASE");
    if(base){
        SP(path, "%s/data/%s/db%lld/%s.%s", base, folder, CSV_FILE_SCALE, tableName, format);
    }
    else{
        EP("Set project base directory: export HORSE_BASE=/path/to/HorsePower\n" \
           "\t- Data path: $HORSE_BASE/data/%s/db*/*.%s",folder,format);
    }
}

static L initDBTable(L n, CS PRE_DEFINED[], L* SYM_LIST_LINE){
    DOI(n, insertSym(createSymbol((S)PRE_DEFINED[i])));
    // printAllSymol();
    DOI(n, SYM_LIST_LINE[i]= getSymbol((S)PRE_DEFINED[i]));
    R 0;
}

static L readTableRegion(){
    C CSV_LINE[128]; setTpchPath(CSV_LINE, (S)"region");
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
    C CSV_LINE[128]; setTpchPath(CSV_LINE, (S)"nation");
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
    C CSV_LINE[128]; setTpchPath(CSV_LINE, (S)"customer");
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
    C CSV_LINE[128]; setTpchPath(CSV_LINE, (S)"orders");
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
    C CSV_LINE[128]; setTpchPath(CSV_LINE, (S)"lineitem");
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
    C CSV_LINE[128]; setTpchPath(CSV_LINE, (S)"part");
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
    C CSV_LINE[128]; setTpchPath(CSV_LINE, (S)"supplier");
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
    C CSV_LINE[128]; setTpchPath(CSV_LINE, (S)"partsupp");
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

static const char *tpchName[] = { \
    "region",   "nation",   "customer", \
    "orders",   "lineitem", "part",     \
    "supplier", "partsupp" };

static L (*tpchDB[8])() = { \
    readTableRegion,  readTableNation  , readTableCustomer, \
    readTableOrders,  readTableLineitem, readTablePart, \
    readTableSupplier,readTablePartsupp };

static L readTpchTables(){
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

L metaTable(V x, S tableName){
    V keyV = getTableKeys(x);
    V valV = getTableVals(x);
    P("Table: %s\n", tableName);
    DOI(vn(keyV), \
        {P("[%2lld] %-15s %s\n",i,\
        getSymbolStr(vQ(keyV,i)),\
        getTypeName(vp(vV(valV,i))));})
    R 0;
}

/* load from bin */
L initTableFromBin(S tableName){
    C path[99]; setTpchBinPath(path, tableName);
    FILE *fp = fopen(path, "rb");
    if(!fp) EP("File %s open fails\n",path);
    V x = allocNode();
    readSerializeV(x, fp);
    fclose(fp);
    registerTable(tableName, x);
    metaTable(x, tableName);
    R 0;
}

/* load pl tables */

static L readTableBlackScholes(){
    C CSV_LINE[128]; SP(CSV_LINE, "/home/sable/hanfeng.c/github/benchmark-udf/data/in_1M.tbl");
    P("File: %s\n", CSV_LINE);
    L TYPE_LINE[]  = {H_FLT, H_FLT, H_FLT, H_FLT, H_FLT, H_FLT,
                      H_C  , H_FLT, H_FLT};
    const L NUM_COL_LINE = 9;
    Q SYM_LIST_LINE[NUM_COL_LINE];
    const C* PRE_DEFINED[] = {
        "sptprice", "strike", "rate", "divq", "volatility", "time",
        "optiontype", "divs", "dgrefval"
    };

    initDBTable(NUM_COL_LINE, PRE_DEFINED, SYM_LIST_LINE);
    V tableBlackScholes = readCSV(CSV_LINE, NUM_COL_LINE, TYPE_LINE, SYM_LIST_LINE);
    registerTable((S)"blackscholes", tableBlackScholes);
    R 0;
}


/* load tables */

L initTableByName(S tableName){
    if(!findTableByName(getSymbol(tableName))){
        P("Loading table %s\n",tableName);
        if(isReadBin){
            P(">> Reading from bin\n");
            initTableFromBin(tableName);
        }
        else{
            if(sEQ(tableName, "region"))
                readTableRegion();
            else if(sEQ(tableName, "nation"))
                readTableNation();
            else if(sEQ(tableName, "customer"))
                readTableCustomer();
            else if(sEQ(tableName, "orders"))
                readTableOrders();
            else if(sEQ(tableName, "lineitem"))
                readTableLineitem();
            else if(sEQ(tableName, "part"))
                readTablePart();
            else if(sEQ(tableName, "supplier"))
                readTableSupplier();
            else if(sEQ(tableName, "partsupp"))
                readTablePartsupp();
            else if(sEQ(tableName, "blackscholes"))
                readTableBlackScholes();
            else EP("Table %s NOT FOUND\n",tableName);
        }
    }
    else {
        P("Table %s has been loaded\n",tableName);
    }
    R 0;
}


