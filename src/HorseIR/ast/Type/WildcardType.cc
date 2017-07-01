#include <string>
#include <cassert>

#include "../Type.h"

using namespace horseIR::ast ;

WildcardType::WildcardType(HorseIRParser::TypeCaseWildcardContext* cst, ASTNode::MemManagerType& mem)
    : Type(cst, mem, Type::TypeClass::Wildcard, ASTNode::ASTNodeType::WildcardType)
{}

WildcardType::WildcardType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::Wildcard, ASTNode::ASTNodeType::WildcardType)
{}

bool WildcardType::isGeneralizationOf(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    return true ;
}

std::size_t WildcardType::getNumNodesRecursively() const
{
    return 1 ;
}

std::vector<ASTNode*> WildcardType::getChildren() const
{
    return std::vector<ASTNode*>() ;
}

std::string WildcardType::toString() const
{
    return "?" ;
}

std::string WildcardType::toTreeString() const
{
    return "(WildcardType)" ;
}
