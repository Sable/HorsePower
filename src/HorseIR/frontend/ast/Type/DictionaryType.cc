#include <string>
#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

DictionaryType::DictionaryType(ASTNode* parent, HorseIRParser::TypeCaseDictContext* cst, ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::Dictionary, ASTNode::ASTNodeClass::DictionaryType)
{
    assert(cst != nullptr) ;

    auto dictCST = cst->typeDict() ;
    keyType = Type::makeTypeASTNode(this, dictCST->key, mem) ;
    valueType = Type::makeTypeASTNode(this, dictCST->value, mem) ;
}

DictionaryType::DictionaryType(ASTNode::MemManagerType& mem)
    : Type(mem, Type::TypeClass::Dictionary, ASTNode::ASTNodeClass::DictionaryType),
      keyType(nullptr),
      valueType(nullptr)
{}

bool DictionaryType::isGeneralizationOf(const Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Dictionary) return false ;
    auto dictType = static_cast<const DictionaryType*>(type) ;

    bool keyIsGeneralization = keyType->isGeneralizationOf(dictType->keyType) ;
    bool valueIsGeneralization = valueType->isGeneralizationOf(dictType->valueType) ;

    return keyIsGeneralization && valueIsGeneralization ;
}

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

Type* DictionaryType::getKeyType() const
{
    return keyType ;
}

DictionaryType& DictionaryType::setKeyType(const Type* type)
{
    assert(type != nullptr) ;
    keyType = const_cast<Type*>(type) ;
    return *this ;
}

Type* DictionaryType::getValueType() const
{
    return valueType ;
}

DictionaryType& DictionaryType::setValueType(const Type* type)
{
    assert(type != nullptr) ;
    valueType = const_cast<Type*>(type) ;
    return *this ;
}
