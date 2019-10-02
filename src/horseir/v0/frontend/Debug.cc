#include <iostream>

#include "./ast/AST.h"
#include "./ast/CSTConverter.h"
#include "interpreter/Interpreter.h"
#include "../backend/h_io.h"

using namespace horseIR;

const char *rawProgram = R"PROGRAM(
module default{
    import Builtin.*;
    def main() : table {
        a0:table      = @load_table(`lineitem);

        t0:f64        = check_cast(@column_value(a0, `l_extendedprice), f64);
        t1:f64        = check_cast(@column_value(a0, `l_discount     ), f64);
        t2:d          = check_cast(@column_value(a0, `l_shipdate     ), d  );
        t3:f64        = check_cast(@column_value(a0, `l_quantity     ), f64);

        t4:bool       = @geq(t2, 1994.01.01 :d);
        t5:d          = @datetime_add(1994.01.01 :d, 1:i64, `year);
        t6:bool       = @lt(t2, t5);
        t7:f64        = @minus(0.06 :f64, 0.01 :f64);
        t8:f64        = @plus(0.06 :f64, 0.01 :f64);

        t9:bool       = @gt(t1,t7);
        t10:bool      = @lt(t1,t8);
        t11:bool      = @and(t9,t10);
        t12:bool      = @lt(t3, 240 :i64);

        t13:bool      = @and(t4,t6);
        t14:bool      = @and(t13,t9);
        t15:bool      = @and(t14,t12);

        t16:f64       = @compress(t15,t0);
        t17:f64       = @compress(t15,t1);
        t18:f64       = @mul(t16, t17);
        t19:f64       = @sum(t18);

        t20:list<sym> = @tolist(`revenue);
        t21:list<f64> = @enlist(t19);
        t22:table     = @table(t20,t21);
        return t22;
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
/*
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
*/

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