#pragma once

#include <string>

#include "ASTNode.h"
#include "../grammar/HorseIRParser.h"

namespace horseIR {
    namespace ast {
        class Type : public ASTNode {
        public:
            enum class TypeClass {Scalar, List, Dictionary, Enumeration, Function} ;
            
            virtual constexpr Type::TypeClass getTypeClass() const = 0 ;
            virtual bool isGeneralizationOf(Type*) const = 0 ;
            
            static Type* makeTypeASTNode(HorseIRParser::TypeContext* cst, ASTNodeMemory& mem) ;
        } ;

        class ScalarType : public Type {
        public:
            ScalarType() = delete ;
            ScalarType(HorseIRParser::TypeCaseScalarContext* cst, ASTNodeMemory& mem) ;
            
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
            ListType() = delete ;
            ListType(HorseIRParser::TypeCaseListContext* cst, ASTNodeMemory& mem) ;
            
            virtual constexpr Type::TypeClass getTypeClass() const override ;
            virtual bool isGeneralizationOf(Type* type) const override ;
            
            virtual constexpr std::string toString() const override ;
            virtual constexpr std::string toTreeString() const override ;
            
            constexpr Type* getElementType() const ;
        protected:
            Type* elementType ;
        } ;

        class DictionaryType : public Type {
        public:
            DictionaryType() = delete ;
            DictionaryType(HorseIRParser::TypeCaseDictContext* cst, ASTNodeMemory& mem) ;
            
            virtual constexpr Type::TypeClass getTypeClass() const override ;
            virtual bool isGeneralizationOf(Type* type) const override ;
            
            virtual constexpr std::string toString() const override ;
            virtual constexpr std::string toTreeString() const override ;
            
            constexpr Type* getKeyType() const ;
            constexpr Type* getValueType() const ;
        protected:
            Type* keyType ;
            Type* valueType ;
        } ;
        
        class EnumerationType : public Type {
        public:
            EnumerationType() = delete ;
            EnumerationType(HorseIRParser::TypeCaseEnumContext* cst, ASTNodeMemory& mem) ;
            
            virtual constexpr Type::TypeClass getTypeClass() const override ;
            virtual bool isGeneralizationOf(Type* type) const override ;
            
            virtual constexpr std::string toString() const override ;
            virtual constexpr std::string toTreeString() const override ;
            
            constexpr Type* getElementType() const ;
        protected:
            Type* elementType ;
        } ;
    }
}
