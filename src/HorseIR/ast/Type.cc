#include <cassert>

#include "Type.h"

Type* Type::makeTypeASTNode(Type::ParseTreeType *cst, ASTNodeMemory& mem)
{
    assert(cst != nullptr) ;
    ScalarType::ParseTreeType* scalar ;
    ListType::ParseTreeType* list ;

    if ((scalar = dynamic_cast<decltype(scalar)>(cst)) != nullptr) {
        ScalarType* ret = new ScalarType(scalar, mem) ;
        mem.manage(ret) ;
        return ret ;
    } else if ((list = dynamic_cast<decltype(list)>(cst)) != nullptr) {
        ListType* ret = new ListType(list, mem) ;
        mem.manage(ret) ;
        return ret ;
    } else {
        assert(false) ;
        return nullptr ;
    }
}



ScalarType::ScalarType(ScalarType::ParseTreeType* cst, ASTNodeMemory& mem)
{
    (void) mem ;
    assert(cst != nullptr) ;

    this->cst = dynamic_cast<antlr4::tree::ParseTree*>(cst) ;
    assert(this->cst != nullptr) ;
    
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

constexpr Type::TypeClass ScalarType::getTypeClass() const
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

constexpr std::string ScalarType::toString() const
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

constexpr std::string ScalarType::toTreeString() const 
{
    return "(ScalarType " + toString() + ")" ;
}

constexpr ScalarType::ScalarClass ScalarType::getScalarClass() const
{
    return scalarClass ;
}



ListType::ListType(ListType::ParseTreeType* cst, ASTNodeMemory& mem)
{
    assert(cst != nullptr) ;
    Type::ParseTreeType* listCTX ;
    listCTX = dynamic_cast<decltype(listCTX)>(cst->typeList()) ;
    assert(listCTX != nullptr) ;

    Type* internalType = Type::makeTypeASTNode(listCTX, mem) ;
}

constexpr Type::TypeClass ListType::getTypeClass() const
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

constexpr std::string ListType::toString() const
{
    return "list<" + elementType->toString() + ">" ;
}

constexpr std::string ListType::toTreeString() const
{
    return "(ListType " + elementType->toTreeString() + " )" ;
}

constexpr Type* ListType::getInternalType() const
{
    return elementType ;
}
