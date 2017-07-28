#include <string>
#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

EnumerationType::EnumerationType(ASTNode* parent, HorseIRParser::TypeCaseEnumContext* cst, ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::Enumeration, ASTNode::ASTNodeClass::EnumerationType)
{
    assert(cst != nullptr) ;
    
    auto enumCST = cst->typeEnum() ;
    elementType = Type::makeTypeASTNode(this, enumCST->element, mem) ;
}

EnumerationType::EnumerationType(HorseIRParser::TypeCaseEnumContext* cst, ASTNode::MemManagerType& mem)
    : EnumerationType(nullptr, cst, mem) 
{}

EnumerationType::EnumerationType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::Enumeration, ASTNode::ASTNodeClass::EnumerationType),
      elementType(nullptr)
{}

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

EnumerationType* EnumerationType::duplicateShallow(ASTNode::MemManagerType &mem) const
{
    EnumerationType* enumerationType = new EnumerationType(mem) ;
    enumerationType->__duplicateShallow(this) ;
    return enumerationType ;
}

EnumerationType* EnumerationType::duplicateDeep(ASTNode::MemManagerType &mem) const
{
    EnumerationType* enumerationType = new EnumerationType(mem) ;
    enumerationType->__duplicateDeep(this, mem) ;
    return enumerationType ;
}

bool EnumerationType::isGeneralizationOf(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Enumeration) return false ;
    auto castedPtr = static_cast<const EnumerationType*>(type) ;
    return elementType->isGeneralizationOf(castedPtr->getElementType()) ;
}

Type* EnumerationType::getElementType() const
{
    return elementType ;
}

EnumerationType& EnumerationType::setElementType(Type *type)
{
    assert(type != nullptr) ;
    elementType = type ;
    return *this ;
}

void EnumerationType::__duplicateShallow(const EnumerationType* enumType)
{
    assert(enumType != nullptr) ;
    elementType = enumType->elementType ;
    return ;
}

void EnumerationType::__duplicateDeep(const EnumerationType* enumType, ASTNode::MemManagerType& mem)
{
    assert(enumType != nullptr) ;
    Type* duplicateElementType = nullptr ;
    if (enumType->elementType != nullptr) {
        duplicateElementType = static_cast<Type*>(enumType->elementType->duplicateDeep(mem)) ;
        (void) duplicateElementType->setParentASTNode(this) ;
    }
    elementType = duplicateElementType ;
    return ;
}
