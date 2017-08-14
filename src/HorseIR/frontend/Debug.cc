#include <iostream>

#include "./ast/AST.h"
#include "./ast/CSTConverter.h"

using namespace horseIR;

const char *rawProgram = ""
    "/*                                                                                        \n"
    " * varchar(99) -> sym                                                                     \n"
    " * int         -> i64                                                                     \n"
    " */                                                                                       \n"
    "module default {                                                                          \n"
    "    import Builtin.*;                                                                     \n"
    "    def find_valid_index(colVal :list<i64>, indexBool :list<i64>) : list<i64> {           \n"
    "        colSize   :? =  @len(colVal);                                                     \n"
    "        validBool :? = @lt(indexBool,colSize);                                            \n"
    "        indexSize :? = @len(indexBool);                                                   \n"
    "        indexRange:? = @range(indexSize);                                                 \n"
    "        validIndex:? = @reduce(validBool, indexRange);                                    \n"
    "        return validIndex;                                                                \n"
    "    }                                                                                     \n"
    "    def find_valid_item(colVal :list<i64>, indexBool :list<i64>)  : list<i64> {           \n"
    "        colSize   :? = @len(colVal);                                                      \n"
    "        validBool :? = @lt(indexBool,colSize);                                            \n"
    "        validItem :? = @reduce(validBool, indexBool);                                     \n"
    "        return validItem;                                                                 \n"
    "    }                                                                                     \n"
    "    def main() : table {                                                                  \n"
    "        s0:list<sym> = check_cast(@column_value(`Employee,   `LastName)      , list<sym>);\n"
    "        s1:list<i64> = check_cast(@column_value(`Employee,   `DepartmentID)  , list<i64>);\n"
    "        s2:list<i64> = check_cast(@column_value(`Department, `DepartmentID)  , list<i64>);\n"
    "        s3:list<sym> = check_cast(@column_value(`Department, `DepartmentName), list<sym>);\n"
    "                                                                                          \n"
    "        t0:list<i64> = @index_of        (s2,s1);                                          \n"
    "        t1:list<i64> = @find_valid_index(s2,t0);                                          \n"
    "        t2:list<i64> = @find_valid_item (s2,t0);                                          \n"
    "                                                                                          \n"
    "        r0:list<sym> = @index(s0,t1);                                                     \n"
    "        r1:list<i64> = @index(s1,t1);                                                     \n"
    "        r2:list<sym> = @index(s3,t2);                                                     \n"
    "                                                                                          \n"
    "        d0:dict<sym,sym> = @dict(`LastName      , r0);                                    \n"
    "        d1:dict<sym,i64> = @dict(`DepartmentID  , r1);                                    \n"
    "        d2:dict<sym,sym> = @dict(`DepartmentName, r2);                                    \n"
    "                                                                                          \n"
    "        z0:?    = @list(d0,d1,d2);                                                        \n"
    "        z:table = @table(z0);                                                             \n"
    "        return z;                                                                         \n"
    "    }                                                                                     \n"
    "                                                                                          \n"
    "}                                                                                         \n"
    "";

int main (int argc, const char *argv[])
{
  const std::string raw = R"((null, 2012.12.21, 1996.0009.06, null))";

  antlr4::ANTLRInputStream stream (raw);
  HorseIRLexer lexer (&stream);
  antlr4::CommonTokenStream tokenStream (&lexer);
  HorseIRParser parser (&tokenStream);
  auto context = parser.literalTDate ();

  ast::ASTNode::ASTNodeMemory mem;

  auto astNode = ast::CSTConverter::convert (mem, context);
  astNode->setEnclosingFilename ("stdin");
  std::cout << astNode->toString () << std::endl;
  std::cout << astNode->getASTNodeClass () << std::endl;

}