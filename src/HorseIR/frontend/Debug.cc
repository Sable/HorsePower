#include <iostream>

#include "./ast/AST.h"
#include "./ast/CSTConverter.h"

using namespace horseIR;

const char *rawProgram = R"PROGRAM(
/*
 * varchar(99) -> sym
 * int         -> i64
 */
module default {
    import Builtin.plus;
    def inc_by_one(num :i32) :i32 {
        ret :? = @plus(num, 1 :i32);
        return ret;
    }

    def main() : table {
        v1:? = 1 :i32;
        v2:? = @inc_by_one(v1);
        return v2;
    }
}
)PROGRAM";

#include <chrono>
#include <thread>
#include "ast/ASTPrinter.h"
#include "interpreter/Interpreter.h"
#include "../backend/h_io.h"

int main (int argc, const char *argv[])
{
  antlr4::ANTLRInputStream stream (rawProgram);
  HorseIRLexer lexer (&stream);
  antlr4::CommonTokenStream tokenStream (&lexer);
  HorseIRParser parser (&tokenStream);
  auto context = parser.program ();

  ast::ASTNode::ASTNodeMemory mem;

  auto astNode = ast::CSTConverter::convert (mem, context);

  ast::ASTPrinter printer (std::cout);
  printer.print (astNode);
  std::cout << std::endl;

  initMain ();
  initSys ();

  using namespace horseIR::interpreter;
  Interpreter interpreter (astNode);
  printV (interpreter.interpret ("default", "main"));

  return 0;
}