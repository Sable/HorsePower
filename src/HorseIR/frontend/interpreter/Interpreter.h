#pragma once

#include <unordered_map>

#include "Dispatcher.h"
#include "../ast/AST.h"
#include "../ast/ASTPrinter.h"
#include "../../backend/h_global.h"

namespace horseIR
{
namespace interpreter
{

class Interpreter {
 public:
  using IdentifierNumberType = std::size_t;

  // explicit Interpreter (ast::CompilationUnit *unit);

  // V interpret (std::size_t argc, V argv[]);
  // V interpret ();

  // protected:
  Dispatcher dispatcher;

  std::unordered_map<ast::Identifier *, IdentifierNumberType> idNumberingMap;
  void createIdentifierNumbering (ast::CompilationUnit *unit);
  void createIdentifierNumbering (ast::Module *module);
  void createIdentifierNumbering (ast::Method *method);

  class IdentifierNumberingPrinter : public ast::ASTPrinter {
   public:
    using MapType = std::unordered_map<ast::Identifier *, IdentifierNumberType>;

    IdentifierNumberingPrinter (std::ostream &stream, const MapType &pMap)
        : ast::ASTPrinter (stream), map (pMap)
    {}

   protected:
    const MapType &map;

    void caseIdentifier (const ast::Identifier *identifier) override
    {
      stream << identifier->getName () << '[' << map.at (
          const_cast<ast::Identifier*>(identifier)
      ) << ']';
    }
  };

};

}
}

