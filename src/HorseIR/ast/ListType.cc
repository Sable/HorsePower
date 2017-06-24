#include <string>
#include <cassert>

#include "Type.h"

using ASTNode = horseIR::ast::ASTNode ;
using ListType = horseIR::ast::ListType ;
using Type = horseIR::ast::Type ;

ListType::ListType(HorseIRParser::TypeCaseListContext* cst, ASTNode::MemManagerType& mem)
    : Type(cst, mem, Type::TypeClass::List, ASTNode::ASTNodeType::ListType)
{
    assert(cst != nullptr) ;

    auto listCST = static_cast<HorseIRParser::TypeListContext*>(cst->typeList()) ;
    elementType = Type::makeTypeASTNode(listCST->element, mem) ;
}

ListType::ListType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::List, ASTNode::ASTNodeType::ListType),
      elementType(nullptr)
{}
      
bool ListType::isGeneralizationOf(const Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::List) return false ;
    auto listType = static_cast<const ListType*>(type) ;
    return elementType->isGeneralizationOf(listType->elementType) ;
}

std::size_t ListType::getNumNodesRecursively() const
{
    if (elementType == nullptr) {
        return 1 ;
    } else {
        return elementType->getNumNodesRecursively() + 1 ;
    }
}

std::vector<ASTNode*> ListType::getChildren() const
{
    return std::vector<ASTNode*>{elementType} ;
}

std::string ListType::toString() const
{
    return "list<" + elementType->toString() + ">" ;
}

std::string ListType::toTreeString() const
{
    return "(ListType " + elementType->toTreeString() + ")" ;
}

constexpr Type* ListType::getElementType() const
{
    return elementType ;
}

ListType& ListType::setElementType(const Type *type)
{
    assert(type != nullptr) ;
    elementType = const_cast<Type*>(type) ;
    return *this ;
}
