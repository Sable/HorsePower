#pragma once

#include <string>

#include "ASTNode.h"
#include "../grammar/HorseIRParser.h"

class Type : public ASTNode {
public:
    typedef HorseIRParser::TypeContext ParseTreeType ;
    enum class TypeClass {Scalar, List, Dictionary, Enumeration, Function} ;
    
    virtual constexpr Type::TypeClass getTypeClass() const = 0 ;
    virtual bool isGeneralizationOf(Type*) const = 0 ;

    static Type* makeTypeASTNode(Type::ParseTreeType* cst, ASTNodeMemory& mem) ;
} ;

class ScalarType : public Type {
public:
    typedef HorseIRParser::TypeCaseScalarContext ParseTreeType ;
    ScalarType() = delete ;
    ScalarType(ScalarType::ParseTreeType* cst, ASTNodeMemory& mem) ;
    
    enum class ScalarClass {
        Bool, Char, Integer8, Integer16, Integer32, Integer64, FP32, FP64,
        Complex, Symbol, Month, Date, DateTime, Minute, Second, Time, String,
        Table, KeyTable
    } ;
    virtual constexpr Type::TypeClass getTypeClass() const override ;
    virtual bool isGeneralizationOf(Type* type) const override ;
    
    virtual constexpr std::string toString() const override ;
    virtual constexpr std::string toTreeString() const override ;

    constexpr ScalarType::ScalarClass getScalarClass() const ;
protected:
    ScalarType::ScalarClass scalarClass ; 
} ;

class ListType : public Type {
public:
    typedef HorseIRParser::TypeCaseListContext ParseTreeType ;
    ListType() = delete ;
    ListType(ListType::ParseTreeType* cst, ASTNodeMemory& mem) ;

    virtual constexpr Type::TypeClass getTypeClass() const override ;
    virtual bool isGeneralizationOf(Type* type) const override ;

    virtual constexpr std::string toString() const override ;
    virtual constexpr std::string toTreeString() const override ;
    
    constexpr Type* getInternalType() const ;
protected:
    Type* elementType ;
} ;
