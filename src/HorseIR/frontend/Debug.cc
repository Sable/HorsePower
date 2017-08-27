#include <iostream>

#include "./ast/AST.h"
#include "./ast/CSTConverter.h"
#include "interpreter/Interpreter.h"
#include "../backend/h_io.h"

using namespace horseIR;

const char *rawProgram = R"PROGRAM(
/*
 * varchar(99) -> sym
 * int         -> i64
 */
module default {
    import Builtin.*;
    def find_valid_index(colVal:i64, indexBool:i64) : i64 {
        colSize   :? = @len(colVal);
        validBool :? = @lt(indexBool,colSize);
        indexSize :? = @len(indexBool);
        indexRange:? = @range(indexSize);
        validIndex:? = @compress(validBool, indexRange);
        return validIndex;
    }
    def find_valid_item(colVal:i64, indexBool:i64)  : i64 {
        colSize   :? = @len(colVal);
        validBool :? = @lt(indexBool,colSize);
        validItem :? = @compress(validBool, indexBool);
        return validItem;
    }
    def main() : table {
        a0:table = @load_table(`Employee);
        a1:table = @load_table(`Department);

        s0:sym = check_cast(@column_value(a0, `LastName)      , sym);
        s1:i64 = check_cast(@column_value(a0, `DepartmentID)  , i64);
        s2:i64 = check_cast(@column_value(a1, `DepartmentID)  , i64);
        s3:sym = check_cast(@column_value(a1, `DepartmentName), sym);

        t0:i64 = @index_of       (s2,s1);
        t1:i64 = @find_valid_index(s2,t0);
        t2:i64 = @find_valid_item (s2,t0);

        r0:sym = @index(s0,t1);
        r1:i64 = @index(s1,t1);
        r2:sym = @index(s3,t2);

        k0:sym       = (`LastName,`DepartmentID,`DepartmentName);
        k1:list<sym> = @tolist(k0);
        k2:list<?>   = @list(r0,r1,r2);
        z:table      = @table(k1,k2);

        return z;
    }
}
)PROGRAM";

int main (int argc, const char *argv[])
{
  initMain ();
  initSym ();
  initSys ();

  insertSym (createSymbol ((S) "Employee"));
  insertSym (createSymbol ((S) "LastName"));
  insertSym (createSymbol ((S) "DepartmentID"));
  insertSym (createSymbol ((S) "Department"));
  insertSym (createSymbol ((S) "DepartmentName"));

  const char *EMP_PATH = "employee.csv";
  const std::size_t EMP_NUM_COL = 2;
  const L EMP_TYPES[] = {H_Q, H_I};
  const Q EMP_HEADER[] = {
      getSymbol ((S) "LastName"), getSymbol ((S) "DepartmentID")
  };
  V EMP = readCSV (
      (S) EMP_PATH, (L) EMP_NUM_COL, (L*) EMP_TYPES, (Q*) EMP_HEADER
  );
  registerTable ((S) "Employee", EMP);
  printV (EMP);

  const char *DEP_PATH = "department.csv";
  const std::size_t DEP_NUM_COL = 2;
  const L DEP_TYPES[] = {H_I, H_Q};
  const Q DEP_HEADER[] = {
      getSymbol ((S) "DepartmentID"), getSymbol ((S) "DepartmentName")
  };
  V DEP = readCSV (
      (S) DEP_PATH, (L) DEP_NUM_COL, (L*) DEP_TYPES, (Q*) DEP_HEADER
  );
  registerTable ((S) "Department", DEP);
  printV (DEP);


  antlr4::ANTLRInputStream stream (rawProgram);
  HorseIRLexer lexer (&stream);
  antlr4::CommonTokenStream tokenStream (&lexer);
  HorseIRParser parser (&tokenStream);
  auto context = parser.program ();

  ast::ASTNode::ASTNodeMemory mem;

  auto astNode = ast::CSTConverter::convert (mem, context);

  interpreter::Interpreter interpreter (astNode);
  printV (interpreter.interpret ("default", "main"));

  return 0;
}