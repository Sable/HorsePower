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
    import Builtin.*;
    def calculate (x :i32) :i32 {
        equal_0 :bool = @eq(x, 0:i32);
        equal_1 :bool = @eq(x, 1:i32);
        equal_2 :bool = @eq(x, 2:i32);

        goto [ret] equal_1;
        goto [ret] equal_2;

        prev :i32 = @minus(x, 1:i32);
        prev2 :i32 = @minus(x, 2:i32);

        t0 :i32 = @calculate(prev);
        t1 :i32 = @calculate(prev2);

        ret :i32 = @plus(t0, t1);
        return ret;

        [ret]
        return 1:i32;
    }

    def main() :i32 {
        v :i32 = @calculate(10 :i32);
        return v ;
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