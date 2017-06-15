#include <string>
#include <cassert>

#include "Type.h"

using EnumerationType = horseIR::ast::EnumerationType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
using Type = horseIR::ast::Type ;

EnumerationType::EnumerationType(HorseIRParser::TypeCaseEnumContext* cst, ASTNodeMemory& mem)
{
    assert(cst != nullptr) ;
    this->cst = static_cast<decltype(this->cst)>(cst) ;
    auto enumCST = static_cast<HorseIRParser::TypeEnumContext*>(cst->typeEnum()) ;

    elementType = Type::makeTypeASTNode(enumCST->element, mem) ;
    this->children.push_back(elementType) ;
}

Type::TypeClass EnumerationType::getTypeClass() const
{
    return Type::TypeClass::Enumeration ;
}

bool EnumerationType::isGeneralizationOf(Type *type) const {
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Enumeration) return false ;
    EnumerationType* enumType = static_cast<EnumerationType*>(type) ;

    return elementType->isGeneralizationOf(enumType->elementType) ;
}

std::string EnumerationType::toString() const
{
    return "enum<" + elementType->toString() + ">" ;
}

std::string EnumerationType::toTreeString() const
{
    return "(EnumerationType " + elementType->toTreeString() + ")" ;
}

constexpr Type* EnumerationType::getElementType() const
{
    return elementType ;
}
