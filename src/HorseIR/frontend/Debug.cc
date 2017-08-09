#include <iostream>

#include "./ast/AST.h"

using namespace horseIR;

int main (int argc, const char *argv[])
{
  std::cout << "Demo" << std::endl;

  ast::ASTNode::ASTNodeMemory mem;
  auto type = new ast::PrimitiveType (mem);
  type->setPrimitiveClass (ast::PrimitiveType::PrimitiveClass::KeyTable) ;
  std::cout << type->toString () << std::endl ;

  return 0;
}