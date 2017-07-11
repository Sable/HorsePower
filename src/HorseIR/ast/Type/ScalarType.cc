#include <string>
#include <cassert>

#include "../AST.h"

using namespace horseIR::ast ;

ScalarType::ScalarType(ASTNode* parent, HorseIRParser::TypeCaseScalarContext* cst, ASTNode::MemManagerType& mem)
    : Type(parent, cst, mem, Type::TypeClass::Scalar, ASTNode::ASTNodeClass::ScalarType)
{
    assert(cst != nullptr) ;
    (void) mem ;
    
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

ScalarType::ScalarType(ScalarType::ScalarClass type, ASTNode::MemManagerType& mem)
    : scalarClass(type),
      Type(mem, Type::TypeClass::Scalar, ASTNode::ASTNodeClass::ScalarType)
{}

ScalarType::ScalarType(ASTNode::MemManagerType& mem)
    : scalarClass(ScalarType::ScalarClass::Integer8),
      Type(mem, Type::TypeClass::Scalar, ASTNode::ASTNodeClass::ScalarType)
{}
                  
bool ScalarType::isGeneralizationOf(const Type *type) const 
{
    assert(type != nullptr) ;
    if (type->getTypeClass() != Type::TypeClass::Scalar) return false ;
    auto scalarType = static_cast<const ScalarType*>(type) ;
    return this->scalarClass == scalarType->scalarClass ;
}

std::size_t ScalarType::getNumNodesRecursively() const
{
    return 1 ;
}

std::vector<ASTNode*> ScalarType::getChildren() const
{
    return std::vector<ASTNode*>() ;
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

ScalarType::ScalarClass ScalarType::getScalarClass() const
{
    return scalarClass ;
}

ScalarType& ScalarType::setScalarClass(const ScalarType::ScalarClass type)
{
    scalarClass = type ;
    return *this ;
}
