#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <cassert>

#include "antlr4-runtime.h"
#include "../grammar/HorseIRParser.h"

class ASTNode {
public: 
    std::size_t getNumChildren() ;
    std::vector<ASTNode*>::const_iterator childConstBegin() ;
    std::vector<ASTNode*>::const_iterator childConstEnd() ;
    void appendChild(ASTNode*) ;
    ASTNode* getChildAt(std::size_t pos) ;

    antlr4::tree::ParseTree* getCST() ;
    virtual std::size_t getNumNodes() ;

    virtual std::string toString() = 0 ;
    virtual std::string toTreeString() = 0 ;
protected:
    antlr4::tree::ParseTree* cst ;
    std::vector<ASTNode*> children ;
} ;

class Type : public ASTNode {
public:
    enum class TypeClass {
        Scalar,
        List,
        Dictionary,
        Enumeration,
        Function
    } ;
    
    virtual Type::TypeClass getTypeClass() = 0 ;
    virtual bool isGeneralizationOf(Type*) = 0 ;
} ;

class ScalarType : public Type {
public:
    ScalarType() = delete ;
    ScalarType(HorseIRParser::TypeCaseScalarContext* cst) ;
    
    enum class ScalarClass {
        Bool,
        Char,
        Integer8,
        Integer16,
        Integer32,
        Integer64,
        FP32,
        FP64,
        Complex,
        Symbol,
        Month,
        Date,
        DateTime,
        Minute,
        Second,
        Time,
        String,
        Table,
        KeyTable
    } ;
    virtual Type::TypeClass getTypeClass() override ;
    virtual bool isGeneralizationOf(Type* type) override ;
    
    virtual std::size_t getNumNodes() override ;
    virtual std::string toString() override ;
    virtual std::string toTreeString() override ;

    ScalarType::ScalarClass getScalarClass() ;
protected:
    ScalarType::ScalarClass scalarClass ;
} ;

ScalarType::ScalarType(HorseIRParser::TypeCaseScalarContext* cst)
{
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

bool
ScalarType::isGeneralizationOf(Type *type)
{
    if (type->getTypeClass() != Type::TypeClass::Scalar) return false ;
    ScalarType *scalarType = static_cast<ScalarType*>(type) ;
    return this->scalarClass == scalarType->scalarClass ;
}

std::size_t
ScalarType::getNumNodes()
{
    return 1 ;
}

std::string
ScalarType::toString()
{
    switch (scalarClass) {
        case Sca
    }
}