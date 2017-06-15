#include <string>
#include <cassert>

#include "Type.h"

using DictionaryType = horseIR::ast::DictionaryType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
using Type = horseIR::ast::Type ;

DictionaryType::DictionaryType(HorseIRParser::TypeCaseDictContext* cst, ASTNodeMemory& mem)
{
    assert(cst != nullptr) ;
    this->cst = static_cast<decltype(this->cst)>(cst) ;
    auto dictCST = static_cast<HorseIRParser::TypeDictContext*>(cst->typeDict()) ;
    
    keyType = Type::makeTypeASTNode(dictCST->key, mem) ;
    this->children.push_back(keyType) ;
    valueType = Type::makeTypeASTNode(dictCST->value, mem) ;
    this->children.push_back(valueType) ;
}

Type::TypeClass DictionaryType::getTypeClass() const
{
    return Type::TypeClass::Dictionary ;
}

bool DictionaryType::isGeneralizationOf(Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Dictionary) return false ;
    DictionaryType* dictType = static_cast<DictionaryType*>(type) ;

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

constexpr Type* DictionaryType::getValueType() const
{
    return valueType ;
}


