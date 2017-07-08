#include <vector>
#include <sstream>
#include <cassert>
#include "../grammar/HorseIRParser.h"

#include "../Literal.h"

using namespace horseIR::ast ;

SymbolLiteral::SymbolLiteral(HorseIRParser::LiteralCaseSymbolContext *cst, ASTNode::MemManagerType &mem)
    : Literal(cst, mem, Literal::LiteralClass::SymbolLiteral, ASTNode::ASTNodeClass::SymbolLiteral),
      type(new ScalarType(ScalarType::ScalarClass::Symbol, mem))
{
    assert(cst != nullptr);

    HorseIRParser::LiteralSymbolContext *context = cst->literalSymbol();
    const std::string lex = context->value->getText();
    value = lex.substr(1, lex.length());
}

SymbolLiteral::SymbolLiteral(ASTNode::MemManagerType &mem)
    : Literal(mem, Literal::LiteralClass::SymbolLiteral, ASTNode::ASTNodeClass::SymbolLiteral),
      value(),
      type(new ScalarType(ScalarType::ScalarClass::Symbol, mem))
{}

horseIR::ast::Type* SymbolLiteral::getLiteralType() const
{
    return type ;
}

SymbolLiteral::InternalType SymbolLiteral::getValue() const
{
    return value ;
}

std::size_t SymbolLiteral::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> SymbolLiteral::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string SymbolLiteral::toString() const
{
    std::ostringstream stream ;
    stream << '`' << value ;
    if (type != nullptr) {
        stream << " :" << type->toString() ;
    } else {
        stream << " :nullptr" ;
    }
    return stream.str() ;
}

std::string SymbolLiteral::toTreeString() const
{
    return "(SymbolLiteral " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}