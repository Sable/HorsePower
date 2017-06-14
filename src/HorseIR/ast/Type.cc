#include <cassert>

#include "Type.h"

using Type = horseIR::ast::Type ;
Type* Type::makeTypeASTNode(HorseIRParser::TypeContext *cst, ASTNodeMemory& mem)
{
    assert(cst != nullptr) ;
    HorseIRParser::TypeCaseScalarContext* scalar = nullptr;
    HorseIRParser::TypeCaseWildcardContext* wildcard = nullptr ;
    HorseIRParser::TypeCaseListContext* list = nullptr;
    HorseIRParser::TypeCaseDictContext* dictionary = nullptr;
    HorseIRParser::TypeCaseEnumContext* enumeration = nullptr ;
    HorseIRParser::TypeCaseFuncContext* function = nullptr ;
    
    if ((scalar = dynamic_cast<decltype(scalar)>(cst)) != nullptr) {
        ScalarType* ret = new ScalarType(scalar, mem) ;
        mem.manage(ret) ;
        return ret ;
    } else if ((wildcard = dynamic_cast<decltype(wildcard)>(cst)) != nullptr) {
        WildcardType* ret = new WildcardType(wildcard, mem) ;
        mem.manage(ret) ;
        return ret ;
    } else if ((list = dynamic_cast<decltype(list)>(cst)) != nullptr) {
        ListType* ret = new ListType(list, mem) ;
        mem.manage(ret) ;
        return ret ;
    } else if ((dictionary = dynamic_cast<decltype(dictionary)>(cst)) != nullptr) {
        DictionaryType* ret = new DictionaryType(dictionary, mem) ;
        mem.manage(ret) ;
        return ret ;
    } else if ((enumeration = dynamic_cast<decltype(enumeration)>(cst)) != nullptr) {
        EnumerationType* ret = new EnumerationType(enumeration, mem) ;
        mem.manage(ret) ;
        return ret ;
    } else if ((function = dynamic_cast<decltype(function)>(cst)) != nullptr) {
        FunctionType* ret = new FunctionType(function, mem) ;
        mem.manage(ret) ;
        return ret ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}


using ScalarType = horseIR::ast::ScalarType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
ScalarType::ScalarType(HorseIRParser::TypeCaseScalarContext* cst, ASTNodeMemory& mem)
{
    (void) mem ;
    assert(cst != nullptr) ;
    this->cst = static_cast<decltype(this->cst)>(cst) ;
    
    const std::string tokenContent = cst->tokenValue->getText() ;
    if (tokenContent == "bool") {
        scalarClass = ScalarType::ScalarClass::Bool ;
    } else if (tokenContent == "char") {
        scalarClass = ScalarType::ScalarClass::Char ;
    } else if (tokenContent == "i8") {
        scalarClass = ScalarType::ScalarClass::Integer8 ;
    } else if (tokenContent == "i16") {
        scalarClass = ScalarType::ScalarClass::Integer16 ;
    } else if (tokenContent == "i32") {
        scalarClass = ScalarType::ScalarClass::Integer32 ;
    } else if (tokenContent == "i64") {
        scalarClass = ScalarType::ScalarClass::Integer64 ;
    } else if (tokenContent == "f32") {
        scalarClass = ScalarType::ScalarClass::FP32 ;
    } else if (tokenContent == "f64") {
        scalarClass = ScalarType::ScalarClass::FP64 ;
    } else if (tokenContent == "complex") {
        scalarClass = ScalarType::ScalarClass::Complex ;
    } else if (tokenContent == "sym") {
        scalarClass = ScalarType::ScalarClass::Symbol ;
    } else if (tokenContent == "m") {
        scalarClass = ScalarType::ScalarClass::Month ;
    } else if (tokenContent == "d") {
        scalarClass = ScalarType::ScalarClass::Date ;
    } else if (tokenContent == "z") {
        scalarClass = ScalarType::ScalarClass::DateTime ;
    } else if (tokenContent == "u") {
        scalarClass = ScalarType::ScalarClass::Minute ;
    } else if (tokenContent == "v") {
        scalarClass = ScalarType::ScalarClass::Second ;
    } else if (tokenContent == "t") {
        scalarClass = ScalarType::ScalarClass::Time ;
    } else if (tokenContent == "str") {
        scalarClass = ScalarType::ScalarClass::String ;
    } else if (tokenContent == "table") {
        scalarClass = ScalarType::ScalarClass::Table ;
    } else if (tokenContent == "ktable") {
        scalarClass = ScalarType::ScalarClass::KeyTable ;
    } else {
        assert(false) ;
    }
}

Type::TypeClass ScalarType::getTypeClass() const
{
    return Type::TypeClass::Scalar ;
}

bool ScalarType::isGeneralizationOf(Type *type) const 
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Scalar) return false ;
    ScalarType* scalarType = static_cast<ScalarType*>(type) ;
    return this->scalarClass == scalarType->scalarClass ;
}

