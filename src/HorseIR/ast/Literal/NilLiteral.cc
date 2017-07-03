#include <vector>
#include "../grammar/HorseIRParser.h"

#include "../Literal.h"

using namespace horseIR::ast ;

NilLiteral::NilLiteral(HorseIRParser::LiteralCaseNilContext *cst, ASTNode::MemManagerType &mem)
    : Literal(cst, mem, Literal::LiteralClass::NilLiteral, ASTNode::ASTNodeType::NilLiteral)
{
    throw std::runtime_error("not yet implement") ;
}

NilLiteral::NilLiteral(ASTNode::MemManagerType &mem)
    : Literal(mem, Literal::LiteralClass::NilLiteral, ASTNode::ASTNodeType::NilLiteral),
      type(nullptr)
{}

horseIR::ast::Type* NilLiteral::getLiteralType() const
{
    return type ;
}

NilLiteral::InternalType NilLiteral::getValue() const
{
    return nullptr ;
}

std::size_t NilLiteral::getNumNodesRecursively() const
{
    if (type != nullptr) {
        return type->getNumNodesRecursively() + 1 ;
    } else {
        return 1 ;
    }
}

std::vector<ASTNode*> NilLiteral::getChildren() const
{
    if (type != nullptr) {
        return std::vector<ASTNode*> {type} ;
    } else {
        return std::vector<ASTNode*> {} ;
    }
}

std::string NilLiteral::toString() const
{
    return "nil :" + ((type != nullptr)? type->toString(): "nullptr") ;
}

std::string NilLiteral::toTreeString() const
{
    return "(NilLiteral " + ((type != nullptr)? type->toTreeString(): "nullptr") + ")" ;
}