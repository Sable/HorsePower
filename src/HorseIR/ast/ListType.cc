#include <string>
#include <cassert>

#include "Type.h"

using ListType = horseIR::ast::ListType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
using Type = horseIR::ast::Type ;

ListType::ListType(HorseIRParser::TypeCaseListContext* cst, ASTNodeMemory& mem)
{
    assert(cst != nullptr) ;
    this->cst = static_cast<decltype(this->cst)>(cst) ;
    auto listCST = static_cast<HorseIRParser::TypeListContext*>(cst->typeList()) ;
    
    elementType = Type::makeTypeASTNode(listCST->element, mem) ;
    this->children.push_back(elementType) ;
}

Type::TypeClass ListType::getTypeClass() const
{
    return Type::TypeClass::List ;
}

bool ListType::isGeneralizationOf(Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::List) return false ;
    ListType* listType = static_cast<ListType*>(type) ;
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