std::string ScalarType::toString() const
{
    switch (scalarClass) {
    case ScalarType::ScalarClass::Bool:      return std::string("bool") ;
    case ScalarType::ScalarClass::Char:      return std::string("char") ;
    case ScalarType::ScalarClass::Integer8:  return std::string("i8") ;
    case ScalarType::ScalarClass::Integer16: return std::string("i16") ;
    case ScalarType::ScalarClass::Integer32: return std::string("i32") ;
    case ScalarType::ScalarClass::Integer64: return std::string("i64") ;
    case ScalarType::ScalarClass::FP32:      return std::string("f32") ;
    case ScalarType::ScalarClass::FP64:      return std::string("f64") ;
    case ScalarType::ScalarClass::Complex:   return std::string("complex") ;
    case ScalarType::ScalarClass::Symbol:    return std::string("sym") ;
    case ScalarType::ScalarClass::Month:     return std::string("m") ;
    case ScalarType::ScalarClass::Date:      return std::string("d") ;
    case ScalarType::ScalarClass::DateTime:  return std::string("z") ;
    case ScalarType::ScalarClass::Minute:    return std::string("u") ;
    case ScalarType::ScalarClass::Second:    return std::string("v") ;
    case ScalarType::ScalarClass::Time:      return std::string("t") ;
    case ScalarType::ScalarClass::String:    return std::string("str") ;
    case ScalarType::ScalarClass::Table:     return std::string("table") ;
    case ScalarType::ScalarClass::KeyTable:  return std::string("ktable") ;
    }
}

std::string ScalarType::toTreeString() const 
{
    return "(ScalarType " + toString() + ")" ;
}

constexpr ScalarType::ScalarClass ScalarType::getScalarClass() const
{
    return scalarClass ;
}


using WildcardType = horseIR::ast::WildcardType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
WildcardType::WildcardType(HorseIRParser::TypeCaseWildcardContext* cst, ASTNodeMemory& mem)
{
    (void) mem;
    assert(cst != nullptr) ;
    this->cst = static_cast<decltype(this->cst)>(cst) ;
}

Type::TypeClass WildcardType::getTypeClass() const
{
    return Type::TypeClass::Wildcard ;
}

bool WildcardType::isGeneralizationOf(horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    return true ;
}

std::string WildcardType::toString() const
{
    return "?" ;
}

std::string WildcardType::toTreeString() const
{
    return "(WildcardType)" ;
}


using ListType = horseIR::ast::ListType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
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


using DictionaryType = horseIR::ast::DictionaryType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
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


using EnumerationType = horseIR::ast::EnumerationType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
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



