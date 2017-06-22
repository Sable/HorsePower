#include <string>
#include <cassert>

#include "Type.h"

using ListType = horseIR::ast::ListType ;
using Type = horseIR::ast::Type ;

ListType::ListType(HorseIRParser::TypeCaseListContext* cst, ASTNode::MemManagerType& mem)
    : Type(cst, mem, Type::TypeClass::List)
{
    assert(cst != nullptr) ;

    auto listCST = static_cast<HorseIRParser::TypeListContext*>(cst->typeList()) ;
    elementType = Type::makeTypeASTNode(listCST->element, mem) ;
    this->children.push_back(elementType) ;
}

ListType::ListType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::List),
      elementType(nullptr)
{}
      
bool ListType::isGeneralizationOf(const Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::List) return false ;
    auto listType = static_cast<const ListType*>(type) ;
    return elementType->isGeneralizationOf(listType->elementType) ;
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
    
    if (elementType != nullptr) {
        this->children.clear() ;
    }
    elementType = const_cast<Type*>(type) ;
    this->children.push_back(elementType) ;
    return *this ;
}
