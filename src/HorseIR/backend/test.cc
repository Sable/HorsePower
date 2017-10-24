
#include "h_global.h"

L testInputFile(S filePath){
    if(!filePath){
        fprintf(stderr, "Correct path must be set (-p path)\n");
        exit(99);
    }
    P("path = %s\n", filePath);
    // readFile(filePath, 1);
    R 0;
}

#define CHECK(e, x) { \
    if(e!=0) { P("Error at line %d, (err=%lld)\n",x,e); printErrMsg(e); exit(99); } \
    if(H_DEBUG)P("Pass line %d\n",x); }


V literalDate(L x){
    V z = allocNode();
    initV(z,H_D,1);
    vd(z) = x;
    R z;
}

V literalBool(L x){
    V z = allocNode();
    initV(z,H_B,1);
    vb(z) = x;
    R z;
}

V literalI64(L x){
    V z = allocNode();
    initV(z,H_L,1);
    vl(z) = x;
    R z;
}

V literalString(S s){
    V z = allocNode();
    initV(z,H_S,1);
    S t = allocStrMem(strlen(s));
    strcpy(t,s);
    vs(z) = t;
    R z;
}

V literalSym(S str){
    V z = allocNode();
    initV(z,H_Q,1);
    vq(z) = getSymbol(str);
    R z;
}

V literalF64(E x){
    V z = allocNode();
    initV(z,H_E,1);
    ve(z) = x;
    R z;
}

V literalChar(C x){
    V z = allocNode();
    initV(z,H_C,1);
    vc(z) = x;
    R z;
}

V literalSymVector(L n, S strs[]){
    V z = allocNode();
    initV(z,H_Q,n);
    DOI(n, vQ(z,i)=getSymbol(strs[i]))
    R z;
}

V literalBoolVector(L n, B b[]){
    V z = allocNode();
    initV(z,H_B,n);
    DOI(n, vB(z,i)=b[i])
    R z;
}

V literalI64Vector(L n, L b[]){
    V z = allocNode();
    initV(z,H_L,n);
    DOI(n, vL(z,i)=b[i])
    R z;
}

V literalStrVector(L n, S b[]){
    V z = allocNode();
    initV(z,H_S,n);
    DOI(n, {S t=allocStrMem(strlen(b[i])); strcpy(t,b[i]); vS(z,i)=t;})
    R z;
}

// #define PROFILE(n,x) x
#define PROFILE(n,x) { struct timeval tt_0, tt_1; \
        gettimeofday(&tt_0, NULL); L e = x; CHECK(e,n); gettimeofday(&tt_1, NULL); \
        P("[Profiling] Line %d: %g ms\n", n,calcInterval(tt_0,tt_1)/1000.0); }

B isOptimized = true;

#include "test_simple.h"
#include "test_types.h"
#include "test_pfns.h"
#include "test_tpch.h"
#include "test_pl.h"

L testMain(I qid){
    initMain();  // memory
    initSym();   // symbol
    initSys();
    L op = 4;
    switch(op){
        case 1: testSimple();     break;
        case 2: testTypes();      break;
        case 3: testOrderBy();    break;
        case 4: testTPCH(qid);    break;
        case 5: testPL(0);        break;
        case 6: testPfns();       break;
        case 7: testMemory();     break;
        case 8: readTpchTables(); break;
        default: break;
    }
    /* Print info */
    printSymInfo();
    printHeapInfo();
    R 0;
}

// L loadCSV(S filePath, L types, L size){
//  R readFile((S)CSV_EMP, 1);
// }

