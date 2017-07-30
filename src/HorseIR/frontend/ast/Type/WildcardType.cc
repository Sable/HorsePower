#include <string>
#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

WildcardType::WildcardType(ASTNode* parent,
                           HorseIRParser::TypeCaseWildcardContext* cst,
                           ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::Wildcard, ASTNode::ASTNodeClass::WildcardType)
{}

WildcardType::WildcardType(HorseIRParser::TypeCaseWildcardContext* cst,
                           ASTNode::MemManagerType& mem)
    : WildcardType(nullptr, cst, mem)
{}

WildcardType::WildcardType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::Wildcard, ASTNode::ASTNodeClass::WildcardType)
{}

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

WildcardType* WildcardType::duplicateShallow(ASTNode::MemManagerType &mem) const
{
    WildcardType* wildcardType = new WildcardType(mem) ;
    wildcardType->__duplicateShallow(this) ;
    return wildcardType ;
}

WildcardType* WildcardType::duplicateDeep(ASTNode::MemManagerType &mem) const
{
    WildcardType* wildcardType = new WildcardType(mem) ;
    wildcardType->__duplicateDeep(this, mem) ;
    return wildcardType ;
}

bool WildcardType::isGeneralizationOf(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    return true ;
}

bool WildcardType::isSameAs(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    return type->getTypeClass() == Type::TypeClass::Wildcard ;
}

void WildcardType::__duplicateShallow(const WildcardType* wildcardType)
{
    assert(wildcardType) ;
    return ;
}

void WildcardType::__duplicateDeep(const WildcardType* wildcardType, ASTNode::MemManagerType& mem)
{
    assert(wildcardType) ;
    (void) mem ;
    return ;
}
