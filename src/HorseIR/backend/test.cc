#include <iostream>
#include "../frontend/build/include/ast/AST.h"
#include "antlr4-runtime.h"

#include "h_global.h"


const char* program = ""
    "/*.  \n"
    " * varchar(99) -> sym  \n"
    " * int         -> i64 \n"
    " */  \n"
    "module default {  \n"
    "    import Builtin.*;  \n"
    "    def main() : table {  \n"
    "        s0:list<sym> = check_cast(@column_value(`Employee,   `LastName)      , list<sym>);  \n"
    "        s1:list<i64> = check_cast(@column_value(`Employee,   `DepartmentID)  , list<i64>);  \n"
    "        s2:list<i64> = check_cast(@column_value(`Department, `DepartmentID)  , list<i64>);  \n"
    "        s3:list<sym> = check_cast(@column_value(`Department, `DepartmentName), list<sym>);  \n"
    "  \n"
    "        t0:list<i64> = @index_of        (s2,s1);  \n"
    "        t1:list<i64> = @find_valid_index(s2,t0);  \n"
    "        t2:list<i64> = @find_valid_item (s2,t0);  \n"
    "  \n"
    "        r0:list<sym> = @index(s0,t1);  \n"
    "        r1:list<i64> = @index(s1,t1);  \n"
    "        r2:list<sym> = @index(s3,t2);  \n"
    "  \n"
    "        d0:dict<sym,sym> = @dict(`LastName      , r0);  \n"
    "        d1:dict<sym,i64> = @dict(`DepartmentID  , r1);  \n"
    "        d2:dict<sym,sym> = @dict(`DepartmentName, r2);  \n"
    "  \n"
    "        z0:?    = @list(d0,d1,d2);  \n"
    "        z:table = @table(z0);  \n"
    "        return z;  \n"
    "    }  \n"
    "  \n"
    "}  \n"
    ;

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

L testInputFile(S filePath){
    if(!filePath){
        fprintf(stderr, "Correct path must be set (-p path)\n");
        exit(99);
    }
    P("path = %s\n", filePath);
    // readFile(filePath, 1);
    R 0;
}

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

/* return -1, if too large */
L getNiceNumber(L n){
    L k = 1, c = 62;
    while(c>0 && k<n){c--;k<<=1;}
    R (k<n?-1:k);
}

#define CHECK(e, x) { \
    if(e!=0) { P("Error at line %d, (err=%lld)\n",x,e); exit(99); } \
    if(H_DEBUG)P("Pass line %d\n",x); }

V handleLiteral(horseIR::ast::Literal* literal)
{
    // using Literal = horseIR::ast::Literal ;
    // using SymbolLiteral = horseIR::ast::SymbolLiteral ;
    // switch (literal->getLiteralClass()) {
    // case Literal::LiteralClass::SymbolLiteral :
    //     R initSymbol(allocNode(), getSymbol((S) static_cast<SymbolLiteral*>(literal)->getValue().c_str())) ;
    // default:
    //     assert(false) ;
    //     return nullptr ;
    // }
    R NULL;
}

V handleParameter(horseIR::ast::Operand* operand, std::map<std::string, V>& env)
{
    // using Literal = horseIR::ast::Literal ;
    // using Identifier = horseIR::ast::Identifier ;
    // switch (operand->getOperandClass()) {
    // case horseIR::ast::Operand::OperandClass::Identifier:
    //     return env[static_cast<Identifier*>(operand)->getFullName()] ;
    // case horseIR::ast::Operand::OperandClass::Literal:
    //     return handleLiteral(static_cast<Literal*>(operand)) ;
    // }
    R NULL;
}
        

