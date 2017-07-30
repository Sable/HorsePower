#include <string>
#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

DictionaryType::DictionaryType(ASTNode* parent,
                               HorseIRParser::TypeCaseDictContext* cst,
                               ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::Dictionary, ASTNode::ASTNodeClass::DictionaryType)
{
    assert(cst != nullptr) ;

    auto dictCST = cst->typeDict() ;
    keyType = Type::makeTypeASTNode(this, dictCST->key, mem) ;
    valueType = Type::makeTypeASTNode(this, dictCST->value, mem) ;
}

DictionaryType::DictionaryType(HorseIRParser::TypeCaseDictContext* cst,
                               ASTNode::MemManagerType& mem)
    : DictionaryType(nullptr, cst, mem)
{}

DictionaryType::DictionaryType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::Dictionary, ASTNode::ASTNodeClass::DictionaryType),
      keyType(nullptr),
      valueType(nullptr)
{}

std::size_t DictionaryType::getNumNodesRecursively() const
{
    std::size_t count = 1 ;
    count += (keyType == nullptr)? 0: keyType->getNumNodesRecursively() ;
    count += (valueType == nullptr) ? 0: valueType->getNumNodesRecursively() ;
    return count ;
}

std::vector<ASTNode*> DictionaryType::getChildren() const
{
    return std::vector<ASTNode*>{keyType, valueType} ;
}

std::string DictionaryType::toString() const
{
    return "dict<" + keyType->toString() + ", " + valueType->toString() + ">" ;
}

std::string DictionaryType::toTreeString() const
{
    return "(DictionaryType " + keyType->toTreeString() + " " +
        valueType->toTreeString() + ")" ;
}

DictionaryType* DictionaryType::duplicateShallow(ASTNode::MemManagerType &mem) const
{
    DictionaryType* dictionaryType = new DictionaryType(mem) ;
    dictionaryType->__duplicateShallow(this) ;
    return dictionaryType ;
}

DictionaryType* DictionaryType::duplicateDeep(ASTNode::MemManagerType &mem) const
{
    DictionaryType* dictionaryType = new DictionaryType(mem) ;
    dictionaryType->__duplicateDeep(this, mem) ;
    return dictionaryType ;
}

bool DictionaryType::isGeneralizationOf(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Dictionary) return false ;
    auto castedPtr = static_cast<const Type*>(type) ;
    const bool keyIsGeneralized = keyType->isGeneralizationOf(type) ;
    const bool valueIsGeneralied = valueType->isGeneralizationOf(type) ;
    return keyIsGeneralized && valueIsGeneralied ;
}

bool DictionaryType::isSameAs(const horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Dictionary) return false ;
    auto castedPtr = static_cast<const DictionaryType*>(type) ;
    const bool keyIsSame = keyType->isSameAs(castedPtr->keyType) ;
    const bool valueIsSame = keyType->isSameAs(castedPtr->valueType) ;
    return keyIsSame && valueIsSame ;
}

Type* DictionaryType::getKeyType() const
{
    return keyType ;
}

DictionaryType& DictionaryType::setKeyType(Type* type)
{
    assert(type != nullptr) ;
    keyType = type ;
    return *this ;
}

Type* DictionaryType::getValueType() const
{
    return valueType ;
}

DictionaryType& DictionaryType::setValueType(Type* type)
{
    assert(type != nullptr) ;
    valueType = type ;
    return *this ;
}

void DictionaryType::__duplicateShallow(const DictionaryType* dicType)
{
    assert(dicType != nullptr) ;
    keyType = dicType->keyType ;
    valueType = dicType->valueType ;
    return ;
}

void DictionaryType::__duplicateDeep(const DictionaryType* dicType, ASTNode::MemManagerType& mem)
{
    assert(dicType != nullptr) ;
    Type* duplicateKeyType = nullptr ;
    if (dicType->keyType != nullptr) {
        duplicateKeyType = static_cast<Type*>(dicType->keyType->duplicateDeep(mem)) ;
        (void) duplicateKeyType->setParentASTNode(this) ;
    }
    keyType = duplicateKeyType ;
    Type* duplicateValueType = nullptr ;
    if (dicType->valueType != nullptr) {
        duplicateValueType = static_cast<Type*>(dicType->valueType->duplicateDeep(mem)) ;
        (void) duplicateValueType->setParentASTNode(this) ;
    }
    valueType = duplicateValueType ;
    return ;
}
