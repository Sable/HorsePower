// C CSV_EMP[] = "data/simple-join/employee.csv";
// C CSV_DEP[] = "data/simple-join/department.csv";
C CSV_EMP[] = "data/test-tables/employee.csv";
C CSV_DEP[] = "data/test-tables/department.csv";
L TYPE_EMP[]  = {H_Q, H_L};
L TYPE_DEP[]  = {H_L, H_Q};
const L NUM_COL_EMP = 2;
const L NUM_COL_DEP = 2;
Q SYM_LIST_EMP[2];
Q SYM_LIST_DEP[2];


void initTable(){
    const C* PRE_DEFINED[] = {
        "LastName", "DepartmentID",
        "DepartmentID", "DepartmentName"
    };
    DOI(4, insertSym(createSymbol((S)PRE_DEFINED[i])));
    // printAllSymol();
    SYM_LIST_EMP[0] = getSymbol((S)"LastName");
    SYM_LIST_EMP[1] = getSymbol((S)"DepartmentID");
    SYM_LIST_DEP[0] = getSymbol((S)"DepartmentID");
    SYM_LIST_DEP[1] = getSymbol((S)"DepartmentName");
}


/* UDF1: Find valid index */
L udfFindValidIndex(V validIndex, V colVal, V indexBool){
    L e;
    V colSize   = allocNode(), validBool  = allocNode();
    V indexSize = allocNode(), indexRange = allocNode();
    e = pfnLen     (colSize, colVal);                    CHECK(e,101);
    e = pfnLt      (validBool, indexBool, colSize);      CHECK(e,102);
    e = pfnLen     (indexSize, indexBool);               CHECK(e,103);
    e = pfnRange   (indexRange, indexSize);              CHECK(e,104);
    e = pfnCompress(validIndex, validBool, indexRange);  CHECK(e,105);
    R 0;
}

/* UDF2: Find valid item */
L udfFindValidItem(V validItem, V colVal, V indexBool){
    L e;
    V colSize = allocNode(), validBool = allocNode();
    e = pfnLen     (colSize, colVal);                   CHECK(e,201);
    e = pfnLt      (validBool, indexBool, colSize);     CHECK(e,202);
    e = pfnCompress(validItem, validBool, indexBool);   CHECK(e,203);
    R 0;
}

/*
 * steps of simple examples
 */
L simulateSimpleRaw(){
    L e;
    V s0 = allocNode();  V t0 = allocNode();  V r0 = allocNode();  V k0 = allocNode();
    V s1 = allocNode();  V t1 = allocNode();  V r1 = allocNode();  V k1 = allocNode();
    V s2 = allocNode();  V t2 = allocNode();  V r2 = allocNode();  V k2 = allocNode();
    V s3 = allocNode();
    V z0 = allocNode();  V z = allocNode();
    V a0 = allocNode();  V a1= allocNode();
    P("** Start simulation\n");
    e = pfnLoadTable(a0, initSymbol(allocNode(),getSymbol((S)"Employee")));   CHECK(e,1);
    e = pfnLoadTable(a1, initSymbol(allocNode(),getSymbol((S)"Department"))); CHECK(e,2);

    e = pfnColumnValue(s0, \
          a0, \
          initSymbol(allocNode(),getSymbol((S)"LastName")));       CHECK(e,3);
    e = pfnColumnValue(s1, \
          a0, \
          initSymbol(allocNode(),getSymbol((S)"DepartmentID")));   CHECK(e,4);
    e = pfnColumnValue(s2, \
          a1, \
          initSymbol(allocNode(),getSymbol((S)"DepartmentID")));   CHECK(e,5);
    e = pfnColumnValue(s3, \
          a1, \
          initSymbol(allocNode(),getSymbol((S)"DepartmentName"))); CHECK(e,6);

    e = pfnIndexOf       (t0, s2, s1);     CHECK(e,7);
    e = udfFindValidIndex(t1, s2, t0);     CHECK(e,8);
    e = udfFindValidItem (t2, s2, t0);     CHECK(e,9);

    e = pfnIndex    (r0, s0, t1);     CHECK(e,10);
    e = pfnIndex    (r1, s1, t1);     CHECK(e,11);
    e = pfnIndex    (r2, s3, t2);     CHECK(e,12);

    initV(k0,H_Q,3);                  CHECK(e,13);
    vQ(k0,0)=getSymbol((S)"LastName");
    vQ(k0,1)=getSymbol((S)"DepartmentID");
    vQ(k0,2)=getSymbol((S)"DepartmentName");

    e = pfnToList(k1, k0);          CHECK(e,14);
    V rn[] = {r0,r1,r2};
    e = pfnList(k2, 3, rn);         CHECK(e,15);
    e = pfnTable(z, k1, k2);        CHECK(e,16);

    P("Result of the join of table Employee and Department:\n\n");
    printTablePretty(z, -1);
    R 0;
}

void fillItem(V a, L *values, L len){
    initV(a,H_L,len);
    DOI(len, vL(a,i)=values[i])
}

void testSimple(){
	initTable(); // table
	P("Reading table Employee\n");
    V tableEmp = readCSV(CSV_EMP, NUM_COL_EMP, TYPE_EMP, SYM_LIST_EMP);
    registerTable((S)"Employee", tableEmp);
    P("Reading table Department\n");
    V tableDep = readCSV(CSV_DEP, NUM_COL_DEP, TYPE_DEP, SYM_LIST_DEP);
    registerTable((S)"Department", tableDep);
    /* Simulation */
    // simulateSimple();
    simulateSimpleRaw();
}