L simulateSimple(){
    // antlr4::ANTLRInputStream inStream(program) ;
    // horseIR::HorseIRLexer lexer(&inStream) ;
    // antlr4::CommonTokenStream tokenStream(&lexer) ;
    // horseIR::HorseIRParser parser(&tokenStream) ;
    // horseIR::HorseIRParser::ProgramContext* program = parser.program() ;

    // horseIR::ast::ASTNode::MemManagerType mem ;
    // auto* compilationUnit = new horseIR::ast::CompilationUnit(program, mem) ;
    // auto* module = compilationUnit->getModule("default") ;
    // auto* method = module->getMethod("main") ;

    // std::map<std::string, V> variableStack ;
    
    // for (horseIR::ast::StatementIterator iterator = method->begin(); iterator != method->end(); ) {
    //     auto* statement = *iterator ;
    //     switch (statement->getStatementClass()) {
    //     case horseIR::ast::Statement::StatementClass::Assign: {
    //         auto* invokeStmt = static_cast<horseIR::ast::AssignStatement*>(statement) ;
    //         // assert(invokeStmt->isInvocation()) ;
    //         assert(invokeStmt->getIsInvocation()) ;
    //         auto* invokeTargetOperand = invokeStmt->getInvokeTarget() ;
    //         assert(invokeTargetOperand->getOperandClass() == horseIR::ast::Operand::OperandClass::Literal) ;
    //         auto* literal = static_cast<horseIR::ast::Literal*>(invokeTargetOperand) ;
    //         assert(literal->getLiteralClass() == horseIR::ast::Literal::LiteralClass::FunctionLiteral) ;
    //         auto* function = static_cast<horseIR::ast::FunctionLiteral*>(literal) ;
    //         std::string builtinFunc = function->getValue() ;
    //         V lhsValue = allocNode() ;
    //         const std::vector<horseIR::ast::Operand*> parameters = invokeStmt->getParameters() ;
    //         if (builtinFunc == "column_value") {
    //             pfnColumnValue(lhsValue,
    //                            handleParameter(parameters[0], variableStack),
    //                            handleParameter(parameters[1], variableStack)) ;
    //         } else if (builtinFunc == "index_of") {
    //             pfnIndexOf(lhsValue,
    //                        handleParameter(parameters[0], variableStack),
    //                        handleParameter(parameters[1], variableStack)) ;
    //         } else if (builtinFunc == "find_valid_index") {
    //             pfnFindValidIndex(lhsValue,
    //                               handleParameter(parameters[0], variableStack),
    //                               handleParameter(parameters[1], variableStack)) ;
    //         } else if (builtinFunc == "find_valid_item") {
    //             pfnFindValidItem(lhsValue,
    //                              handleParameter(parameters[0], variableStack),
    //                              handleParameter(parameters[1], variableStack)) ;
    //         } else if (builtinFunc == "index") {
    //             pfnIndex(lhsValue,
    //                      handleParameter(parameters[0], variableStack),
    //                      handleParameter(parameters[1], variableStack)) ;
    //         } else if (builtinFunc == "dict") {
    //             pfnDict(lhsValue,
    //                     handleParameter(parameters[0], variableStack),
    //                     handleParameter(parameters[1], variableStack)) ;
    //         } else if (builtinFunc == "list") {
    //             pfnList(lhsValue, 3,
    //                     handleParameter(parameters[0], variableStack),
    //                     handleParameter(parameters[1], variableStack),
    //                     handleParameter(parameters[2], variableStack)) ;
    //         } else if (builtinFunc == "table") {
    //             pfnTable(lhsValue,
    //                      handleParameter(parameters[0], variableStack)) ;
    //         } else {
    //             assert(false) ;
    //             break;
    //         }
    //         variableStack[invokeStmt->getLHSName()->getFullName()] = lhsValue ;
    //         break ;
    //     }
    //     case horseIR::ast::Statement::StatementClass::Return : {
    //         break ;
    //     }
    //     default: {
    //         assert(false) ;
    //         break ;
    //     }
    //     }
    //     (void) iterator.nextOnTrue() ;
    // }

    // P("\n");
    // printTablePretty(variableStack["<local>.z"], -1);
    R 0;
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

    e = pfnTolist(k1, k0);          CHECK(e,14);
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

void testOrderBy(){
    P("Testing order by\n");
    V a = allocNode();
    initV(a,H_G,2);
    L value_A[] = {32,12,7,78,12,45};
    L value_B[] = {56,34,32,45,78,23};
    fillItem(vV(a,0),value_A,6);
    fillItem(vV(a,1),value_B,6);
    P("a = "); printList(a); P("\n");
    V b = allocNode();
    initV(b,H_B,2);
    vB(b,0) = true;  //ASC
    vB(b,1) = false; //DESC
    P("b = "); printList(b); P("\n");
    V z = allocNode();
    pfnOrderBy(z,a,b);
    printList(z);
}

#include "test_types.h"
#include "test_tpch_q6.h"
#include "test_pfns.h"
#include "test_read_tpch.h"

L testMain(){
    initMain();  // memory
    initSym();   // symbol
    initSys();
    initTable(); // table
    L op = 7;
    switch(op){
        case 1: {
            P("Reading table Employee\n");
            V tableEmp = readCSV(CSV_EMP, NUM_COL_EMP, TYPE_EMP, SYM_LIST_EMP);
            registerTable((S)"Employee", tableEmp);
            P("Reading table Department\n");
            V tableDep = readCSV(CSV_DEP, NUM_COL_DEP, TYPE_DEP, SYM_LIST_DEP);
            registerTable((S)"Department", tableDep);
            /* Simulation */
            // simulateSimple();
            simulateSimpleRaw();
        } break;
        case 2: testTypes();      break;
        case 3: testOrderBy();    break;
        case 4: testTPCHQ6();     break;
        case 5: testPfns();       break;
        case 6: testMemory();     break;
        case 7: readTpchTables(); break;
        default: break;
    }
    /* Print info */
    printSymInfo();
    printHeapInfo();
    getchar();
    R 0;
}

// L loadCSV(S filePath, L types, L size){
//  R readFile((S)CSV_EMP, 1);
// }

