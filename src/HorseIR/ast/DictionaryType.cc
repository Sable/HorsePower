#include <string>
#include <cassert>

#include "Type.h"

using DictionaryType = horseIR::ast::DictionaryType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
using Type = horseIR::ast::Type ;

DictionaryType::DictionaryType(HorseIRParser::TypeCaseDictContext* cst, MemoryManager<ASTNode>& mem)
    : Type(cst, mem, Type::TypeClass::Dictionary)
{
    assert(cst != nullptr) ;

    auto dictCST = static_cast<HorseIRParser::TypeDictContext*>(cst->typeDict()) ;    
    keyType = Type::makeTypeASTNode(dictCST->key, mem) ;
    this->children.push_back(keyType) ;
    valueType = Type::makeTypeASTNode(dictCST->value, mem) ;
    this->children.push_back(valueType) ;
}

DictionaryType::DictionaryType(MemoryManager<ASTNode>& mem)
    : Type(mem, Type::TypeClass::Dictionary),
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

std::string DictionaryType::toString() const
{
    return "dict<" + keyType->toString() + ", " + valueType->toString() + ">" ;
}

std::string DictionaryType::toTreeString() const
{
    return "(DictionaryType " + keyType->toTreeString() + " " +
        valueType->toTreeString() + ")" ;
}

constexpr Type* DictionaryType::getKeyType() const
{
    return keyType ;
}

DictionaryType& DictionaryType::setKeyType(const Type* type)
{
    assert(type != nullptr) ;
    
    this->children.clear() ;
    
    keyType = const_cast<Type*>(type) ;
    this->children.push_back(keyType) ;
    if (valueType != nullptr) this->children.push_back(valueType) ;
    return *this ;
}

constexpr Type* DictionaryType::getValueType() const
{
    return valueType ;
}

DictionaryType& DictionaryType::setValueType(const Type* type)
{
    assert(type != nullptr) ;
    
    this->children.clear() ;
    if (keyType != nullptr) this->children.push_back(keyType) ;
    valueType = const_cast<Type*>(type) ;
    this->children.push_back(valueType) ;
    return *this ;
}