using FunctionType = horseIR::ast::FunctionType ;
using ASTNodeMemory = horseIR::ast::ASTNodeMemory ;
FunctionType::FunctionType(HorseIRParser::TypeCaseFuncContext* cst, ASTNodeMemory& mem)
{
    assert(cst != nullptr) ;
    this->cst = static_cast<decltype(this->cst)>(cst) ;
    
    HorseIRParser::TypeFunc0Context* type0 = nullptr ;
    HorseIRParser::TypeFunc1Context* type1 = nullptr ;
    HorseIRParser::TypeFunc2Context* type2 = nullptr ;
    HorseIRParser::TypeFunc3Context* type3 = nullptr ;

    auto funcCST = static_cast<HorseIRParser::TypeFuncContext*>(cst->typeFunc()) ;
    if ((type0 = dynamic_cast<decltype(type0)>(funcCST)) != nullptr) {
        flexable = false ;
        returnType = Type::makeTypeASTNode(type0->type(), mem) ;
    } else if ((type1 = dynamic_cast<decltype(type1)>(funcCST)) != nullptr) {
        flexable = true ;
        returnType = Type::makeTypeASTNode(type1->type(), mem) ;
    } else if ((type2 = dynamic_cast<decltype(type2)>(funcCST)) != nullptr) {
        flexable = false ;
        std::vector<HorseIRParser::TypeContext*> types(std::move(type2->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(*ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else if ((type3 = dynamic_cast<decltype(type3)>(funcCST)) != nullptr) {
        flexable = true ;
        std::vector<HorseIRParser::TypeContext*> types(std::move(type3->type())) ;
        for (auto ptr = types.cbegin(); ptr != types.cend(); ++ptr) {
            auto param = Type::makeTypeASTNode(*ptr, mem) ;
            if (ptr + 1 != types.cend()) {
                parameterTypes.push_back(param) ;
            } else {
                returnType = param ;
            }
        }
    } else {
        assert(false) ;
    }

    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        this->children.push_back(*ptr) ;
    }
    this->children.push_back(returnType) ;
}

Type::TypeClass FunctionType::getTypeClass() const
{
    return Type::TypeClass::Function ;
}

bool FunctionType::isGeneralizationOf(horseIR::ast::Type *type) const
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Function) return false ;
    auto functionType = static_cast<FunctionType*>(type) ;
    
    bool paramGeneralization = true ;
    if (flexable) {
        if (!(parameterTypes.size() <= functionType->parameterTypes.size())) {
            paramGeneralization = false ;
        }
    } else {
        if (!(parameterTypes.size() == functionType->parameterTypes.size())) {
            paramGeneralization = false ;
        }
    }
    if (!paramGeneralization) return false ;

    for (std::size_t ptr = 0; ptr < parameterTypes.size(); ++ptr) {
        const auto myParam = parameterTypes.at(ptr) ;
        const auto targetParam = parameterTypes.at(ptr) ;
        if (!myParam->isGeneralizationOf(targetParam)) return false ;
    }

    return returnType->isGeneralizationOf(functionType->returnType) ;
}

std::string FunctionType::toString() const
{
    std::ostringstream ostream ;
    ostream << "func<" ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        Type* paramType = *ptr ;
        ostream << paramType->toString()
                << ((ptr + 1 == parameterTypes.cend())? "" : ", ") ;
    }
    ostream << (flexable? ", ..." : "")
            << " :"
            << returnType->toString()
            << ">" ;
    return ostream.str() ;
}

std::string FunctionType::toTreeString() const
{
    std::ostringstream ostream ;
    ostream << std::boolalpha
            << "(FunctionType" ;
    for (auto ptr = parameterTypes.cbegin(); ptr != parameterTypes.cend(); ++ptr) {
        Type* paramType = *ptr ;
        ostream << " "
                << paramType->toTreeString() ;
    }
    ostream << " "
            << returnType->toTreeString()
            << " "
            << flexable
            << ")" ;
    return ostream.str() ;
}

constexpr std::size_t FunctionType::getMinNumParameters() const
{
    return parameterTypes.size() ;
}

constexpr bool FunctionType::isFlexable() const
{
    return flexable ;
}
