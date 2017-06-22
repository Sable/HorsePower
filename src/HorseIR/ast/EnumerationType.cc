#include <string>
#include <cassert>

#include "Type.h"

using EnumerationType = horseIR::ast::EnumerationType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
using Type = horseIR::ast::Type ;

EnumerationType::EnumerationType(HorseIRParser::TypeCaseEnumContext* cst, MemoryManager<ASTNode>& mem)
    : Type(cst, mem, Type::TypeClass::Enumeration)
{
    assert(cst != nullptr) ;
    
    auto enumCST = static_cast<HorseIRParser::TypeEnumContext*>(cst->typeEnum()) ;
    elementType = Type::makeTypeASTNode(enumCST->element, mem) ;
    this->children.push_back(elementType) ;
}

EnumerationType::EnumerationType(MemoryManager<ASTNode>& mem)
    : Type(mem, Type::TypeClass::Enumeration),
      elementType(nullptr)
{}

bool EnumerationType::isGeneralizationOf(const Type *type) const {
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Enumeration) return false ;
    auto enumType = static_cast<const EnumerationType*>(type) ;

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

EnumerationType& EnumerationType::setElementType(const Type *type)
{
    if (elementType != nullptr) {
        this->children.erase(std::remove_if(this->children.begin(), this->children.end(),
                                            [=] (ASTNode* p_search) -> bool {
                                                return p_search == elementType ;
                                            })) ;
    }
    elementType = const_cast<Type*>(type) ;
    this->children.push_back(elementType) ;
    return *this ;
}
