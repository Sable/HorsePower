#include <string>
#include <cassert>

#include "Type.h"

using ASTNode = horseIR::ast::ASTNode ;
using EnumerationType = horseIR::ast::EnumerationType ;
using Type = horseIR::ast::Type ;

EnumerationType::EnumerationType(HorseIRParser::TypeCaseEnumContext* cst, ASTNode::MemManagerType& mem)
    : Type(cst, mem, Type::TypeClass::Enumeration, ASTNode::ASTNodeType::EnumerationType)
{
    assert(cst != nullptr) ;
    
    auto enumCST = static_cast<HorseIRParser::TypeEnumContext*>(cst->typeEnum()) ;
    elementType = Type::makeTypeASTNode(enumCST->element, mem) ;
}

EnumerationType::EnumerationType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::Enumeration, ASTNode::ASTNodeType::EnumerationType),
      elementType(nullptr)
{}

bool EnumerationType::isGeneralizationOf(const Type *type) const {
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Enumeration) return false ;
    auto enumType = static_cast<const EnumerationType*>(type) ;

    return elementType->isGeneralizationOf(enumType->elementType) ;
}

std::size_t EnumerationType::getNumNodesRecursively() const
{
    if (elementType == nullptr) {
        return 1 ;
    } else {
        return elementType->getNumNodesRecursively() + 1 ;
    }
}

std::vector<ASTNode*> EnumerationType::getChildren() const
{
    return std::vector<ASTNode*>{elementType} ;
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
    assert(type != nullptr) ;
    elementType = const_cast<Type*>(type) ;
    return *this ;
}
