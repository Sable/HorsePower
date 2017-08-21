#pragma once

#include "../ast/AST.h"

namespace horseIR
{
namespace analysis
{

}
class CFG {
 public:
  class BasicBlock {

  };

 protected:
  std::vector<std::unique_ptr<BasicBlock *>> basicBlocks;
  std::vector<ast::Method *> methods;
};

}
}